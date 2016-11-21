/**
 * @file PeerLookupServer.cpp
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
#include "PeerLookupServer.hpp"

#include "Protocol/MessagesGenerator.hpp"

namespace Ziqe {
namespace Host {

PeerLookupServer::PeerLookupServer(Base::UniquePointer<MessageStreamFactoryInterface> &&factory,
                                   Protocol::MessageStream::Port listenPort)
    : mStreamFactory{Base::move (factory)},
      mServer{mStreamFactory->createMessageServer (listenPort)}
{
}

void PeerLookupServer::run() {
    // TODO: write a Service API (Base::Service)
    while (true) {
        // Wait for new client.
        auto client = mServer.acceptClient ();

        auto message = client.receiveMessage ();
        if (! message)
            return;

        handleMessage (client, message->first, message->second);
    }
}

void PeerLookupServer::handleMessage(const Protocol::MessageStream &clientStream,
                                     const Protocol::Message &message,
                                     const Protocol::MessageStream::MessageFieldReader &reader)
{
    /**
        ## What information a RunThreadPeerLookupAcceptPropose message should contain:
            * The process info:
                * Process Peers info.
                * Process data (memory etc.).
            *
      */

    switch (message.getType ()) {
    case Protocol::Message::Type::RunThreadPeerLookup:
        clientStream.sendMessage (Protocol::MessagesGenerator::makeRunThreadPeerPropose ());
        break;

    case Protocol::Message::Type::RunThreadPeerLookupAcceptPropose: {
        // TODO: request protocol object


//        auto newProcessPeersServer = ProcessPeersServer::getConnections ();
//        auto newProcessPeersClient = ProcessPeersClient{newProcessPeersServer};
//        auto newThreadClient       = ThreadClient{mStreamFactory->createMessageStream ()};


    }
        break;
    default:
        return;
    }
}

} // namespace Host
} // namespace Ziqe
