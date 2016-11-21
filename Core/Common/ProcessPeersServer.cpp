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

void ProcessPeersServer::onMessageReceived(const Protocol::Message &type,
                                           Protocol::MessageStream::MessageFieldReader &fieldReader,
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

void ProcessPeersServer::onStopThreadReceived(Protocol::MessageStream &stream, HostedThreadID threadID) {
    auto thread = globalToLocalThread (threadID);

    if (! thread ) {
        ZQ_ASSERT_REPORT (thread, "Invalid Thread ID");
        stream.sendMessage (Protocol::StopThreadOKMessage{threadID});
    }

    thread->stop ();
    stream.sendMessage (Protocol::StopThreadOKMessage{threadID});
}

void ProcessPeersServer::onContinueThread(Protocol::MessageStream &stream, HostedThreadID threadID) {
    auto thread = globalToLocalThread (threadID);

    if (! thread ) {
        ZQ_ASSERT_REPORT (thread, "Invalid Thread ID");
        stream.sendMessage (Protocol::ContiniueThreadOKMessage{threadID});
    }

    thread->cont ();
    stream.sendMessage (Protocol::ContiniueThreadOKMessage{threadID});
}

void ProcessPeersServer::onKillThreadReceived(Protocol::MessageStream &stream, HostedThreadID threadID) {
    auto thread = globalToLocalThread (threadID);

    if (! thread ) {
        ZQ_ASSERT_REPORT (thread, "Invalid Thread ID");
        stream.sendMessage (Protocol::KillThreadOKMessage{threadID});
    }

    thread->kill ();
    stream.sendMessage (Protocol::KillThreadOKMessage{threadID});
}

void ProcessPeersServer::onRunThreadReceived(Protocol::MessageStream &stream, HostedThreadID newThreadID,
                                             Protocol::MemoryMap &currentMemoryMap,
                                             Protocol::ThreadState &state)
{

}

void ProcessPeersServer::onHelloReceived(Protocol::MessageStream &stream, const Base::RawArray<HostedThreadID> &newThreads)
{
    mOtherServers.getWrite ().first.addThreads (newThreads, stream.getInfo ());
}

void ProcessPeersServer::onGoodbyeReceived(Protocol::MessageStream &stream, const Base::RawArray<HostedThreadID> &leavingThreads)
{
    mOtherServers.getWrite ().first.removeThreads (leavingThreads);
}

void ProcessPeersServer::sendHello()
{
   sendToAll (Protocol::ProcessPeerHelloMessage {getProcessThreadIDs ()});
}

void ProcessPeersServer::sendGoodbye()
{
    sendToAll (Protocol::ProcessPeerGoodbyeMessage {getProcessThreadIDs ()});
}

} // namespace Ziqe
