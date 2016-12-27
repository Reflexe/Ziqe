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

#include "ZiqeAPI/Memory.h"
#include "ZiqeAPI/Logging.h"

ZqKernelAddress ZqMmAllocateContiguous (ZqSizeType size)
{
    return (ZqKernelAddress) kmalloc (size, GFP_KERNEL);
}

ZqKernelAddress ZqMmAllocateAtomicContiguous (ZqSizeType size)
{
    return kmalloc (size, GFP_ATOMIC);
}

ZqBool ZqMmAllocateUserMemory(ZqSizeType length,
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

    return ZQ_TRUE;
}

void ZqMmDeallocateUserMemory(ZqUserAddress address, ZqSizeType length)
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
    byte_offset_in_kernel_memory = ((ZqUserAddress)vmf->virtual_address - vma->vm_start)
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

static int kernel_memory_mmap (struct file *filp, struct vm_area_struct *vma) {
    // Initilize the vtable.
    vma->vm_ops = &kernel_memory_vm_ops;

    return 0;
}

struct file_operations kernel_memory_fops = {
    .owner =     THIS_MODULE,
    .mmap =	     kernel_memory_mmap,
};

ZqBool ZqMmMapKernelToUser(ZqToUserMapContext *context) {
    ZqUserAddress realDestinationAddress = context->destinationAddress;

    // MAYBE: allocate it only once.
    struct file *flip = anon_inode_getfile("ZqKToU",
                                           &kernel_memory_fops,
                                           NULL, /* no private data */
                                           O_RDWR);

    context->destinationAddress = vm_mmap (flip,
                                           realDestinationAddress,
                                           context->length,
                                           context->protection,
                                           0, /* flags */
                                           (unsigned long) context->sourceAddress); /* offset */

    // Now, the "callback" ops->mmap get called and updates the vma.

    // Make sure destinationAddress is really the requested address.
    // The kernel might change it.
    ZQ_BUG_IF_NOT (context->destinationAddress != realDestinationAddress
                   && realDestinationAddress != 0);

    fput (flip);

    return ZQ_TRUE;
}

ZqError ZqCopyToUser(ZqUserAddress destination,
                    ZqKernelAddress source,
                    ZqSizeType length)
{
    int ret = copy_to_user ((void __user*) destination,
                            source,
                            length);

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

static void inline_hint zq_put_pages (struct page **pages,
                                      ZqSizeType pages_count)
{
    ZqSizeType i;

    for (i = 0; i < pages_count; ++i)
    {
        put_page (pages[i]);
    }
}

// Should not be trusted to write to the user.
ZqError ZqMmMapUserToKernel(ZqToKernelMapContext *context) {
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
                             context->protection & PROT_WRITE, /* write */
                             0, /* force */
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

        // Map the user's pages to the kernel's virtual memory.
        context->destinationAddress = vmap (pagesArray,
                                            pagesCount,
                                            0 /* flags */,
                                            context->protection);
    }
    ZqMmUnlockUserMemoryRead ();

    return ZQ_E_OK;
}

void ZqMmUnmapUserToKernel(ZqToKernelMapContext *context) {
    zq_put_pages (context->pagesArray, pagesArraySize);

    kfree (context->pagesArray);

    vunmap (context->destinationAddress);
}
