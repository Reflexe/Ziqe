/**
 * @file ProcessPeersClient.cpp
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
#include "ProcessPeersClient.hpp"

#include "MessagesGenerator.hpp"

namespace Ziqe {

ProcessPeersClient::ProcessPeersClient(ProcessPeersServer &localServer)
    : mConnections{localServer.getConnections ()}
{
}

ProcessPeersClient::~ProcessPeersClient()
{
}

void ProcessPeersClient::continueRemoteThread(GlobalThreadID threadID) {
    if (! sendThreadMessage (threadID, MessagesGenerator::makeContinueThread (threadID)))
        return;

    mCurrentTask = Task{Task::Type::ContinueRemoteThread};
    waitUntilCurrentTaskComplete ();
}

Base::UniquePointer<ProcessPeersClient> ProcessPeersClient::runNewRemoteThread(LocalThread &localThread) {
// TODO: needs memory.
}

void ProcessPeersClient::killRemoteThread(GlobalThreadID threadID) {
    if (! sendThreadMessage (threadID, MessagesGenerator::makeKillThread (threadID)))
        return;

    mCurrentTask = Task{Task::Type::KillRemoteThread};
    waitUntilCurrentTaskComplete ();
}

void ProcessPeersClient::stopRemoteThread(GlobalThreadID threadID) {
    if (! sendThreadMessage (threadID, MessagesGenerator::makeStopThread (threadID)))
        return;

    mCurrentTask = Task{Task::Type::StopRemoteThread};
    waitUntilCurrentTaskComplete ();
}

} // namespace Ziqe
