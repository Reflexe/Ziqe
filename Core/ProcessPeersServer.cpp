/**
 * @file ProcessPeersServer.cpp
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
#include "ProcessPeersServer.hpp"

#include "Protocol/MessagesGenerator.hpp"

namespace Ziqe {

ProcessPeersServer::ProcessPeersServer(Protocol::MessageServer &&messageServer)
    : mServer{Base::move (messageServer)}
{
    sendHello ();
}

ProcessPeersServer::~ProcessPeersServer()
{
    sendGoodbye ();
}

ProcessPeersServer ProcessPeersServer::CreateWithPeer(Protocol::MessageServer &&server,
                                                      Protocol::MessageStream &&messageStream,
                                                      GlobalThreadID peerThreadID)
{
    ProcessPeersServer server{Base::move (server)};
    server.mOtherServers.getWrite ().first.addThreads (Base::RawArray{&peerThreadID, 1}, Base::move (messageStream));

    return server;
}

void ProcessPeersServer::onMessageReceived(const Protocol::Message &type,
                                           Protocol::MessageStream::Callback::MessageFieldReader &fieldReader,
                                           const Protocol::MessageStream &messageStream)
{
    using Protocol::Message;

    switch (type.getType ()) {
    case Message::Type::StopThread:
        break;
    case Message::Type::ContinueThread:
        break;
    case Message::Type::KillThread:
        break;
    case Message::Type::ProcessPeerRunThread:
        break;
    case Message::Type::ProcessPeerHello:
        break;
    case Message::Type::ProcessPeerGoodbye:
        break;
    default:
        return;
    }
}

void ProcessPeersServer::onStopThreadReceived(Protocol::MessageStream &stream, GlobalThreadID threadID) {
    auto thread = globalToLocalThread (threadID);

    if (! thread ) {
        DEBUG_CHECK_REPORT (thread, "Invalid Thread ID");
        stream.sendMessage (Protocol::MessagesGenerator::makeStopThreadOK ());
    }

    thread->stop ();
    stream.sendMessage (Protocol::MessagesGenerator::makeStopThreadOK ());
}

void ProcessPeersServer::onContinueThread(Protocol::MessageStream &stream, GlobalThreadID threadID) {
    auto thread = globalToLocalThread (threadID);

    if (! thread ) {
        DEBUG_CHECK_REPORT (thread, "Invalid Thread ID");
        stream.sendMessage (Protocol::MessagesGenerator::makeContinueThreadOK ());
    }

    thread->cont ();
    stream.sendMessage (Protocol::MessagesGenerator::makeContinueThreadOK ());
}

void ProcessPeersServer::onKillThreadReceived(Protocol::MessageStream &stream, GlobalThreadID threadID) {
    auto thread = globalToLocalThread (threadID);

    if (! thread ) {
        DEBUG_CHECK_REPORT (thread, "Invalid Thread ID");
        stream.sendMessage (Protocol::MessagesGenerator::makeKillThreadOK ());
    }

    thread->kill ();
    stream.sendMessage (Protocol::MessagesGenerator::makeKillThreadOK ());
}

void ProcessPeersServer::onRunThreadReceived(Protocol::MessageStream &stream, GlobalThreadID newThreadID,
                                             Protocol::MemoryMap &currentMemoryMap,
                                             Protocol::ThreadState &state)
{

}

void ProcessPeersServer::onHelloReceived(Protocol::MessageStream &stream, const Base::RawArray<GlobalThreadID> &newThreads)
{
    mOtherServers.getWrite ().first.addThreads (newThreads, stream);
}

void ProcessPeersServer::onGoodbyeReceived(Protocol::MessageStream &stream, const Base::RawArray<GlobalThreadID> &leavingThreads)
{
    mOtherServers.getWrite ().first.removeThreads (leavingThreads);
}

void ProcessPeersServer::sendHello()
{
    mOtherServers.getRead ().first.sendMessageToProcessPeers (Protocol::MessagesGenerator::makeProcessPeerHello (getProcessThreadIDs ().toRawArray ()));
}

void ProcessPeersServer::sendGoodbye()
{
    mOtherServers.getRead ().first.sendMessageToProcessPeers (Protocol::MessagesGenerator::makeProcessPeerGoodbye (getProcessThreadIDs ().toRawArray ()));
}

} // namespace Ziqe
