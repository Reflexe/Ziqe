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

/**
 * @brief The ZiqeProtocol class
 *
 * Abstract: the Ziqe Protocol is used in order to create Ziqe::Task -s.
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
 *   will read them in the right order.
 *
 *   Each message has two parts: Header and data (data is not required).
 *   The reader should be able to read all the messages' headers without
 *   parsing their data.
 *
 * - Data Commpressing:
 *   ?, zip?, maybe something faster? optional?
 *
 *   I thinks that it should be optional and get used only
 *   for a big chunks of data.
 *   In case of memory, a lot of bytes may be initilized to zero so
 *   it commpressing may be needed.
 *
 *  - Memory Sharing & Sync Events:
 *   Every process instance has an copy of the memory image.
 *   A few threads on the same computer have the same memory image.
 *
 *   On a Sync Event, an updated memory map sent by the caller. If
 *   there's a change, the callee should notify everyone and send them
 *   the new memory map.
 *
 *   What happend when a someone sending a sync event and another
 *   sync event get processed before? Its memory map get invalidated.
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
 */

class ZiqeProtocol
{
public:
  ZiqeProtocol();
};

#endif // ZIQEPROTOCOL_H
