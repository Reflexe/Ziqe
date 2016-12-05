/**
 * @file Memeory.h
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
 *
 * This file should be readed as a CPP file and a C file.
 */
#ifndef ZIQE_API_MEMORY_H
#define ZIQE_API_MEMORY_H

#include "Types.h"
#include "Macros.h"

ZQ_BEGIN_C_DECL

#include "MemoryConfig.h.generated"
#include "Errors.h.generated"

/**
  @brief The Ziqe memory API.

  @todo Memory Pool API.

  In progress: user virtual memory managment.

  MAYBE: split this file to kernel / common functions and
         user virtual memory.
  MAYBE: let the implementation declare the functions with
         the right indicators (inline_hint-static, ???)
  */

/**
   @brief Deallocate the result of ZqAllocateVirtual.
   @param address
 */
static void ZqMmDeallocateVirtual (ZqKernelAddress address);

/**
   @brief Allocate virtual kernel memory.
   @param size
   @return
 */
static ZqKernelAddress ZqMmAllocateVirtual (ZqSizeType size);

/**
   @brief Copy
   @param destination
   @param source
   @param length
   @return
 */
ZqError ZqCopyToUser (ZqUserAddress destination,
                     ZqKernelAddress source,
                     ZqSizeType length);

ZqBool ZqCopyFromUser (ZqKernelAddress *destination,
                       ZqUserAddress source,
                       ZqSizeType length);

/* User Virtual Memory Areas - User Virtual Memory Ranges */
/*typedef ZqBool(*ZqVirtualPagePermsFaultHandler) (ZqKernelAddress pointer,
                                                 ZqUserAddress addressFault,
                                                 ZqMemoryFaultFlags faultFlags,
                                                 ZqVirtualPage *failedPage);*/
/*
void ZqSetWriteProtectedFaultHandler (ZqVirtualPagePermsFaultHandler handler,
                                      ZqVirtualMemoryArea *area,
                                      ZqKernelAddress pointer);
*/

ZqBool ZqMmAllocateUserMemory (ZqSizeType length,
                               ZqUserMemoryAreaProtection protection,
                               ZqUserAddress *result);
void ZqMmDeallocateUserMemory (ZqUserAddress address, ZqSizeType length);


ZqBool ZqMmMapKernelVirtualToUser (ZqKernelAddress address,
                                ZqUserMemoryAreaProtection protection,
                                ZqSizeType length,
                                ZqUserAddress *destinationAddress);
void ZqMmUnmapKernelVirtualToUser (ZqUserAddress address, ZqSizeType length);

void ZqMapPhysicalToUser (ZqUserAddress);
void ZqUnmapPhysicalToUser (void);

// Locks the current user's memory.
// TODO: required? yes, with user memory iteration
//       and lookup we would want to make sure that no one is
//       changing our stuff.
void ZqLockUserMemoryRead (void);
void ZqLockUserMemoryWrite (void);

typedef ZqBool(*ZqFaultHandler)(ZqVirtualMemoryArea *area, int);

/**
   @brief

   This struct can be change only if ZqLockUserMemoryWrite.

 */
//struct ZqVirtualMemoryArea {
//    // has
//    // has fault handler.
//    // begin, length

//    ZqUserAddress *begin;
//    ZqUserAddress *end;

//    ZqFaultHandler *handler;
//};

#include "MemoryInline.h.generated"

ZQ_END_C_DECL

#endif
