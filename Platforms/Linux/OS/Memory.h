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
#ifndef ZIQE_API_LINUX_MEMORY_H
#define ZIQE_API_LINUX_MEMORY_H

#include "Types.h"
#include "Platforms/Macros.h"

ZQ_BEGIN_C_DECL

#include "MemoryConfig.gen.h"
#include "Errors.gen.h"

/**
  @brief The Ziqe Linux memory API.

  @todo Memory Pool API.
  */
#ifdef __cplusplus
extern void *vmalloc(unsigned long size);
extern void vfree(const void *addr);

extern void kfree (const void *block);
#else
#include <linux/slab.h>
#include <linux/vmalloc.h>
#endif

/**
   @brief Deallocate the result of ZqAllocateVirtual.
   @param address
 */
static inline_hint void ZqMmDeallocateVirtual (ZqKernelAddress address)
{
    vfree (address);
}

/**
   @brief Allocate virtual kernel memory.
   @param size
   @return
 */
static inline_hint ZqKernelAddress ZqMmAllocateVirtual (ZqSizeType size)
{
    return (ZqKernelAddress) vmalloc (size);
}

ZqKernelAddress ZqMmAllocateContiguous (ZqSizeType size);

static inline_hint void ZqMmDeallocateContiguous (ZqKernelAddress address)
{
    kfree (address);
}

ZqKernelAddress ZqMmAllocateAtomicContiguous (ZqSizeType size);

static void inline_hint ZqMmDeallocateAtomicContiguous (ZqKernelAddress address)
{
    kfree (address);
}

#define memcpy(d,s,l) __builtin_memcpy(d,s,l)
#define memset(d,c,l) __builtin_memset(d,c,l)
#define memcmp  __builtin_memcmp

typedef struct {
    ZqUserAddress sourceAddress;
    ZqSizeType length;
    ZqMemoryProtection protection;

    ZqKernelAddress destinationAddress;

    // _private
    struct page **pagesArray;
    ZqSizeType pagesArraySize;
} ZqToKernelMapContext;

typedef struct {
    ZqKernelAddress sourceAddress;
    ZqSizeType length;
    ZqMemoryProtection protection;

    ZqUserAddress destinationAddress;
} ZqToUserMapContext;

ZqError ZqMmAllocateUserMemory (ZqSizeType length,
                               ZqUserMemoryAreaProtection protection,
                               ZqUserAddress *result);
void ZqMmDeallocateUserMemory (ZqUserAddress address, ZqSizeType length);


ZqError ZqMmMapKernelToUser(ZqToUserMapContext *context);
void ZqMmUnmapKernelToUser (ZqToUserMapContext *context);

ZqError ZqMmMapUserToKernel (ZqToKernelMapContext *context);
void ZqMmUnmapUserToKernel (ZqToKernelMapContext *context);

/**
   @brief Lock the current process's memory for read.
 */
void ZqMmLockUserMemoryRead (void);
void ZqMmUnlockUserMemoryRead (void);

/**
   @brief Lock the current process's memory for write.
 */
void ZqMmLockUserMemoryWrite (void);
void ZqMmUnlockUserMemoryWrite (void);

ZQ_END_C_DECL


/* MAYBE: chname this file to Allocators, Mapping, Locks, ... */
#ifdef __cplusplus


#endif /* __cplusplus */

#endif
