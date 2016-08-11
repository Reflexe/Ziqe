/**
 * @file ProcessPeersServer.cpp
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
#include "ProcessPeersServer.h"

namespace Ziqe {

ProcessPeersServer::ProcessPeersServer()
{

}

void ProcessPeersServer::addConnection(GlobalThreadID threadId,
                                       UniquePointer<NetworkProtocol> &&protocol) {
    RWLock::ScopedWriteLock lock{mConnections->first};

    mConnections->second.insert (threadId, std::move (protocol));
}

void ProcessPeersServer::removeConnection(GlobalThreadID threadId) {
    RWLock::ScopedWriteLock lock{mConnections->first};

    mConnections->second.erase (threadId);
}

void ProcessPeersServer::onGetMemoryReceived(ZqAddress address, SizeType length)
{

}

void ProcessPeersServer::onWriteMemoryReceived(ZqAddress address, const SharedVector<Byte> &memory)
{

}

void ProcessPeersServer::onStopThreadReceived(GlobalThreadID threadID) {
    auto thread = globalToLocalThread (threadID);
    DEBUG_CHECK_REPORT (thread);

    thread->stop ();
}

void ProcessPeersServer::onContiniueThread(GlobalThreadID threadID)
{
    auto thread = globalToLocalThread (threadID);
    DEBUG_CHECK_REPORT (thread);

    thread->cont ();
}

void ProcessPeersServer::onKillThreadReceived(GlobalThreadID threadID)
{
    auto thread = globalToLocalThread (threadID);
    DEBUG_CHECK_REPORT (thread);

    thread->kill ();
}

void ProcessPeersServer::onRunThreadReceived(GlobalThreadID newThreadID,
                                             MemoryMap &currentMemoryMap, ThreadState &state)
{

}

void ProcessPeersServer::onHelloReceived(GlobalThreadID newThread)
{
    addConnection (newThread, createProtocolFromn(mCurrentProcessedPacket));
}

void ProcessPeersServer::onGoodbyeReceived(UglyArray<GlobalThreadID> threads) {
    for (SizeType i = 0; i < threads.size (); ++i)
    {
        removeConnection (threads[i]);
    }
}

} // namespace Ziqe
