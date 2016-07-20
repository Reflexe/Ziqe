/**
 * @file ProcessMemory.h
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
#ifndef ZIQE_PROCESSMEMORY_H
#define ZIQE_PROCESSMEMORY_H

#include <cstdint>

#include "Base/Types.h"
#include "Base/HashTable.h"
#include "Base/SharedVector.h"

namespace Ziqe {

class ProcessMemory
{
public:
    typedef SharedVector<Byte> MemoryPage;
    typedef QWord              Address;
    typedef uint64_t           Revision;

    struct MemoryConfiguration{
        enum class AddressSize{
            _16 = 0,
            _32,
            _64,
        };

        AddressSize addressSize;

        SizeType pageSize;
    };

    ProcessMemory(const MemoryConfiguration &memoryConfiguration);


    MemoryPage &lookupPage (Address address);

    Revision getRevision() const;

private:
    static Address alignMemoryAddressTo(Address address, Address alignTo);

    MemoryConfiguration mMemoryConfiguration;

    HashTable<Address, MemoryPage> mPagesTable;

    Revision mRevision;
};

} // namespace Ziqe

#endif // ZIQE_PROCESSMEMORY_H
