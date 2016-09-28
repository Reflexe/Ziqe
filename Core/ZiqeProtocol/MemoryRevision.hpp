/**
 * @file MemoryRevision.hpp
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
#ifndef ZIQE_MEMORYREVISION_H
#define ZIQE_MEMORYREVISION_H

#include "Base/Types.hpp"
#include "Base/HashTable.hpp"

#include "ZiqeAPI/Types.h"

#include "MemoryMap.hpp"

namespace Ziqe {

class MemoryRevision
{
public:
    typedef uint64_t ID;

    MemoryRevision();
    ALLOW_COPY_AND_MOVE (MemoryRevision)
    // TODO: memory map
#if 0
    /**
     * @brief Merge this two revision. If there're a collision, @c revision
     *        will win.
     * @param revision
     */
    void merge (const MemoryRevision &revision);

    MemoryRevision mergeNew (const MemoryRevision &revision) const;

    MemoryRevision fromChangedPages (const Base::LinkedList<Pair<ZqUserAddress,
                                     Base::Vector<uint8_t>>> &pagesAndPreviousPagesList);

private:
    Base::LinkedList<Base::Vector<uint8_t>> comparePages(ZqUserAddress address,
                                                         const Base::Vector<uint8_t> &previousPage)
    {
//        ZqKernelAddress mappedAddress;
//        auto result = ZqMapUserAddressToKernel (address,
//                                                static_cast<ZqSizeType>(previousPage.size ()),
//                                                &mappedAddress);

//        DEBUG_CHECK (result == ZQ_TRUE);

        // Compare Bytes and insert the differences to the linked list.
        return {};
    }

    MemoryMap mRevisionChanges;
#endif
};

} // namespace Ziqe

#endif // ZIQE_MEMORYREVISION_H
