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

#define _LINUX

#include <linux/kernel.h>
#include <linux/memory.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/anon_inodes.h>
#include <linux/file.h>

#include "asm/uaccess.h"

#include "../Memory.h"
#include "../Logging.h"

struct zq_kernel_memory_context{
    // IN
    ZqKernelAddress kernelAddress;
    // OUT
    struct vm_area_struct *vm_area;
};

//ZqBool ZqMapUserAddressToKernel(ZqUserAddress virtualAddress, ZqSizeType size, ZqKernelAddress *result) {
//    void *memory = vmalloc (size);
//    unsigned long copied_bytes = copy_from_user (memory, (void *)virtualAddress, size);

//    if (copied_bytes != size) {
//        vfree (memory);
//        return ZQ_FALSE;
//    } else {
//        *result = memory;
//        return ZQ_TRUE;
//    }

//    // copy from user?
//    // or just get_user_pages?
//}

//void ZqUnmapUserAddressToKernel(ZqKernelAddress kernelAddress)
//{
//    vfree ((void *) kernelAddress);
//}

//ZqUserAddress ZqAllocateUserMemory(ZqSizeType length, ZqUserMemoryAreaProtection protection) {
//    ZqUserAddress addr;

//    addr = vm_mmap (NULL, length, protection, 0, 0);

//    return addr;
//}

//ZqBool ZqGetVirtualMemoryAreaByAddress(ZqVirtualAddress address, ZqVirtualMemoryArea *virtualMemoryArea) {
//    struct mm_struct *mm;

//    mm = current->mm;
//    *virtualMemoryArea = find_vma (mm, address);

//    return (*virtualMemoryArea == NULL ? ZQ_TRUE : ZQ_FALSE);
//}

ZqBool ZqAllocateUserMemory(ZqSizeType length,
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

void ZqDeallocateUserMemory(ZqUserAddress address, ZqSizeType length)
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

    /* increase the page ref count */
    get_page(page);

    vmf->page = page;
    return VM_FAULT_MAJOR;
}

struct vm_operations_struct kernel_memory_vm_ops = {
    .fault =    kernel_memory_vma_fault,
};

static int kernel_memory_mmap (struct file *filp, struct vm_area_struct *vma) {
    struct zq_kernel_memory_context *memory_ctx = filp->private_data;

    memory_ctx->vm_area = vma;
    vma->vm_private_data = filp->private_data;

    // Initilize the vtable.
    vma->vm_ops = &kernel_memory_vm_ops;

    return 0;
}

struct file_operations kernel_memory_fops = {
    .owner =     THIS_MODULE,
    .mmap =	     kernel_memory_mmap,
};

ZqBool ZqMapKernelToUserMemory(ZqKernelAddress address,
                               ZqUserMemoryAreaProtection protection,
                               ZqSizeType length,
                               ZqUserAddress *destinationAddress)
{
    struct zq_kernel_memory_context params;
    ZqUserAddress realDestinationAddress = *destinationAddress;

    // MAYBE: allocate it only once.
    struct file *flip = anon_inode_getfile("ZqKToU",
                                           &kernel_memory_fops,
                                           &params,
                                           O_RDWR);

    params.kernelAddress = address;

    *destinationAddress = vm_mmap (flip,
                                   realDestinationAddress,
                                   length,
                                   protection,
                                   0,
                                   (unsigned long) address); /* offset */

    // Now, the "callback" ops->mmap get called and updates the vma.

    // Make sure destinationAddress is really the requested address.
    // The kernel might change it.
    ZQ_BUG_IF_NOT (*destinationAddress != realDestinationAddress
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
