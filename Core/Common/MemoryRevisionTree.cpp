/**
 * @file MemoryRevisionTree.cpp
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
#include "MemoryRevisionTree.hpp"

#include "Base/RedBlackTree.hpp"

namespace Ziqe {

MemoryRevisionTree::MemoryRevisionTree()
{

}

Protocol::MemoryRevision MemoryRevisionTree::diff(const MemoryRevision::ID &first) const{
    // First phase: check which data should we have in the new revision.
    // Second phase: actually prepare the new revision.
    // TODO redblack tree

//    RedBlackTree<ZqUserAddress, UglyPointer<Vector<uint8_t>>> memoryMap;

//    for (auto &revision : revisions) {
//        for (auto &revisionChange : revision) {
//            memoryMap[revisionChange.address] = ref(revisionChange);
//        }
//    }

//    for (auto &change : memoryMap)
//    {

//    }
}

const Protocol::MemoryRevision &MemoryRevisionTree::currentRevision() const
{
//    return mIDtoRevision.before_end ();
}

} // namespace Ziqe
