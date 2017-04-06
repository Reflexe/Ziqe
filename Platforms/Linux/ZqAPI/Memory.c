/**
 * @file Memory.c
 * @author Shmuel Hazan (shmuelhazan0@gmail.com)
 *
 * Ziqe: copyright (C) 2016 Shmuel Hazan
 *
 * Ziqe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ziqe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/kernel.h>
#include <linux/memory.h>
#include <linux/mm.h>
#include <linux/anon_inodes.h>
#include <linux/file.h>

#include "asm/uaccess.h"

#include "ZqAPI/Memory.h"
#include "ZqAPI/Logging.h"

/**
   @brief Convert ZqMemoryProtection to page protection flags.
   @param protection
   @return
 */
inline_hint pgprot_t memory_prot_to_pgprot_kernel(ZqMemoryProtection protection) {
    switch(protection) {
    case (PROT_NONE):
        return PAGE_NONE;
    case (PROT_READ):
        return PAGE_KERNEL_RO;
    case (PROT_EXEC):
    case (PROT_READ|PROT_EXEC):
        return PAGE_KERNEL_RX;
    case (PROT_WRITE):
    case (PROT_WRITE|PROT_READ):
        return PAGE_SHARED;
    case (PROT_WRITE|PROT_EXEC):
    case (PROT_READ|PROT_WRITE|PROT_EXEC):
        return PAGE_SHARED_EXEC;
    default:
        ZQ_ON_BUG ("Invalid prot");
    }

    return PAGE_NONE;
}

ZqKernelAddress ZQ_SYMBOL(ZqMmAllocateContiguous) (ZqSizeType size)
{
    return (ZqKernelAddress) kmalloc (size, GFP_KERNEL);
}

ZqKernelAddress ZQ_SYMBOL(ZqMmAllocateAtomicContiguous) (ZqSizeType size)
{
    return kmalloc (size, GFP_ATOMIC);
}

ZqError ZQ_SYMBOL(ZqMmAllocateUserMemory)(ZqSizeType length,
                              ZqUserMemoryAreaProtection protection,
                              ZqUserAddress *result) {
    ZqUserAddress addr;

    addr = vm_mmap (NULL, /* flip */
                    0, /* user address */
                    length,
                    protection,
                    0, /* flags */
                    0); /* offset */

    *result = addr;
    // TODO: error handling.

    return ZQ_E_OK;
}

void ZQ_SYMBOL(ZqMmDeallocateUserMemory)(ZqUserAddress address, ZqSizeType length)
{
    vm_munmap ((unsigned long)address, length);
}

static int kernel_memory_vma_fault(struct vm_area_struct *vma,
                                  struct vm_fault *vmf) {
    unsigned long byte_offset_in_kernel_memory;
    struct page *page;

    /* The offset is (the offset in the file: the kernel address)
        + (the fault offset from this kernel address \ from this
           vm_area). */
    byte_offset_in_kernel_memory = ((ZqUserAddress)vmf->address - vma->vm_start)
                                   + (vma->vm_pgoff << PAGE_SHIFT);

    /* Convert the offset in the kernel memory (=memory address)
       to a page. */
    page = vmalloc_to_page((ZqKernelAddress*) byte_offset_in_kernel_memory);
    if (! page)
        return VM_FAULT_SIGSEGV;

    /* increase the page's ref count */
    get_page(page);

    vmf->page = page;
    return VM_FAULT_MAJOR;
}

struct vm_operations_struct kernel_memory_vm_ops = {
    .fault =    kernel_memory_vma_fault,
};

static int kernel_memory_mmap (struct file *filp,
                               struct vm_area_struct *vma) {
    // Initilize the vtable.
    vma->vm_ops = &kernel_memory_vm_ops;
    ZQ_UNUSED (filp);

    return 0;
}

static struct file_operations kernel_memory_fops = {
    .owner =     THIS_MODULE,
    .mmap =	     kernel_memory_mmap,
};

ZqError ZQ_SYMBOL(ZqMmMapKernelToUser)(ZqToUserMapContext *context) {
    // MAYBE: allocate it only once.
    struct file *flip = anon_inode_getfile("ZqKToU",
                                           &kernel_memory_fops,
                                           NULL, /* no private data */
                                           O_RDWR);

    context->destinationAddress = vm_mmap (flip,
                                           0,
                                           context->length,
                                           context->protection,
                                           0, /* flags */
                                           (unsigned long) context->sourceAddress); /* offset */

    // Now, the "callback" ops->mmap get called and updates the vma.

    fput (flip);

    return ZQ_E_OK;
}

void ZqMmLockUserMemoryRead()
{
    down_read (&current->mm->mmap_sem);
}

void ZqMmUnlockUserMemoryRead()
{
    up_read (&current->mm->mmap_sem);
}

void ZqMmLockUserMemoryWrite()
{
    down_write (&current->mm->mmap_sem);
}

void ZqMmUnlockUserMemoryWrite()
{
    down_write (&current->mm->mmap_sem);
}

/**
   @brief Release a memory pages array with the put_page function.
   @param pages   A pointer to a `struct page*` array.
   @param pages_count   The number of elements in @a pages.
 */
static void inline_hint zq_put_pages (struct page **pages,
                                      ZqSizeType pages_count)
{
    ZqSizeType i;

    for (i = 0; i < pages_count; ++i)
    {
        put_page (pages[i]);
    }
}

ZqError ZQ_SYMBOL(ZqMmMapUserToKernel) (ZqToKernelMapContext *context) {
    ZqSizeType firstPage = (context->sourceAddress & PAGE_MASK) >> PAGE_SHIFT;
    ZqSizeType lastPage  = ((context->sourceAddress + context->length -1) & PAGE_MASK) >> PAGE_SHIFT;
    ZqSizeType pagesCount = lastPage - firstPage;

    struct page **pagesArray = kmalloc (pagesCount * sizeof (struct page*), GFP_KERNEL);

    context->pagesArray = pagesArray;
    context->pagesArraySize = pagesCount;

    ZqMmLockUserMemoryRead ();
    {
        int ret;

        /* Pin these pages */
        ret = get_user_pages(context->sourceAddress,
                             pagesCount,
                             FOLL_GET | ((context->protection & PROT_WRITE) ? FOLL_WRITE : 0), /* write */
                             pagesArray,
                             NULL);

        if (ret != pagesCount) {
            /* Only part of these pages has been pinned
               put them */
            if (ret > 0)
                zq_put_pages (pagesArray, ret);

            ZqMmUnlockUserMemoryRead ();
            kfree (pagesArray);

            return ret;
        }
    }
    ZqMmUnlockUserMemoryRead ();

    // Map the user's pages to the kernel's virtual memory.
    context->destinationAddress = vmap (pagesArray,
                                        pagesCount,
                                        0 /* flags */,
                                        memory_prot_to_pgprot_kernel(context->protection));

    if (context->destinationAddress == NULL)
        return ZQ_E_MEM_FAULT;
    else
        return ZQ_E_OK;
}

void ZQ_SYMBOL(ZqMmUnmapUserToKernel) (ZqToKernelMapContext *context) {
    zq_put_pages (context->pagesArray, context->pagesArraySize);

    kfree (context->pagesArray);

    vunmap (context->destinationAddress);
}
