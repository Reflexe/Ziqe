/**
 * @file Message.hpp
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
#ifndef ZIQE_MESSAGE_H
#define ZIQE_MESSAGE_H

#include "Base/Types.hpp"

namespace Ziqe {

/**
 * @brief The ZiqeProtocol
 *
 * Abstract: the Ziqe Protocol is used in order to create Ziqe::Task s.
 *
 * - Terms:
 *   "Page Owner"   : The owner of a memory page, responsible for notifing
 *                    all of the users on a Sync Event if this page updated.
 *
 *   "Process Owner": The owner of a process, responsible for managing
 *                    System Calls and the default owner of all of the
 *                    program's memory pages.
 *
 *   "Sync Event"   : An event which the process owner should update its
 *                    memory to a new map.
 *
 * - Messages:
 *   TODO . Sill not stable.
 *
 *  - System Calls:
 *  System call sent as an request message that block the caler
 *  until there's an answer. The system call itself runs by the
 *  real thread in the process owner machine.
 *
 *  System call is a sync event. See below for more info.
 *
 *  - Memory Map & Dynamic Allocation:
 *  Every process instance starts with the constant part of the
 *  memory map. Then, when it want to allocate memory, it asks
 *  the thread owner server for an allocation. The server will
 *  reply with a address and the caler can now map a new memory
 *  to this address (This progress made to avoid address duplication
 *  between two or more process instances).
 *
 *  - Memory Revisions
 *  A memory revision is a state of the memory map. It represented by
 *  the differences between itself and an older revision (similar to
 *  a git commit patch).
 *  We're using Memory Revisions when we want to merge two remote process
 *  instance's memories together. If we'll copy it all and always
 *  use the newest memory map we know, we'll end up losing changes if two
 *  instances changed two differet values in the memory: we'll get only the
 *  lastest one's changes and lose the previous one's changes (Although the
 *  last's changes are completly unrelated to the previous one's changes).
 *
 *  - Sync Events:
 *  Sync event is Process Instance event that creates a new Memory
 *  Revision that get recorded by the Process Owner. In other words,
 *  Sync Event is an event that synchronizing between two Process Instances.
 *
 *  On a sync event, the caler should provide its current memory revision
 *  and its previous revision. Then, the calee should merge this revision
 *  with its previous revision, creating a new revision and then send the
 *  difference between the caller's revision and the new revision back to
 *  the caler.
 *
 * - Defects:
 *  1. Process Instance's Memory not updates on other process's instance
 *     memory changes. That's prevent us from performing atomic operations
 *     as they should operate. We still can not use those operations and
 *     use system call atomic operations that should work.
 */
class Message
{
public:
    enum class Type : uint16_t {
        /// Process Peer Messages: the 2**15 bit is on.
        ///
        /// @brief Get the value of a remote memory. Used to
        ///        be sent to all of the this process' Process Servers.
        GetMemory           = 0x8001,
        GetMemoryResult     = 0x8005,
        /// @brief Write a value to a remote memory.
        WriteMemory         = 0x8010,
        /// @brief Give a memory page to a Process Peer.
        GivePage            = 0x8015,
        /// @brief Tell a peer to stop running a thread.

        StopThread          = 0x8020,
        StopThreadOK        = 0x8021,

        KillThread          = 0x8025,
        KillThreadOK        = 0x8026,

        ContinueThread     = 0x8030,
        ContinueThreadOK   = 0x8031,

        /// @brief Tell all of a process' Process Peers
        ///        that there's a new Process Peer.
        ProcessPeerHello    = 0x8045,
        /// @brief The same as above but goodbye.
        ProcessPeerGoodbye  = 0x8050,
        /// @brief Tell a process peer to run a thread.
        ProcessPeerRunThread              = 0x8055,
        /// @brief "Ok, I'm running your dammn thread! "
        ProcessPeerRunThreadOK,

        GetAndReserveMemory,

        /// Thread Owner P2P: The 2**14 bit is on.
        /// @brief Tell a Process Owner Peer to run a system call
        ///        as a specific thread.
        DoSystemCall        = 0x4000,
        /// @brief Tell a peer that his DoSystemCall request
        ///        has been done and send him the results (changed memory
        ///        and the function return value).
        SystemCallResult    = 0x4001,

        /// Global Messages: The 2**13 bit is on.
        /// @brief Look for a peer to run a thread.
        RunThreadPeerLookup               = 0x2000,
        RunThreadPeerLookupPropose        = 0x2001,
        RunThreadPeerLookupAcceptPropose  = 0x2002,
    };

    typedef Type MessageType;

    Message(MessageType type);

    MessageType getType() const
    {
        return mMessageType;
    }

    bool isProcessInstanceServerMessage () const;
    bool isProcessInstanceClientMessage () const;

    bool isThreadOwnerServerMessage () const;
    bool isThreadOwnerClientMessage () const;

    bool isGlobalServerMessage() const;
    bool isGlobalClientMessage() const;

private:
    MessageType mMessageType;
};

} // namespace Ziqe

#endif // ZIQE_MESSAGE_H
