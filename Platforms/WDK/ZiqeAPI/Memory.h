/**
 * @file Memory.h
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
#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>

typedef void* ZqKernelAddress;
typedef uint64_t ZqUserAddress;
typedef uint64_t ZqSizeType;

namespace ZiqeAPI {
namespace Allocators {
struct Kernel {
     static ZqKernelAddress allocate(ZqSizeType size)
     {
         return ::ExAllocatePoolWithTag(::NonPagedPool, size, 'Ziqe');
     }

     static void deallocate(ZqKernelAddress address)
     {
         ::ExFreePool (address);
     }
};

struct KernelContiguous {
    static ZqKernelAddress allocate (ZqSizeType size) {
        return MmAllocateContiguousMemorySpecifyCache (size,
                                                       0,
                                                       1ull << sizeof (uint64_t),
                                                       sizeof uint64_t * 8,
                                                       ::Cached);
    }

    static void deallocate (ZqKernelAddress address)
    {
        MmFreeContiguousMemory (address);
    }
};

struct User {
    static ZqUserAddress allocate(ZqSizeType size)
    {

    }

    static void deallocate (ZqKernelAddress address)
    {
          (address);
    }
};
}
}

#endif // MEMORY_H
