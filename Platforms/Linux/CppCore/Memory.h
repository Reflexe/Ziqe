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

#include "CppCore/Types.h"
#include "CppCore/Macros.h"

#include "CppCore/Error.h"

ZQ_BEGIN_C_DECL

typedef void*          ZqKernelAddress;
typedef const void *   ZqConstKernelAddress;
typedef ZqRegisterType ZqMapableAddress;
typedef ZqRegisterType ZqUserAddress;
typedef ZqRegisterType ZqVirtualAddress;

typedef int            ZqUserMemoryAreaProtection;

#define PROT_READ       0x1             /* page can be read */
#define PROT_WRITE      0x2             /* page can be written */
#define PROT_EXEC       0x4             /* page can be executed */
#define PROT_NONE       0x0             /* page can not be accessed */

typedef unsigned long ZqMemoryProtection;

/* Virtual Areas - one or more vm_area_struct */
typedef struct {
    ZqKernelAddress _pVmArea;
} ZqVirtualMemoryArea;

typedef unsigned long ZqMemoryMemoryAreaFlags;

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
static inline_hint void ZQ_SYMBOL(ZqDeallocate) (ZqKernelAddress address)
{
    vfree (address);
}

/**
   @brief Allocate virtual kernel memory.
   @param size
   @return
 */
static inline_hint ZqKernelAddress ZQ_SYMBOL(ZqAllocate) (ZqSizeType size)
{
    return (ZqKernelAddress) vmalloc (size);
}

ZqKernelAddress ZQ_SYMBOL(ZqMmAllocateContiguous) (ZqSizeType size);

static inline_hint void ZqMmDeallocateContiguous (ZqKernelAddress address)
{
    kfree (address);
}

ZqKernelAddress ZQ_SYMBOL(ZqMmAllocateAtomicContiguous) (ZqSizeType size);

static void inline_hint ZQ_SYMBOL(ZqMmDeallocateAtomicContiguous) (ZqKernelAddress address)
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

ZqError ZQ_SYMBOL(ZqMmAllocateUserMemory) (ZqSizeType length,
                               ZqUserMemoryAreaProtection protection,
                               ZqUserAddress *result);
void ZQ_SYMBOL(ZqMmDeallocateUserMemory) (ZqUserAddress address, ZqSizeType length);


ZqError ZQ_SYMBOL(ZqMmMapKernelToUser) (ZqToUserMapContext *context);
void ZQ_SYMBOL(ZqMmUnmapKernelToUser) (ZqToUserMapContext *context);

ZqError ZQ_SYMBOL(ZqMmMapUserToKernel) (ZqToKernelMapContext *context);
void ZQ_SYMBOL(ZqMmUnmapUserToKernel) (ZqToKernelMapContext *context);

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
