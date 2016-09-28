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
#include "MemoryMap.hpp"

#include "Base/ExtendedVector.hpp"

namespace Ziqe {

#if 0
MemoryMap::MemoryMap()
{

}

void MemoryMap::merge(const MemoryMap &otherMap) {
    for (const auto &addressAndArea : otherMap.mMemoryAreasTree) {
        mMemoryAreasTree.insert (addressAndArea.first, addressAndArea.second);
        
        
    }
}

void MemoryMap::insertVector(const MemoryMap::AddressType address,
                             SizeType begin,
                             const MemoryMap::VectorType &copiedVector,
                             MemoryTreeIterator nextIterator)
{
    auto pastLastAddress = address + copiedVector.size ();

//    if (pastLastAddress > )
}

void MemoryMap::mergeSameAddressVectors(VectorType &vector,
                                        const VectorType &copiedVector) {
    // Check if we need some data from @c copiedVector; If not, just leave it as it is.
    if (vector.size () >= copiedVector.size ())
        return;

    SizeType bytesToCopy = copiedVector.size () - vector.size ();
    vector.expand (copiedVector.begin () + bytesToCopy, copiedVector.end (), bytesToCopy);
}

#endif

} // namespace Ziqe
