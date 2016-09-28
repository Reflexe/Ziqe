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

#include "MessagesGenerator.hpp"

namespace Ziqe {

ProcessPeersServer::ProcessPeersServer()
    : mOtherServers{mLock}
{
    sendHello ();
}

ProcessPeersServer::~ProcessPeersServer()
{
    sendGoodbye ();
}

void ProcessPeersServer::onStopThreadReceived(GlobalThreadID threadID) {
    auto thread = globalToLocalThread (threadID);

    if (! thread ) {
        DEBUG_CHECK_REPORT (thread);
        replyToLastPacket (MessagesGenerator::makeStopThreadOK ());
    }

    thread->stop ();
    replyToLastPacket (MessagesGenerator::makeStopThreadOK ());
}

void ProcessPeersServer::onContinueThread(GlobalThreadID threadID) {
    auto thread = globalToLocalThread (threadID);

    if (! thread ) {
        DEBUG_CHECK_REPORT (thread);
        replyToLastPacket (MessagesGenerator::makeContinueThreadOK ());
    }

    thread->cont ();
    replyToLastPacket (MessagesGenerator::makeContinueThreadOK ());
}

void ProcessPeersServer::onKillThreadReceived(GlobalThreadID threadID) {
    auto thread = globalToLocalThread (threadID);

    if (! thread ) {
        DEBUG_CHECK_REPORT (thread);
        replyToLastPacket (MessagesGenerator::makeKillThreadOK ());
    }

    thread->kill ();
    replyToLastPacket (MessagesGenerator::makeKillThreadOK ());
}

void ProcessPeersServer::onRunThreadReceived(GlobalThreadID newThreadID,
                                             MemoryMap &currentMemoryMap,
                                             ThreadState &state)
{

}

void ProcessPeersServer::onHelloReceived(const Base::RawArray<GlobalThreadID> &newThreads)
{

}

void ProcessPeersServer::onGoodbyeReceived(const Base::RawArray<GlobalThreadID> &leavingThreads)
{
    mOtherServers.getWrite ().first->removeThreads (leavingThreads);
}

void ProcessPeersServer::sendHello()
{
    mOtherServers.getRead ().first->sendMessageToProcessPeers (MessagesGenerator::makeProcessPeerHello (getProcessThreadIDs ().toRawArray ()));
}

void ProcessPeersServer::sendGoodbye()
{
    mOtherServers.getRead ().first->sendMessageToProcessPeers (MessagesGenerator::makeProcessPeerGoodbye (getProcessThreadIDs ().toRawArray ()));
}

} // namespace Ziqe
