/**
 * @file MemoryMap.hpp
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
#ifndef ZIQE_MEMORYMAP_H
#define ZIQE_MEMORYMAP_H

#include "Base/Vector.hpp"
#include "Base/RedBlackTree.hpp"
#include "Base/LinkedList.hpp"

namespace Ziqe {
namespace Protocol {

/// A collection of memory areas.
class MemoryMap
{
    typedef Base::Vector<uint8_t> VectorType;

    // TODO: implement after RedBlackTree
#if 0
    typedef Base::RedBlackTree<ZqUserAddress, Base::Vector<uint8_t>> MemoryTree;
    typedef typename MemoryTree::Iterator MemoryTreeIterator;

public:
    MemoryMap();

    void addArea (Base::Vector<uint8_t> &&vector);

    // On collision, *this will win.
    void merge(const MemoryMap &otherMap);

private:
    MemoryTree mMemoryAreasTree;

//    LinkedList<Pair<VectorType, ZqUserAddress>> mMemorAreas;
#endif
};

} // namespace Ziqe
} // namespace Protocol

#endif // ZIQE_MEMORYMAP_H
