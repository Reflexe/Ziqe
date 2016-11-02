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

#include "ThreadOwnerServer.hpp"

namespace Ziqe {

GlobalPeers::GlobalPeers(Base::UniquePointer<MessageStreamFactoryInterface> &&streamFactory)
    : mStreamFactory{Base::move (streamFactory)}
{

}

Base::Expected<GlobalThread, GlobalPeers::RunThreadError> GlobalPeers::runThread(const LocalThread &localThread,
                                                         const Base::SharedPointer<GlobalProcess> &globalProcess)
{
    // Check if we already have a GlobalProcess for this thread's process.
    if (! globalProcess) {
        /**
            * Create a connection from the factory.
            * Send a global lookup message.
            * Receive responses.
            * Turn the connection to unique, Answer one of them.
            * DONE.

            Q: Should the server do something about a lookup request?
            A: No! only send a response if he wants.
         */

        auto globalStream = mStreamFactory->createGlobalMessageStream ();
        globalStream.sendMessage (Protocol::MessagesGenerator::makeRunThreadPeerLookup ());

        auto globalServer = mStreamFactory->createMessageServerFromStream (Base::move (globalStream));

        waitForCurrentTask (mCurrentThreadSentTask, globalServer);

        // We have received a propose message.
        mCurrentThreadSentTask.getProposedClient ().sendMessage (Protocol::MessagesGenerator::makeRunThreadPeerAcceptPropose ());

        // Create ProcessPeers{Server,Client} and ThreadOwnerServer
        // ProcessPeersServer should send hello to the other ProcessPeersServer.
        auto newProcessPeersServer = ProcessPeersServer{Base::move (globalServer)};
        auto newThreadOwnerServer = ThreadOwnerServer{mStreamFactory->createMessageServer ()};


    } else {
        // Get this process' ProcessPeers

        // TODO: implementation :D
    }
}

void GlobalPeers::runThreadRequestWorker(Protocol::MessageStream &stream) {
    // Create a process peers

    auto newProcessPeers = ProcessPeersServer::CreateWithPeer (Base::move (stream),
                                                               static_cast<GlobalThreadID>(localThread.getThreadID ()));
}

void GlobalPeers::onMessageReceived(const Protocol::Message &type,
                                    Protocol::MessageStream::MessageFieldReader &fieldReader,
                                    Protocol::MessageStream &&messageStream)
{
    using Protocol::Message;

    // TODO:
    switch (type.getType ()) {
    case Message::Type::RunThreadPeerLookup:
        break;
    case Message::Type::RunThreadPeerLookupAcceptPropose:
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
