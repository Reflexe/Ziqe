/**
 * @file Memory.cpp
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
#include "Memory.hpp"

void *operator new(size_t count) {
    auto maybePointer = Ziqe::OS::Allocators::Kernel::Allocate (count);
    ZQ_ASSERT (maybePointer);

    return *maybePointer;
}

void operator delete(void *pointer, size_t size) noexcept
{
    Ziqe::OS::Allocators::Kernel::Deallocate (pointer);
    ZQ_UNUSED(size);
}
