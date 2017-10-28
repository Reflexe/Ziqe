/**
 * @file Memory.h
 * @author Shmuel Hazan (shmuelhazan0@gmail.com)
 *
 * Ziqe: copyright (C) 2017 Shmuel Hazan
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
#ifndef MEMORY_H
#define MEMORY_H

#include "CppCore/Macros.h"
#include "CppCore/Types.h"

typedef void*          ZqKernelAddress;
typedef const void *   ZqConstKernelAddress;
typedef ZqKernelAddress ZqMapableAddress;
typedef ZqKernelAddress ZqUserAddress;
typedef ZqKernelAddress ZqVirtualAddress;


#ifdef __cplusplus
# include <cstdlib>
#else
# include <cstdlib.h>
#endif

/**
   @brief Deallocate the result of ZqAllocateVirtual.
   @param address
 */
static inline_hint void ZQ_SYMBOL(ZqDeallocate) (ZqKernelAddress address)
{
    free (address);
}

/**
   @brief Allocate virtual kernel memory.
   @param size
   @return
 */
static inline_hint ZqKernelAddress ZQ_SYMBOL(ZqAllocate) (size_t size)
{
    return (ZqKernelAddress) malloc (size);
}


#endif // MEMORY_H
