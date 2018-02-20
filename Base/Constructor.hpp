/**
 * @file Constructor.hpp
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
#ifndef CONSTRUCTOR_HPP
#define CONSTRUCTOR_HPP

#include "Base/Macros.hpp"

// Placement
inline void *operator new(unsigned long, void *p) noexcept{ return p; }
inline void *operator new[](unsigned long, void *p)  noexcept{ return p; }
inline void  operator delete  (void *, void *) noexcept { }
inline void  operator delete[](void *, void *) noexcept { }

ZQ_BEGIN_NAMESPACE
namespace Base {

/**
  @brief A constructor that can construct a @tparam T on element from
         StorageType that atleast in sizeof(T).
 */
template<class T, class StorageType>
struct CustomStorageConstructor {
    template<class ...Args>
    T *constructN(StorageType *pointer, SizeType n, Args ...args)
    {
        return ::new(static_cast<void*>(pointer)) T[n]{Base::forward<Args>(args)...};
    }

    template<class ...Args>
    T *construct(StorageType *pointer, Args&&...args)
    {
        return ::new(static_cast<void*>(pointer)) T{Base::forward<Args>(args)...};
    }

    /// These functions receives T* and not StorageType* because of
    /// strict aliasing rule.
    void destruct(T *pointer, SizeType n) {
        // Destruct in reverse
        while (n--)
            pointer[n].~T();
    }

    void destruct(T *pointer)
    {
        pointer->~T();
    }
};

/**
  @brief A default constructor that uses its own type
         as StorageType.
 */
template<class T>
using Constructor=CustomStorageConstructor<T, T>;

} // namespace Base
ZQ_END_NAMESPACE

#endif // CONSTRUCTOR_HPP
