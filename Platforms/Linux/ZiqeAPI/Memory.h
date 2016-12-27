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

typedef uint64_t UserMemoryAddress;
class UserMemory {
private:
    UserMemory();


};

typedef void* KernelMemoryAddress;

class KernelMemory {
private:
    KernelMemory ();
};

class Mapping {
private:
};

/**
  Memory:

  */
#endif

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

ZqBool ZqMmAllocateUserMemory (ZqSizeType length,
                               ZqUserMemoryAreaProtection protection,
                               ZqUserAddress *result);
void ZqMmDeallocateUserMemory (ZqUserAddress address, ZqSizeType length);


ZqBool ZqMmMapKernelToUser (ZqToUserMapContext *context);
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

#include "Base/Expected.hpp"

ZQ_BEGIN_NAMESPACE
namespace OS {
namespace Allocators {
/**
   @brief The regular kernel allocator.

   Allocates page-aligned non-contiguous memory.

   @note If it wasn't page aligned, we wouldn't be able
         to map it to usermode safely (other data can be in
         the same page).
   @todo Seperate Base into
    * Base: The real basic, could be lower than ZiqeAPI, sometimes declare what ZiqeAPI should impelement.
    * ZiqeAPI: minimalistic per-platform implementation.
    * Utils: Less basic: Vector, the new operator, HashTable, LinkedList.
    * OS: High level operating system related classes (process manager, thread managment, Socket).
 */
struct Kernel {
     static Base::Expected<ZqKernelAddress, Error>
             Allocate(ZqSizeType size)
     {
         return ZqMmAllocateVirtual (size);
     }

     static void Deallocate(ZqKernelAddress address)
     {
         ZqMmDeallocateVirtual (address);
     }
};

struct User {
    typedef ZqUserMemoryAreaProtection Protection;

    static Base::Expected<ZqUserAddress, Error>
            Allocate(ZqSizeType size, Protection protection) {
        ZqUserAddress addressResult;
        auto result = ZqMmAllocateUserMemory (size, protection, &addressResult);

        if (! )
    }

    static void Deallocate(ZqUserAddress address, ZqSizeType size)
    {
        ZqMmDeallocateUserMemory (address, size);
    }
};

}
}
ZQ_END_NAMESPACE

#endif /* __cplusplus */

#endif
