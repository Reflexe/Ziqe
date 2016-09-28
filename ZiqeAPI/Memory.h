/**
 * @file Memeory.h
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
 *
 * This file should be readed as a CPP file and a C file.
 */
#ifndef ZIQE_API_MEMORY_H
#define ZIQE_API_MEMORY_H

#include "Types.h"
#include "Macros.h"

ZQ_BEGIN_C_DECL

#include "MemoryConfig.h.generated"

static void ZqDeallocateVirtual (ZqKernelAddress address);
static ZqKernelAddress ZqAllocateVirtual (ZqSizeType size);

ZqBool ZqCopyToUser (ZqUserAddress destination,
                     ZqKernelAddress source,
                     ZqSizeType length);

/* Physical Page - A Memory page */

/**
 * @brief ZqAllocatePhysicalMemoryPage  Allocate an unused physical page.
 * @param memoryPage  Pointer to an allocated ZqPhysicalPage object.
 * @return ZQ_TRUE on success, ZQ_FALSE on error (probably out of memory).
 */
ZqBool ZqAllocatePhysicalMemoryPage (ZqPhysicalPage *memoryPage);
/**
 * @brief ZqDeallocatePhysicalMemoryPage  Deallocate an physical memory page.
 * @param memoryPage An valid and filled memory page object.
 */
void ZqDeallocatePhysicalMemoryPage (ZqPhysicalPage *memoryPage);

/**
 * @brief ZqMapPhysicalPageToKernel  Map an physical page to the kernel space.
 * @param mappedAddress  Will contain the mapped page's address (on success).
 * @param memoryPage  The page to map.
 * @return ZQ_TRUE on success, ZQ_FALSE on error (out of kernel virtual memory probably).
 *
 * @note The page will not be deallocted until it unmapped.
 */
ZqBool ZqMapPhysicalPageToKernel (ZqKernelAddress *mappedAddress,
                                  ZqPhysicalPage *memoryPage);
/**
 * @brief ZqUnmapPhysicalPageToKernel  Undo ZqMapPhysicalPageToKernel and
 *                                     possibly free the mapped physical page.
 * @param mappedAddress  The result of @arg mappedAddress in ZqMapPhysicalPageToKernel.
 */
void ZqUnmapPhysicalPageToKernel (ZqKernelAddress mappedAddress);

/* Virtual Pages (User Mapped Physical Paged) */
/**
 * @brief ZqChangeVirtualPagePermissions  Change a Virtual Page Permissions
 * @param page  The page.
 * @param newPermissions Set of new permissions.
 */
void ZqChangeVirtualPagePermissions (ZqVirtualPage *page,
                                     ZqVirtualPagePermissions newPermissions);

void ZqChangeAllVirtualPagesPermissions (ZqVirtualPagePermissions newPermissions);

/* User Virtual Memory Areas - User Virtual Memory Ranges */
typedef ZqBool(*ZqVirtualPagePermsFaultHandler) (ZqKernelAddress pointer,
                                                 ZqUserAddress addressFault,
                                                 ZqMemoryFaultFlags faultFlags,
                                                 ZqVirtualPage *failedPage);

void ZqSetWriteProtectedFaultHandler (ZqVirtualPagePermsFaultHandler handler,
                                      ZqVirtualMemoryArea *area,
                                      ZqKernelAddress pointer);
// MAYBE: move data.
ZqBool ZqCreateMemoryArea (ZqUserAddress address,
                           ZqSizeType areaSize,
                           ZqKernelAddress dataAddress,
                           ZqVirtualMemoryArea *result);

ZqBool ZqCreateOrOverrideMemoryArea (ZqUserAddress address,
                                     ZqSizeType areaSize,
                                     ZqKernelAddress dataAddress,
                                     ZqVirtualMemoryArea *result);

void ZqRemoveMemoryAreaByAddress (ZqUserAddress address,
                                  ZqSizeType areaSize);
void ZqRemoveMemoryArea (ZqVirtualMemoryArea *area);

ZqBool ZqGetMemoryArea (ZqUserAddress beginAddress,
                        ZqSizeType areaSize,
                        ZqVirtualMemoryArea *result);

// TODO: in the start, set all the vm_areas to read only. Then, wait for faults,
// On a fault, copy the page, then allow writing. On an sync event, we would restart
// it all over again.

// t-PhysicalMemoryPage, ZqMapToKernel,ZqUnmapToKernel.

// User Virtual Memory:
// Create memory area (addr, size, data, flags); Remove memory area, Create Or Override Memory Area.

#include "MemoryInline.h.generated"

ZQ_END_C_DECL

#endif
