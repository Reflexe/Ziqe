/**
 * @file MemoryMap.cpp
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
#include "MemoryMap.h"

#include "Base/SharedVector.h"

namespace Ziqe {

MemoryMap::MemoryMap()
{

}

// TODO: we're creating a vector from other vector in order to copy it to
// another vector. We can just copy it to the vector from the start.
Vector<Byte> MemoryMap::toVector() const{
    SizeType memorySize = 0;

    for (const auto &addressAndVector : mAddressBaseToMemory)
    {
        memorySize += addressAndVector.second;
    }

    SharedVector<Byte, Vector<Byte>, UniquePointer<Byte>> vector;
    vector.getVector ().resize (memorySize);

    for (const auto &addressAndVector : mAddressBaseToMemory)
    {
        vector.insertVectorAtBegin (addressAndVector.second);
    }

    return std::move (vector.getVector ());
}

} // namespace Ziqe
