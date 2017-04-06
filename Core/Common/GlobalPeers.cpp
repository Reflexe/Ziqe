/**
 * @file GlobalPeers.cpp
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
#include "GlobalPeers.hpp"

#include "ProcessPeersClient.hpp"

namespace Ziqe {

GlobalPeers::GlobalPeers(Base::UniquePointer<MessageStreamFactoryInterface> &&streamFactory)
    : mStreamFactory{Base::move (streamFactory)}
{

}

Base::Expected<Base::Pair<OwnedThread, OwnedProcess>, GlobalPeers::RunThreadError> GlobalPeers::runThread(const LocalThread &localThread) {
    auto globalStream = mStreamFactory->createGlobalMessageStream ();
    globalStream.sendMessage (Protocol::MessagesGenerator::makeRunThreadPeerLookup ());

    auto globalServer = mStreamFactory->createMessageServerFromStream (Base::move (globalStream));

    waitForTask (mCurrentThreadSentTask, globalServer);

    // We have received a propose message.
    mCurrentThreadSentTask.getProposedClient ().sendMessage (Protocol::MessagesGenerator::makeRunThreadPeerAcceptPropose ());

    // Create ProcessPeers{Server,Client} and ThreadOwnerServer
    // ProcessPeersServer should send hello to the other ProcessPeersServer.
    auto newProcessPeersServer  = ProcessPeersServer{Base::move (globalServer)};
    auto newThreadOwnerServer   = ThreadOwnerServer{mStreamFactory->createMessageServer ()};

    // How do the thread client know what the thread's port?

    return {};
}

void GlobalPeers::runThreadRequestWorker(Protocol::MessageStream &stream) {
    // Run when runThreadAcceptPropuse received.

    // Create a new process peers server and send "hello" to @a stream.
    auto newProcessPeers = ProcessPeersServer::CreateWithPeer (mStreamFactory->createMessageServer (),
                                                               Base::move (stream),
                                                               static_cast<HostedThreadID>(localThread.getThreadID ()));


}

void GlobalPeers::onMessageReceived(const Protocol::Message &type,
                                    Protocol::MessageStream::MessageFieldReader &fieldReader,
                                    Protocol::MessageStream &&messageStream)
{
    using Protocol::Message;

    switch (type.getType ()) {
    case Message::Type::RunThreadPeerLookup:
        messageStream.sendMessage (Protocol::MessagesGenerator::makeRunThreadPeerPropose ());
        break;
    case Message::Type::RunThreadPeerLookupAcceptPropose:
        // Create a ProcessPeersServer.
        break;
    case Message::Type::RunThreadPeerLookupPropose:
        if (! mCurrentThreadSentTask.isComplete ()) {
            mCurrentThreadSentTask.setProposedClient (Base::move (messageStream));
        }
        break;
    default:
        return;
    }
}

} // namespace Ziqe
