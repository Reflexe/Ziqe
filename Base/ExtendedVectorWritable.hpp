/**
 * @file ExtendedVectorWritable.hpp
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
#ifndef ZIQE_BASE_EXTENDEDVECTORWRITABLE_HPP
#define ZIQE_BASE_EXTENDEDVECTORWRITABLE_HPP

#include "ExtendedVector.hpp"

namespace Ziqe {
namespace Base {

template<class _InputExtendedVectorType, class _OutputExtendedVectorType=_InputExtendedVectorType>
class ExtendedVectorWritable
{
public:
    typedef _InputExtendedVectorType InputExtendedVectorType;
    typedef _OutputExtendedVectorType OutputExtendedVectorType;

    ExtendedVectorWritable() = default;
    virtual ~ExtendedVectorWritable() = default;

    ZQ_ALLOW_COPY_AND_MOVE (ExtendedVectorWritable)

    static void ReadFrom (InputExtendedVectorType &) = delete;

    virtual void writeTo (OutputExtendedVectorType &vector) const = 0;
};

} // namespace Base
} // namespace Ziqe

#endif // ZIQE_BASE_EXTENDEDVECTORWRITABLE_HPP
