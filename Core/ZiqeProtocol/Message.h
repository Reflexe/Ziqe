/**
 * @file Message.h
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

#include "Base/Types.h"
#include "Base/SharedVector.h"
#include "Base/FieldReader.h"

namespace Ziqe {

/**
 * @brief The ZiqeProtocol class
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
 *   "Sync Event"   : An event when all of the instance of a process should update
 *                    their memory image (System Call).
 *
 * - Messages:
 *   * Memory: Update, take, give.
 *   * System Call.
 *   * Thread: New, Delete.
 *
 *   Messages can be sent as one block; that make sure that the reader
 *   will read them at the right order.
 *
 *   Each message has two parts: Header and data (data can be empty).
 *   The reader should be able to read all the messages' headers without
 *   parsing their data.
 *
 * - Data Commpressing:
 *   ?, zip?, maybe something faster? optional?
 *
 *   I thinks that it should be optional and get used only
 *   for a big chunks of data.
 *   In case of memory, a lot of bytes may be initilized to zero so
 *   commpressing may be needed badly.
 *
 *  - Memory Ownership and Access:
 *   Every memory Page(?) must be own by one process instance.
 *   When a memory page is owned, the owner is allowed to write and read from
 *   it freely. When one process instance want access to a memory page that
 *   owned by someone else, it should request the current state of the memory
 *   page from its owner.
 *
 *   It is possible also to move ownership between two process instances:
 *   if for example A is the owner of a memory page and it see that B
 *   requested the same page multiple times, it can give B the page's
 *   ownership. Also, B can take the page and notify A about it.
 *
 *   Table 1: Memory Areas and Their Initial Owner
 *    ------------------------------------------------------------------
 *   | Area                  | Initial Owner       | Can be given/taken |
 *    ----------------------- -------------------- ---------------------
 *   | Stack                 | The stack's thread  | Yes?               |
 *    -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 *   | Dynamic allocations   | The allocator       | Yes                |
 *    -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 *   | Static variables      | The Process Owner   | Yes (Not always)   |
 *    -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 *   | Constant Areas (e.g.  | No One (always can  | No                 |
 *   | .text)                | be read)            |                    |
 *    -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
 *
 *  Problems:
 *   * Allocated pages may have duplication (they get allocated by the local machine,
 *     so two machines may allocate the same address).
 *
 *  - System Calls:
 *  The basic problem about system calls is that the Process Owner may need
 *  to read a private memory from the callee. It may also required to write
 *  some data into the callee's memory.
 *
 *  There're two ways to solve this problem:
 *   -- Send all of our memory with the request: a lot of overhead.
 *   -- Parse every system call and send only the required pages.
 *
 *  - Memory Sharing & Sync Events:
 *
 *   Every process instance has an copy of the memory image.
 *   A few threads on the same computer have the same memory image.
 *
 *   On a Sync Event, an updated memory map sent by the caller. If
 *   there's a change, the callee should notify everyone and send them
 *   the new memory map.
 *
 *   What happend when a someone sending a sync event and another
 *   sync event of the same process get processed before?
 *   Its memory map get invalidated.
 *   Maybe a revision parameter for every memory map and when a
 *   computer sent an older rev, it'll request a new memory map
 *   from the caller.
 *
 *   Summery: When a Process Owner processes a Sync Event, it validates its
 *    memory map. Then, it applys it to itself and run the Sync Event.
 *    After the sync event, it send a broadcast to all of the users
 *    and notify them about the new data (memory map). And then it
 *    finishes the Sync event and waking up the caller.
 *
 *   Note: A Sync Event can be called locally and remotly.
 *
 * - Memory Map:
 *  Only the changes between the previous sent revision and the current one.
 *
 * - Defects:
 *   - Sync Events: A very big overhead when a new memory map is required.
 *                  Maybe will do a double check to see if we should get an
 *                  new one at all (if we can see what the requester may respond
 *                  in response to our new map request)
 */
class Message
{
public:
    enum class Type : DWord {
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
        KillThread          = 0x8025,
        ContiniueThread     = 0x8030,
        /// @brief Tell all of a process' Process Peers
        ///        that there's a new Process Peer.
        ProcessPeerHello    = 0x8045,
        /// @brief The same as above but goodbye.
        ProcessPeerGoodbye                = 0x8050,
        /// @brief Tell a process peer to run a thread.
        ProcessPeerRunThread              = 0x8055,
        /// @brief "Ok, I'm running your dammn thread! "
        ProcessPeerRunThreadOK,

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
    Message(FieldReader &reader);

    MessageType getType() const
    {
        return mMessageType;
    }

    bool isProcessP2PMessage () const;
    bool isThreadP2PMessage () const;

    bool isGlobalMessage() const;

    static bool isValidReader (const FieldReader &reader)
    {
        return reader.canReadT<DWord>();
    }

private:
    MessageType mMessageType;
};

} // namespace Ziqe

#endif // ZIQE_MESSAGE_H
