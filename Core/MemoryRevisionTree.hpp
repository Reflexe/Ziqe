/**
 * @file MemoryRevisionTree.hpp
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
#ifndef ZIQE_MEMORYREVISIONTREE_H
#define ZIQE_MEMORYREVISIONTREE_H

#include "Base/HashTable.hpp"

#include "Core/Protocol/MemoryRevision.hpp"

namespace Ziqe {

class MemoryRevisionTree
{
public:
    MemoryRevisionTree();

    using MemoryRevision=Protocol::MemoryRevision;

    /**
     * @brief Create a memory revision that represents one or more memory
     *        revisions.
     * @param first  An older memory revision to start diff-ing from.
     * @return
     */
    MemoryRevision diff (const MemoryRevision::ID &first) const;

    const MemoryRevision &currentRevision () const;

private:
    Base::RedBlackTree<MemoryRevision::ID, MemoryRevision> mIDtoRevision;
};

} // namespace Ziqe

#endif // ZIQE_MEMORYREVISIONTREE_H
