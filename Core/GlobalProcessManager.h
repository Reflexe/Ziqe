/**
 * @file GlobalProcessManager.h
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
#ifndef ZIQE_GLOBALPROCESSMANAGER_H
#define ZIQE_GLOBALPROCESSMANAGER_H

#include "Base/LinkedList.h"
#include "Base/HashTable.h"

#include "Core/GlobalThreadManager.h"
#include "Core/GlobalProcess.h"

namespace Ziqe {

class GlobalProcessManager
{
public:
    GlobalProcessManager(UniquePointer<GlobalPeersClient> &&globalPeersClient,
                         UglyPointer<GlobalThreadManager> threadManager);

    /**
     * @brief runThread  Run local thread by another peer.
     * @param thread
     * @param process
     */
    void runThread (LocalThread &thread, const LocalProcess &process, bool forceNewPeer);

    UglyPointer<GlobalProcess> localToGlobalProcess (const LocalProcess &process);

private:
    typedef HashTable<LocalProcess::ProcessID, GlobalProcess> ProcessTable;

    /// @return The global process's iterator.
    ProcessTable::Iterator addNewProcessConnection (const ProcessTable::Iterator &processIDLookupResult,
                                                    UniquePointer<NetworkProtocol> &&processConnection);

    HashTable<LocalProcess::ProcessID, GlobalProcess> mLocalToGlobal;

    UniquePointer<GlobalPeersClient> mGlobalPeersClient;

    UglyPointer<GlobalThreadManager> mThreadManager;
};

} // namespace Ziqe

#endif // ZIQE_GLOBALPROCESSMANAGER_H
