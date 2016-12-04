/**
 * @file ProcessPeersClient.cpp
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
#include "ProcessPeersClient.hpp"

namespace Ziqe {

ProcessPeersClient::ProcessPeersClient(ProcessPeersServer &localServer,
                                       Base::UniquePointer<MessageStreamFactoryInterface> &&factory)
    : mConnections{localServer.getConnections ()}, mStreamFactory{Base::move (factory)}
{

}

ProcessPeersClient::~ProcessPeersClient()
{
}

void ProcessPeersClient::continueRemoteThread(HostedThreadID threadID) {
    mCurrentTask = Task{Task::Type::ContinueRemoteThread};
    if (! sendThreadMessageAndWait (threadID, Protocol::ContiniueThreadMessage{threadID})) {
        ZQ_ASSERT_REPORT_NOT_REACHED ("Can't send");
        return;
    }
}

Base::UniquePointer<ProcessPeersClient> ProcessPeersClient::runNewRemoteThread(LocalThread &localThread) {
    // TODO: needs memory.
}

void ProcessPeersClient::onMessageReceived(const Protocol::Message::Type &type, Protocol::MessageStream::MessageFieldReader &fieldReader)
{

}

void ProcessPeersClient::killRemoteThread(HostedThreadID threadID) {
    mCurrentTask = Task{Task::Type::KillRemoteThread};
    if (! sendThreadMessageAndWait (threadID, Protocol::KillThreadMessage{threadID})) {
        ZQ_ASSERT_REPORT_NOT_REACHED ("Can't send");
        return;
    }
}

void ProcessPeersClient::stopRemoteThread(HostedThreadID threadID) {
    mCurrentTask = Task{Task::Type::StopRemoteThread};
    if (! sendThreadMessageAndWait (threadID, Protocol::StopThreadMessage{threadID})) {
        ZQ_ASSERT_REPORT_NOT_REACHED ("Can't send");
        return;
    }
}

} // namespace Ziqe
