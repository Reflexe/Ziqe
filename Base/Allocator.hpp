/**
 * @file Allocator.hpp
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
#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include "Base/Macros.hpp"

// Define the new and delete operators.
void* operator new  (size_t count);
void operator delete (void *pointer) noexcept;

// delete with size for C++14.
void operator delete  (void* ptr, size_t size);

ZQ_BEGIN_NAMESPACE
namespace Base {

template<class T>
struct Allocator {
    T* allocate(SizeType n) {
        n *= sizeof(T);
        return static_cast<T*>(::operator new(n));
    }

    void deallocate(T *p)
    {
        ::operator delete(p);
    }
};

} // namespace Base
ZQ_END_NAMESPACE

#endif // ALLOCATOR_HPP
