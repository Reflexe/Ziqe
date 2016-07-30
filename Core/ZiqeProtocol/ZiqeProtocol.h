/**
 * @file %{FILENAME}
 * @author shrek0 (shrek0.tk@gmail.com)
 * @section LICENSE
 *
 * ProtocolLearn copyright (C) %YEAR% shrek0
 *
 * ProtocolLearn is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ProtocolLearn is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 */
#ifndef ZIQEPROTOCOL_H
#define ZIQEPROTOCOL_H

#include "Base/InputStreamInterface.h"
#include "Base/OutputStreamInterface.h"

#include "Base/Memory.h"
#include "Base/FieldsReader.h"

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

namespace Ziqe {

class ZiqeProtocol
{
public:
    struct Message {
        uint16_t messageType;

        uint64_t processID;
    };

    ZiqeProtocol(UniquePointer<InputStreamInterface> &globaInputStream,
                 UniquePointer<OutputStreamInterface> &globalOutputStream);

    Signal<void(UniquePointer<Peer>&&)> sNewPeers;
};

}

#endif // ZIQEPROTOCOL_H
