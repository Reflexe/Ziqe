/**
 * @file ProcessMemoryManager.cpp
 * @author shrek0 (shrek0.tk@gmail.com)
 *
 * Main: copyright (C) 2016 shrek0
 *
 * Main is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Main is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "ProcessMemoryManager.h"

#include "ZiqeAPI/Memory.h"

namespace Ziqe {

ProcessMemoryManager::ProcessMemoryManager()
    : mModifiedPages{makeUnique<SpinLock>()}
{

}

void ProcessMemoryManager::onWritePageFault(ZqUserAddress address) {
    ZqUserAddress alignedAddress = ZQ_PAGE_ALIGN (address);

    mModifiedPages.get ().first.emplace_back (Pair<MappedPageType, ZqUserAddress>{mapUserPage (alignedAddress),
                                                                                  alignedAddress});
    setPageWrite (address);
}

MemoryRevision ProcessMemoryManager::createRevision()
{
    auto listAndLock = mModifiedPages.get ();

    return newRevision.fromChangedPages (listAndLock.first);
}

void ProcessMemoryManager::setPageWrite()
{
}

ProcessMemoryManager::MappedPageType ProcessMemoryManager::mapUserPage(ZqUserAddress address) {
    ZqKernelAddress kernelAddress;

    DEBUG_CHECK (ZqMapUserAddressToKernel (address, ZQ_PAGE_SIZE, &kernelAddress) != ZQ_TRUE);

    return MappedPageType{kernelAddress};
}

} // namespace Ziqe
