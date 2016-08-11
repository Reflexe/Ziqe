/**
 * @file ProcessPeersWorker.h
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
#ifndef ZIQE_PROCESSPEERSWORKER_H
#define ZIQE_PROCESSPEERSWORKER_H

#include "Base/LocalProcess.h"
#include "Base/RWLock.h"
#include "Base/Mutex.h"

#include "Core/ZiqeProtocol/ProcessPeers.h"
#include "Core/ZiqeProtocol/MessagesGenerator.h"
#include "Core/ZiqeProtocol/ThreadState.h"

#include "Core/GlobalProcess.h"
#include "Core/GlobalThreadManager.h"

namespace Ziqe {

/// Every process peer should know about which connaction owns which threadID.
/// Every process peer should know about all of his process' other peers.
/// GlobalThreadID are the Process OWner's Local Thread IDs.
class ProcessPeersWorker final : private ProcessPeers::Callback
{
public:
    ProcessPeersWorker();

    // Operations

    /**
     * @brief getMemory  Send all the process peer a get memory request and
     *                   receive the result.
     * @param address
     * @param size
     * @return
     */
    SharedVector<Byte> getMemory (ZqAddress address,
                                  SizeType size);

    void writeMemory (const SharedVector<Byte> &data, ZqAddress address);

    // NOT Broadcast
    void stopThread (GlobalThreadID threadID);
    void continueThread (GlobalThreadID threadID);

    void killThread (GlobalThreadID threadID);
    UniquePointer<GlobalThread> runThread (LocalThread &localThread);

private:
    class ProcessPeersTasks
    {
    public:
        typedef MessagesGenerator::IdentiferType TaskID;

        /// A request that should get response.
        struct Task {
            Task() = default;
            ALLOW_COPY_AND_MOVE (Task)
            virtual ~Task() = 0;

            enum class TaskType {
                GetMemory,
                RunThread
            };

            LocalThread requestThread;
        };

        ProcessPeersTasks()
        // TODO: set mNextTaskID to a random number.
            : mNextTaskID{}
        {
        }

        TaskID addNewTask(UniquePointer<Task> &&task) {
            Mutex::ScopedLock lock{mTasksTableLock};

            mIdentiferToTask.insert (generateNewID (),
                                     std::move (task));
        }

        // Can return null.
        UniquePointer<Task> getAndRemoveTask(TaskID taskID) {
            Mutex::ScopedLock lock{mTasksTableLock};

            // mIdentiferToTask.end().data is nullptr, we'll it
            // to the caller's variable and it will set .data to
            // nullptr again.
            return mIdentiferToTask.find (taskID).takePointer ();
        }
// TODO: split the process peer server and client.
//       The client
    private:
        /// Called only when this class is locked.
        TaskID generateNewID ()
        {
            return mNextTaskID++;
        }

        typedef HashTable<TaskID, Task>          TasksTable;

        mutable Mutex mTasksTableLock;

        TaskID mNextTaskID;
        TasksTable mIdentiferToTask;
    };

    LocalProcess mProcess;

    SharedPointer<GlobalThreadManager> mGlobalThreadManager;

    SharedPointer<NetworkProtocolPool> mProtocolPool;

    /// "Global" remote threads in the same process.
    /// Access Rules: Lock: mConnectionsTableLock; When: Result Usage, Read, Write.
    ///
    mutable RWLock mConnectionsTableLock;
    HashTable<GlobalThreadID, NetworkProtocolPool::ProtocolsType::Iterator> mThreadIDToConnection;

    /// "Global" local threads in the same process.
    /// Access Rules: Lock: mLocalThreadsTableLock; When: Read, Write.
    mutable RWLock mLocalThreadsTableLock;
    HashTable<GlobalThreadID, LocalThread> mProcessLocalThreads;

    ProcessPeersTasks mTasks;

    bool getLocalThreadByGlobalThreadID(GlobalThreadID thread, LocalThread &localThread);
    bool getConnectionByThreadID (GlobalThreadID thread,
                                  NetworkProtocolPool::ProtocolsType::Iterator &connection);

    /// Server.
    void onContiniueThread (GlobalThreadID threadID) override;
    void onStopThreadReceived (GlobalThreadID threadID) override;
    void onKillThreadReceived (GlobalThreadID threadID) override;
    void onGetMemoryReceived (const SharedPointer<NetworkProtocol> &protocol,
                              ZqAddress address, SizeType length) override;
    void onGivePageReceived (ZqAddress page, const SharedVector<Byte> &page) override;
    void onRunThreadReceived (GlobalThreadID newThreadID, MemoryMap &currentMemoryMap,
                              ThreadState &state) override;
    void onWriteMemoryReceived (ZqAddress address, const SharedVector<Byte> &memory) override;

    /// Client.
    void onGetMemoryResultReceived (MessagesGenerator::IdentiferType id, const SharedVector<Byte> &memory) override;
    void onGoodbyeReceived (UglyArray<GlobalThreadID> threads) override;
    void onRunThreadOKReceived () override;
};

} // namespace Ziqe

#endif // ZIQE_PROCESSPEERSWORKER_H
