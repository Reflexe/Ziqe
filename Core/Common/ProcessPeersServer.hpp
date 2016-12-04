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

#include "Common/Types.hpp"
#include "Common/MessageStreamFactoryInterface.hpp"

#include "Protocol/ThreadState.hpp"
#include "Protocol/MemoryMap.hpp"
#include "Protocol/MessageServer.hpp"

namespace Ziqe {

class ProcessPeersServer
{
public:
    struct OtherServers {
        typedef Base::Pair<Protocol::MessageStream::Address,
                            Protocol::MessageStream::Port> StreamInfoType;


        struct StreamInfoAndReferenceCount {
            StreamInfoType info;
            SizeType referenceCount;
        };

        typedef Base::LinkedList<StreamInfoAndReferenceCount> ConnectionListType;

        void addThreads(const Base::RawArray<HostedThreadID> &threadsToAdd,
                        const StreamInfoType &info)
        {
            mConnectionsList.emplace_back (info, threadsToAdd.size ());
            auto beforeEnd = mConnectionsList.beforeEnd ();

            for (const auto &threadID : threadsToAdd)
            {
                mThreadIDToStream[threadID] = beforeEnd;
            }
        }

        void removeThreads (const Base::RawArray<HostedThreadID> &threadsToRemove) {
            for (const auto &threadID : threadsToRemove) {
                auto theradIDIterator = mThreadIDToStream.find (threadID);

                auto currentCount = (theradIDIterator->second->referenceCount -= 1);
                if (currentCount == 0)
                    mConnectionsList.erase (theradIDIterator->second);

                mThreadIDToStream.erase (theradIDIterator);
            }
        }

        const ConnectionListType& getAllConnections () const
        {
            return mConnectionsList;
        }

        enum class FindThreadStreamError {
            InvalidThreadID
        };

        Base::Expected<StreamInfoType, FindThreadStreamError>
        findThreadStreamInfo (HostedThreadID threadID) const{
            auto iterator = mThreadIDToStream.find (threadID);

            if (iterator == mThreadIDToStream.end()) {
                return {FindThreadStreamError::InvalidThreadID};
            }

            // Copy: with a reference, it may be invalid
            // when returned to user (it get unlocked, and might get removed).
            return {iterator->second->info};
        }

        ConnectionListType mConnectionsList;
        Base::HashTable<HostedThreadID, typename ConnectionListType::Iterator> mThreadIDToStream;
    };

    /**
     * @brief LockedConnections  The type for the this process instance's shared other peers container.
     */
    typedef Base::RWLocked<OtherServers>        LockedConnections;
    typedef Base::RawPointer<LockedConnections> ConnectionsType;

    ProcessPeersServer(Protocol::MessageServer &&messageServer);
    ~ProcessPeersServer();

    ZQ_ALLOW_MOVE (ProcessPeersServer)
    ZQ_DISALLOW_COPY (ProcessPeersServer)

    ConnectionsType getConnections ()
    {
        return &mOtherServers;
    }

private:
    LocalThread *globalToLocalThread (HostedThreadID threadID) {
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
    void onStopThreadReceived (Protocol::MessageStream &stream, HostedThreadID threadID);
    void onContinueThread     (Protocol::MessageStream &stream, HostedThreadID threadID);
    void onKillThreadReceived (Protocol::MessageStream &stream, HostedThreadID threadID);

    void onRunThreadReceived (Protocol::MessageStream &stream,
                              HostedThreadID newThreadID,
                              Protocol::MemoryMap &currentMemoryMap,
                              Protocol::ThreadState &state);

    void onHelloReceived (Protocol::MessageStream &stream, const Base::RawArray<HostedThreadID> &newThreads);
    void onGoodbyeReceived (Protocol::MessageStream &stream, const Base::RawArray<HostedThreadID> &leavingThreads);

    Base::Vector<HostedThreadID> getProcessThreadIDs () const
    {
        return {mProcessLocalThreads.keysBegin (), mProcessLocalThreads.keysEnd ()};
    }

    template<class MessageType>
    void sendToAll (const MessageType &message) {
        auto readOtherServers{mOtherServers.getRead ()};

        for (auto &connectionInfo : readOtherServers.first.getAllConnections()) {
            auto newStream = mMessageFactory->createMessageStream (connectionInfo.info.first,
                                                                   connectionInfo.info.second);
            newStream.sendMessage (message);
        }
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
    Base::HashTable<HostedThreadID, LocalThread> mProcessLocalThreads;

    Base::UniquePointer<MessageStreamFactoryInterface> mMessageFactory;
};

} // namespace Ziqe

#endif // ZIQE_PROCESSPEERSSERVER_H
