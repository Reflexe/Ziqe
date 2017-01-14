/**
 * @file Types.hpp
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
#ifndef ZIQE_TYPES_H
#define ZIQE_TYPES_H

/* Types that aren't big enough to have their own file */

#include "ZqAPI/Types.h"
#include "Platforms/Macros.h"

ZQ_BEGIN_NAMESPACE

using ::int8_t;
using ::int16_t;
using ::int32_t;
using ::int64_t;

using ::uint8_t;
using ::uint16_t;
using ::uint32_t;
using ::uint64_t;

typedef uint64_t  SizeType;
typedef int64_t DifferenceType;

typedef decltype (nullptr) NullptrType;

ZQ_END_NAMESPACE

#endif // ZIQE_TYPES_H
