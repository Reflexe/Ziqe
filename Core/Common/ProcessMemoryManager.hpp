/**
 * @file ProcessMemoryManager.h
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
#ifndef ZIQE_PROCESSMEMORYMANAGER_H
#define ZIQE_PROCESSMEMORYMANAGER_H

#include "Base/LocalProcess.h"
#include "Base/LinkedList.h"
#include "Base/SpinLock.h"

#include "MemoryRevision.h"

namespace Ziqe {

class ProcessMemoryManager
{
public:
    ProcessMemoryManager();

    void onWritePageFault (ZqUserAddress address);

    MemoryRevision createRevision ();

private:
    void setAllPagesReadOnly ();
    void setPageWrite ();

    MappedPageType mapUserPage(ZqUserAddress address);

    template<class T>
    struct UnmapUserAddressDeleter
    {
        void operator() (T *pointer)
        {
            ZqUnmapUserAddressToKernel (static_cast<ZqKernelAddress>(pointer));
        }
    };

    typedef UniquePointer<Byte[], UnmapUserAddressDeleter<Byte> MappedPageType;

    UniqueSpinLocked<LinkedList<Pair<MappedPageType, ZqUserAddress>>> mModifiedPages;

    LocalProcess mLocalProcess;
};

} // namespace Ziqe

#endif // ZIQE_PROCESSMEMORYMANAGER_H
