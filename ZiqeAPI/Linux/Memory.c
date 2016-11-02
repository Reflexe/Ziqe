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
#include <linux/slab.h>
#include <linux/memory.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>

#include "asm/uaccess.h"

#include "../Memory.h"

#if 0
void ZqRegisterLatePageFaultHandler(ZqPageFaultHandler handler)
{
    // TODO: in the start, set all the vm_areas to read only. Then, wait for faults,
    // On a fault, copy the page, then allow writing. On an sync event, we would restart
    // it all over again.

    // We should also consider the new API function.
    // t-ZqVirtualMemoryPage;
    // ZqChangeVirtualPagePermissions(), ZqChangeAllVirtualPagesPermissions,
    // ZqSetVirtualPermissionsFailureHandler (handler, level={USER_GLOBAL,PROCESS,VM_AREA,VIRTUAL_PAGE},
    // hookPlace={BEFORE_ACCESS_CHECK, BEFORE_SIGSEGV, BEFORE_COPY_ON_WRITE, AFTER_SWAP})

    // t-PhysicalMemoryPage, ZqMapToKernel,ZqUnmapToKernel.

    // User Virtual Memory:
    // Create memory area (addr, size, data, flags); Remove memory area, Create Or Override Memory Area.
}

void ZqUnregisterLatePageFaultHandler(void)
{

}

ZqBool ZqMapUserAddressToKernel(ZqUserAddress virtualAddress, ZqSizeType size, ZqKernelAddress *result)
{
    void *memory = vmalloc (size);
    unsigned long copied_bytes = copy_from_user (memory, (void *)virtualAddress, size);

    *result = memory;

    if (copied_bytes != size) {
        vfree (memory);
        return ZQ_FALSE;
    } else {
        return ZQ_TRUE;
    }

    // copy from user?
    // or just get_user_pages?
}

void ZqUnmapUserAddressToKernel(ZqKernelAddress kernelAddress)
{
    vfree ((void *) kernelAddress);
}

ZqUserAddress ZqAllocateUserMemory(ZqSizeType length) {
    ZqUserAddress addr;
    struct mm_struct *mm;

    mm = current->mm;

    down_write(&mm->mmap_sem);
    addr = (ZqAddress)do_mmap (NULL, 0, length, 0, 0, 0, 0, NULL);
    up_write(&mm->mmap_sem);

    return addr;
}

void ZqDeallocateUserMemory(ZqKernelAddress address, ZqSizeType length) {
    struct mm_struct *mm;

    mm = current->mm;

    down_write(&mm->mmap_sem);
    do_munmap (mm, (unsigned long)address, length);
    up_write(&mm->mmap_sem);
}

ZqBool ZqGetVirtualMemoryAreaByAddress(ZqVirtualAddress address, ZqVirtualMemoryArea *virtualMemoryArea) {
    struct mm_struct *mm;

    mm = current->mm;
    *virtualMemoryArea = find_vma (mm, address);

    return (*virtualMemoryArea == NULL ? ZQ_TRUE : ZQ_FALSE);
}


ZqBool ZqMapPhysicalPageToKernel(ZqKernelAddress *mappedAddress, ZqPhysicalPage *memoryPage)
{
    kmap ()
}

#endif
