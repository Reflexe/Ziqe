/**
 * @file PeerLookupClient.cpp
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
#include "PeerLookupClient.hpp"

#include "Protocol/MessagesGenerator.hpp"

namespace Ziqe {
namespace Client {

PeerLookupClient::PeerLookupClient(Base::UniquePointer<MessageStreamFactoryInterface> &&factory)
{

}

Base::Expected<Base::Pair<Thread, Process>, PeerLookupClient::RunThreadError>
PeerLookupClient::runThread(const LocalThread &localThread) {
    auto globalStream = mStreamFactory->createGlobalMessageStream ();
    globalStream.sendMessage (Protocol::MessagesGenerator::makeRunThreadPeerLookup ());

    auto globalServer = mStreamFactory->createMessageServerFromStream (Base::move (globalStream));

    waitForTask (mCurrentThreadSentTask, globalServer);

    // We have received a propose message.
    mCurrentThreadSentTask.getProposedClient ().sendMessage (Protocol::MessagesGenerator::makeRunThreadPeerAcceptPropose ());

    // Create ProcessPeers{Server,Client} and ThreadServer
    // ProcessPeersServer should send hello to the other ProcessPeersServer.
    auto newProcessPeersServer  = ProcessPeersServer{Base::move (globalServer)};
    auto newThreadOwnerServer   = ThreadServer{mStreamFactory->createMessageServer ()};

    // How do the thread client know what the thread's port?

    return {};
}

} // namespace Client
} // namespace Ziqe
