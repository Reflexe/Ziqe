/**
 * @file Types.h
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

#include <cstdint>

namespace Ziqe {

typedef std::size_t SizeType;
typedef uint8_t     Byte;
typedef uint16_t    Word;
typedef uint32_t    DWord;
typedef uint64_t    QWord;

template<class T, class TT>
struct Pair
{
    T first;
    TT second;
};

} // namespace Ziqe

#endif // ZIQE_TYPES_H
