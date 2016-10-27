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
#include "ProcessPeersServer.hpp"

#include "Protocol/MessagesGenerator.hpp"

namespace Ziqe {

ProcessPeersServer::ProcessPeersServer()
{
    sendHello ();
}

ProcessPeersServer::~ProcessPeersServer()
{
    sendGoodbye ();
}

Base::UniquePointer<ProcessPeersServer> ProcessPeersServer::createNewProcessInstance() {
    Net::Stream stream;

    stream.send (MessagesGenerator::makeRunThreadPeerLookup ());
}

void ProcessPeersServer::onMessageReceived(const Protocol::Message &type,
                                           Protocol::MessageStream::Callback::MessageFieldReader &fieldReader,
                                           const Net::Stream &stream)
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
        DEBUG_CHECK_REPORT (thread);
        stream.sendMessage (MessagesGenerator::makeStopThreadOK ());
    }

    thread->stop ();
    stream.sendMessage (MessagesGenerator::makeStopThreadOK ());
}

void ProcessPeersServer::onContinueThread(Protocol::MessageStream &stream, GlobalThreadID threadID) {
    auto thread = globalToLocalThread (threadID);

    if (! thread ) {
        DEBUG_CHECK_REPORT (thread);
        stream.sendMessage (MessagesGenerator::makeContinueThreadOK ());
    }

    thread->cont ();
    stream.sendMessage (MessagesGenerator::makeContinueThreadOK ());
}

void ProcessPeersServer::onKillThreadReceived(Protocol::MessageStream &stream, GlobalThreadID threadID) {
    auto thread = globalToLocalThread (threadID);

    if (! thread ) {
        DEBUG_CHECK_REPORT (thread);
        stream.sendMessage (MessagesGenerator::makeKillThreadOK ());
    }

    thread->kill ();
    stream.sendMessage (MessagesGenerator::makeKillThreadOK ());
}

void ProcessPeersServer::onRunThreadReceived(Protocol::MessageStream &stream, GlobalThreadID newThreadID,
                                             Protocol::MemoryMap &currentMemoryMap,
                                             Protocol::ThreadState &state)
{

}

void ProcessPeersServer::onHelloReceived(Protocol::MessageStream &stream, const Base::RawArray<GlobalThreadID> &newThreads)
{
    mOtherServers.getWrite ().first->addThreads (newThreads, stream);
}

void ProcessPeersServer::onGoodbyeReceived(Protocol::MessageStream &stream, const Base::RawArray<GlobalThreadID> &leavingThreads)
{
    mOtherServers.getWrite ().first->removeThreads (leavingThreads);
}

void ProcessPeersServer::sendHello()
{
    mOtherServers.getRead ().first->sendMessageToProcessPeers (Protocol::MessagesGenerator::makeProcessPeerHello (getProcessThreadIDs ().toRawArray ()));
}

void ProcessPeersServer::sendGoodbye()
{
    mOtherServers.getRead ().first->sendMessageToProcessPeers (Protocol::MessagesGenerator::makeProcessPeerGoodbye (getProcessThreadIDs ().toRawArray ()));
}

} // namespace Ziqe