/**
 * @file Peer.h
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
#ifndef ZIQE_PEER_H
#define ZIQE_PEER_H

#include "Base/OutputStreamInterface.h"

#include "Base/Memory.h"
#include "Base/LinkedList.h"

#include "Base/LocalProcess.h"
#include "Base/LocalThread.h"

#include "Core/GlobalProcess.h"
#include "Core/GlobalThread.h"

namespace Ziqe {

class Peer
{
public:
    Peer();

    /**
     * @brief runThread  Tell a peer to run a thread.
     * @param thread
     *
     * This function will send all of the required data to
     * the this peer and tell it to run this thread. Any future
     * system calls / memory requests will get to us.
     */
    void runThread (const LocalThread &thread,
                    const LocalProcess &process);

    // Stop this thread until this peer give us an answer for this syscall.
    void requestSystemCallForThread (const Thread &thread);

    // make us the owner of this page.
    void takePage (const ProcessMemory::MemoryPage &memoryPage);

    // Make him the owner of this page.
    void givePage (const ProcessMemory::MemoryPage &memoryPage);

private:
    struct PeerInformation
    {
        enum class Arch {
            x86    = 0,
            ARM    = 1,
            x86_64 = 2
        };

        Arch arch;
        // nick?
        enum class Cost{
            _1
        };
    };

    LinkedList<GlobalProcess> mSharedProcesses;

    /// @brief A stream that sent to this peer.
    UniquePointer<OutputStreamInterface> mOutputStream;
};

} // namespace Ziqe

#endif // ZIQE_PEER_H
