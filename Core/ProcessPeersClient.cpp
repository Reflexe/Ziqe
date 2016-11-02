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

#include "Protocol/MessagesGenerator.hpp"

namespace Ziqe {

ProcessPeersClient::ProcessPeersClient(ProcessPeersServer &localServer)
    : mConnections{localServer.getConnections ()}
{
}

ProcessPeersClient::~ProcessPeersClient()
{
}

void ProcessPeersClient::continueRemoteThread(GlobalThreadID threadID) {
    if (! sendThreadMessage (threadID, Protocol::MessagesGenerator::makeContinueThread (threadID)))
        return;

    mCurrentTask = Task{Task::Type::ContinueRemoteThread};
    waitUntilCurrentTaskComplete ();
}

Base::UniquePointer<ProcessPeersClient> ProcessPeersClient::runNewRemoteThread(LocalThread &localThread) {
    // TODO: needs memory.
}

void ProcessPeersClient::onMessageReceived(const Protocol::Message &type, Protocol::MessageStream::MessageFieldReader &fieldReader)
{

}

void ProcessPeersClient::killRemoteThread(GlobalThreadID threadID) {
    if (! sendThreadMessage (threadID, Protocol::MessagesGenerator::makeKillThread (threadID)))
        return;

    mCurrentTask = Task{Task::Type::KillRemoteThread};
    waitUntilCurrentTaskComplete ();
}

void ProcessPeersClient::stopRemoteThread(GlobalThreadID threadID) {
    if (! sendThreadMessage (threadID, Protocol::MessagesGenerator::makeStopThread (threadID)))
        return;

    mCurrentTask = Task{Task::Type::StopRemoteThread};
    waitUntilCurrentTaskComplete ();
}

} // namespace Ziqe
