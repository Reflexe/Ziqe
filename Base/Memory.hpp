/**
 * @file Memory.hpp
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
#ifndef ZIQE_MEMORY_H
#define ZIQE_MEMORY_H

#include "Types.hpp"
#include "Macros.hpp"
#include "Checks.hpp"
#include "Allocator.hpp"

ZQ_BEGIN_NAMESPACE
namespace Base {

template<class T>
struct DefaultDeleter {
    void operator() (T *pointer)
    {
        delete pointer;
    }
};

template<class T>
struct DefaultDeleter<T[]> {
    void operator() (T *pointer)
    {
        delete[] pointer;
    }
};

template<class T>
struct NoDeleter {
    void operator() (T *)
    {
    }
};

} // namespace Base
ZQ_END_NAMESPACE

#endif // ZIQE_MEMORY_H
