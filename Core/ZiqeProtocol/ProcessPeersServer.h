/**
 * @file ProcessPeersServer.h
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
#ifndef ZIQE_PROCESSPEERSSERVER_H
#define ZIQE_PROCESSPEERSSERVER_H

#include "Base/Memory.h"
#include "Base/Types.h"
#include "Base/RWLock.h"
#include "Base/LocalThread.h"

#include "Network/NetworkProtocol.h"

#include "Core/GlobalThread.h"

namespace Ziqe {

class ProcessPeersServer : public NetworkProtocol::Callback
{
public:
    typedef SharedPointer<Pair<RWLock, HashTable<GlobalThreadID, NetworkProtocol>>> ConnectionsType;

    ProcessPeersServer();

    const ConnectionsType &getConnections ();

private:
    /// "Global" remote threads in the same process.
    /// Access Rules: Lock: mConnectionsTableLock; When: Result Usage, Read, Write.
    ConnectionsType mConnections;

    void addConnection (GlobalThreadID threadId, UniquePointer<NetworkProtocol> &&protocol);
    void removeConnection (GlobalThreadID threadId);

    /// "Global" local threads in the same process.
    HashTable<GlobalThreadID, LocalThread> mProcessLocalThreads;

    UniquePointer<NetworkPacket> mCurrentProcessedPacket;

    LocalThread *globalToLocalThread (GlobalThreadID threadID) {
        auto iterator = mProcessLocalThreads.find (threadID);

        if (iterator == mProcessLocalThreads.end ())
            return nullptr;
        else
            return &(*iterator);
    }

    void onGetMemoryReceived (ZqAddress address, SizeType length);
    void onWriteMemoryReceived (ZqAddress address,
                                const SharedVector<Byte> &memory);

    void onStopThreadReceived (GlobalThreadID threadID);
    void onContiniueThread    (GlobalThreadID threadID);
    void onKillThreadReceived (GlobalThreadID threadID);

    void onRunThreadReceived (GlobalThreadID newThreadID, MemoryMap &currentMemoryMap,
                              ThreadState &state);

    void onHelloReceived (GlobalThreadID newThread);
    void onGoodbyeReceived (UglyArray<GlobalThreadID> threads);
};

} // namespace Ziqe

#endif // ZIQE_PROCESSPEERSSERVER_H
