/**
 * @file MemoryRevision.h
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

#include "Base/Types.h"
#include "Base/HashTable.h"

#include "ZiqeAPI/Types.h"

namespace Ziqe {

class MemoryRevision
{
public:
    typedef uint64_t ID;

    /**
     * @brief Merge this two revision. If there're a collision, @c revision
     *        will win.
     * @param revision
     * @return The new revision.
     */
    MemoryRevision merge (const MemoryRevision &revision) const;

    MemoryRevision();

    MemoryRevision fromChangedPages (const LinkedList<Pair<ZqUserAddress,
                                                           Vector<Byte>>> &pagesAndPreviousPagesList);

private:
    LinkedList<ZqUserAddress, Vector<Byte>> comparePages(ZqUserAddress address,
                                                         const Vector<Byte> &previousPage)
    {
        ZqKernelAddress mappedAddress;
        auto result = ZqMapUserAddressToKernel (address,
                                                previousPage.size (),
                                                &mappedAddress);

        DEBUG_CHECK (result == ZQ_TRUE);

        // Compare Bytes and insert the differences to the linked list.
    }

    HashTable<ZqAddress, Vector<Byte>> mRevisionChanges;
};

} // namespace Ziqe

#endif // ZIQE_MEMORYREVISION_H
