/**
 * @file ProcessPeersServer.hpp
 * @author Shmuel Hazan (shmuelhazan0@gmail.com)
 *
 * Ziqe: copyright (C) 2016 Shmuel Hazan
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

#include "Base/Memory.hpp"
#include "Base/Types.hpp"
#include "Base/RWLock.hpp"
#include "Base/LocalThread.hpp"
#include "Base/LinkedList.hpp"
#include "Base/HashTable.hpp"

#include "Core/Types.hpp"
#include "Core/Protocol/ThreadState.hpp"
#include "Core/Protocol/MemoryMap.hpp"
#include "Protocol/MessageServer.hpp"

namespace Ziqe {

class ProcessPeersServer final
{
public:
    struct OtherServers {
        void addThreads(const Base::RawArray<GlobalThreadID> &threadsToAdd,
                        Base::UniquePointer<Protocol::MessageStream> &&stream)
        {
            mConnectionsList.emplace_back (Base::move (*stream));
            auto beforeEnd = mConnectionsList.beforeEnd ();

            for (const auto &threadID : threadsToAdd)
            {
                mThreadIDToStream[threadID] = beforeEnd;
            }
        }

        void removeThreads (const Base::RawArray<GlobalThreadID> &threadsToRemove) {
            for (const auto &threadID : threadsToRemove)
            {
                mThreadIDToStream.erase (threadID);
            }
        }

        void sendMessageToProcessPeers (const Protocol::MessageStream::OutputDataType &vector) const{
            for (const auto &connection : mConnectionsList)
            {
                connection.sendMessage (vector);
            }
        }

        bool sendThreadMessage (GlobalThreadID threadID, const Protocol::MessageStream::OutputDataType &vector) const{
            auto iterator = mThreadIDToStream.find (threadID);

            if (iterator == mThreadIDToStream.end()) {
                DEBUG_CHECK_NOT_REACHED ();

                return false;
            }

            (*iterator->second).sendMessage (vector);
            return true;
        }

        typedef Base::LinkedList<Protocol::MessageStream> ConnectionListType;

        ConnectionListType mConnectionsList;
        Base::HashTable<GlobalThreadID, typename ConnectionListType::Iterator> mThreadIDToStream;
    };

    /**
     * @brief LockedConnections  The type for the this process instance's shared other peers container.
     */
    typedef Base::RWLocked<OtherServers>        LockedConnections;
    typedef Base::RawPointer<LockedConnections> ConnectionsType;

    ProcessPeersServer(Protocol::MessageServer &&messageServer);
    ~ProcessPeersServer();

    static ProcessPeersServer CreateWithPeer (Protocol::MessageServer &&server, Protocol::MessageStream &&messageStream,
                                              GlobalThreadID peerThreadID);

    ZQ_ALLOW_MOVE (ProcessPeersServer)
    ZQ_DISALLOW_COPY (ProcessPeersServer)

    ConnectionsType getConnections ()
    {
        return &mOtherServers;
    }

private:
    LocalThread *globalToLocalThread (GlobalThreadID threadID) {
        auto iterator = mProcessLocalThreads.find (threadID);

        if (iterator == mProcessLocalThreads.end ())
            return nullptr;
        else
            return &(iterator)->second;
    }

    // Main message processor.
    void onMessageReceived (const Protocol::Message &type,
                            Protocol::MessageStream::MessageFieldReader &fieldReader,
                            const Protocol::MessageStream &messageStream) ;

    // Messages processors.
    void onStopThreadReceived (Protocol::MessageStream &stream, GlobalThreadID threadID);
    void onContinueThread     (Protocol::MessageStream &stream, GlobalThreadID threadID);
    void onKillThreadReceived (Protocol::MessageStream &stream, GlobalThreadID threadID);

    void onRunThreadReceived (Protocol::MessageStream &stream,
                              GlobalThreadID newThreadID,
                              Protocol::MemoryMap &currentMemoryMap,
                              Protocol::ThreadState &state);

    void onHelloReceived (Protocol::MessageStream &stream, const Base::RawArray<GlobalThreadID> &newThreads);
    void onGoodbyeReceived (Protocol::MessageStream &stream, const Base::RawArray<GlobalThreadID> &leavingThreads);

    Base::Vector<GlobalThreadID> getProcessThreadIDs () const
    {
        return {mProcessLocalThreads.keysBegin (), mProcessLocalThreads.keysEnd ()};
    }

    // Global Senders
    void sendHello ();
    void sendGoodbye ();

    // Stream Specific Senders.
    void sendRunThreadPropose (Net::Stream &protocol);

    Protocol::MessageServer mServer;

    /// "Global" remote process instances in the same process.
    /// Access Rules: Lock: mConnectionsTableLock; When: Result Usage, Read, Write.
    LockedConnections mOtherServers;

    /// "Global" local threads in the same process.
    Base::HashTable<GlobalThreadID, LocalThread> mProcessLocalThreads;
};

} // namespace Ziqe

#endif // ZIQE_PROCESSPEERSSERVER_H
