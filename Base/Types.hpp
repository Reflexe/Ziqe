/**
 * @file Types.hpp
 * @author shrek0 (shrek0.tk@gmail.com)
 *
 * Ziqe: copyright (C) 2016 shrek0
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

#include "ZiqeAPI/Types.h"

namespace Ziqe {

typedef ZqSizeType SizeType;
typedef ZqDifferenceType DifferenceType;


typedef decltype (nullptr) NullptrType;

template<class T1, class T2>
struct Pair
{
    T1 first;
    T2 second;
};

template<class T1, class T2, class T3>
struct Triple
{
    T1 first;
    T2 second;
    T3 third;
};

} // namespace Ziqe

#endif // ZIQE_TYPES_H
