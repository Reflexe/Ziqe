/**
 * @file GlobalProcessManager.cpp
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
#include "GlobalProcessManager.hpp"

namespace Ziqe {

GlobalProcessManager::GlobalProcessManager()
{
}

// TODO: wait for a more stable ProcessPeer API.
#if 0
void GlobalProcessManager::runThread(LocalThread &thread,
                                     const LocalProcess &process,
                                     bool forceNewPeer) {
    DEBUG_CHECK (thread.isStopped ());
    // Make sure this thread isn't already running.
    DEBUG_CHECK (! mThreadManager->localToGlobalThread (thread));

    auto iterator = mLocalToGlobal.find (process.getProcessID ());

//     TODO: handle failures and timeouts.
//     We can look for a new peer if the no shared process is responding after
//     some time.
    if (forceNewPeer || iterator == mProcesses.end ())  {
        auto newGlobalThreadAndProcess = mGlobalPeersClient->runThread (thread);

        mThreadManager->addThread (newGlobalThread.getThreadID (),
                                   std::move (newGlobalThread));

        addNewProcessConnection (iterator,
                                 std::move (newGlobalThreadAndProcess.second));
    } else {
        // If the process is shared with someone already.

        auto newGlobalThread = iterator->second.getClient().runThread (thread);
        mThreadManager->addThread (thread.getThreadID (),
                                   std::move (newGlobalThread));
    }
}
//UglyPointer<GlobalProcess> GlobalProcessManager::localToGlobalProcess(const LocalProcess &process) {
//    auto iterator = mLocalToGlobal.find (process.getProcessID ());

//    return (iterator == mLocalToGlobal.end ()) ?
//                &(*iterator)
//              : nullptr;
//}

//GlobalProcessManager::ProcessTable::Iterator
//GlobalProcessManager::addNewProcessConnection(const ProcessTable::Iterator &processIDLookupResult,
//                                              UniquePointer<NetworkProtocol> &&processConnection) {
//    if (processIDLookupResult == mLocalToGlobal.end ()) {
//        UniquePointer<GlobalProcess> globalProcess = new GlobalProcess{};

//        // Create a new GlobalProcess.
//    } else {
//        // Add a process connection to the previous GlobalProcess.
//    }
//}

#endif

} // namespace Ziqe
