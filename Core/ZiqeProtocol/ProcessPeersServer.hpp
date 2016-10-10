/**
 * @file ProcessPeersServer.hpp
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

#include "Base/Memory.hpp"
#include "Base/Types.hpp"
#include "Base/RWLock.hpp"
#include "Base/LocalThread.hpp"
#include "Base/LinkedList.hpp"
#include "Base/HashTable.hpp"

#include "Core/Types.hpp"
#include "Core/ZiqeProtocol/ThreadState.hpp"
#include "Core/ZiqeProtocol/MemoryMap.hpp"
#include "Core/ZiqeProtocol/MessageStream.hpp"

namespace Ziqe {

class ProcessPeersServer final : implements private MessageStream::Callback
{
public:
    struct OtherServers {
        void addThreads(const Base::RawArray<GlobalThreadID> &threadsToAdd,
                        Base::UniquePointer<MessageStream> &&stream)
        {

        }

        void removeThreads (const Base::RawArray<GlobalThreadID> &threadsToRemove) {
            for (const auto &threadID : threadsToRemove)
            {
                mThreadIDToStream.erase (threadID);
            }
        }

        void sendMessageToProcessPeers (const Net::Stream::DataType &vector) const{
            for (const auto &connection : mConnectionsList)
            {
                connection.sendMessage (vector);
            }
        }

        bool sendThreadMessage (GlobalThreadID threadID, const MessageStream::InputDataType &vector) const{
            auto iterator = mThreadIDToStream.find (threadID);

            if (iterator == mThreadIDToStream.end()) {
                DEBUG_CHECK_NOT_REACHED ();

                return false;
            }

            (*iterator->second).sendMessage (vector);
            return true;
        }

        typedef Base::LinkedList<MessageStream> ConnectionListType;

        ConnectionListType mConnectionsList;
        Base::HashTable<GlobalThreadID, typename ConnectionListType::Iterator> mThreadIDToStream;
    };

    typedef Base::RWLocked<Base::RawPointer<OtherServers>> ConnectionsType;

    ProcessPeersServer();
    ~ProcessPeersServer();

    ZQ_ALLOW_MOVE (ProcessPeersServer)
    ZQ_DISALLOW_COPY (ProcessPeersServer)

    const ConnectionsType &getConnections ()
    {
        return mOtherServers;
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
    void onMessageReceived (const Message &type,
                            MessageFieldReader &fieldReader,
                            const Net::Stream &stream) override;

    // Messages processors.
    void onStopThreadReceived (MessageStream &stream, GlobalThreadID threadID);
    void onContinueThread     (MessageStream &stream, GlobalThreadID threadID);
    void onKillThreadReceived (MessageStream &stream, GlobalThreadID threadID);

    void onRunThreadReceived (MessageStream &stream,
                              GlobalThreadID newThreadID,
                              MemoryMap &currentMemoryMap,
                              ThreadState &state);

    void onHelloReceived (MessageStream &stream, const Base::RawArray<GlobalThreadID> &newThreads);
    void onGoodbyeReceived (MessageStream &stream, const Base::RawArray<GlobalThreadID> &leavingThreads);

    Base::Vector<GlobalThreadID> getProcessThreadIDs () const
    {
        return {mProcessLocalThreads.keysBegin (), mProcessLocalThreads.keysEnd ()};
    }

    // Global Senders
    void sendHello ();
    void sendGoodbye ();

    // Stream Specific Senders.
    void sendRunThreadPropose (Net::Stream &protocol);

private:
    Base::RWLock mLock;

    /// "Global" remote process instances in the same process.
    /// Access Rules: Lock: mConnectionsTableLock; When: Result Usage, Read, Write.
    ConnectionsType mOtherServers;

    /// "Global" local threads in the same process.
    Base::HashTable<GlobalThreadID, LocalThread> mProcessLocalThreads;
};

} // namespace Ziqe

#endif // ZIQE_PROCESSPEERSSERVER_H
