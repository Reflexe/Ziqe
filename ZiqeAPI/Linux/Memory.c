/**
 * @file Memory.c
 * @author shrek0 (shrek0.tk@gmail.com)
 *
 * Ziqe: copyright (C) 2016 shrek0
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
#include <linux/slab.h>
#include <linux/memory.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>

#include "asm/uaccess.h"

#include "Memory.h"

inline void ZqDeallocateVirtual (ZqKernelAddress address)
{
    vfree ((void *) address);
}

inline ZqKernelAddress ZqAllocateVirtual (ZqKernelAddress size)
{
    return (ZqKernelAddress) vmalloc (size);
}

void ZqRegisterLatePageFaultHandler(ZqPageFaultHandler handler)
{
 // change all the vma in this task to call this handler instead of the default.

    // Create a simple diassembler that detect what size of data required, and what the dst
    // of this instruction. Then, send a (get|write)Memory and write the result to dst
    // and skip the fault instruction.

    // That way we keep write and read atomic, but with a lot of overhead.

    // We can also just request the complete page and create it on the requested computer.
    // but then read/write aren't not atomic.
}

void ZqUnregisterLatePageFaultHandler()
{

}

ZqBool ZqMapUserAddressToKernel(ZqUserAddress virtualAddress, ZqSizeType size, ZqAddress *result)
{
    void *memory = vmalloc (size);
    unsigned long result = copy_from_user (memory, virtualAddress, result);

    if (result != size)
        return ZQ_FALSE;
    else
        return ZQ_TRUE;


    // copy from user?
    // or just get_user_pages?
}

void ZqUnmapUserAddressToKernel(ZqAddress kernelAddress)
{
    vfree ((void *) kernelAddress);
}

ZqUserAddress ZqAllocateUserMemory(ZqSizeType length) {
    ZqAddress addr;
    struct mm_struct *mm;

    mm = current->mm;

    down_write(&mm->mmap_sem);
    addr = (ZqAddress)do_mmap (NULL, 0, length, PROT_NONE, 0, 0);
    up_write(&mm->mmap_sem);

    return addr;
}

void ZqDeallocateUserMemory(ZqKernelAddress address, ZqSizeType length) {
    struct mm_struct *mm;

    mm = current->mm;

    down_write(&mm->mmap_sem);
    do_munmap (mm, address, length);
    up_write(&mm->mmap_sem);
}
