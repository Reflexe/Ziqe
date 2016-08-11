/**
 * @file ProcessPeersWorker.cpp
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
#include "ProcessPeersWorker.h"

#include "Base/ProcessManager.h"

#include "Core/ZiqeProtocol/MessagesGenerator.h"

namespace Ziqe {

struct ProcessPeersWorker::GetMemoryTask : ProcessPeers::ProcessPeersTasks::Task {
    SharedVector<Byte> *mLocalVector;
};

ProcessPeersWorker::ProcessPeersWorker()
{

}

SharedVector<Byte> ProcessPeersWorker::getMemory(ZqAddress address, SizeType size)
{
    mTasks.addNewTask (new )
}

void ProcessPeersWorker::writeMemory(const SharedVector<Byte> &data, ZqAddress address)
{
    mProtocolPool->sendData (MessagesGenerator::makeWriteMemory (data,
                                                                 address));
}

void ProcessPeersWorker::stopThread(GlobalThreadID threadID)
{
    NetworkProtocolPool::ProtocolsType::Iterator iterator;

    if (! getConnectionByThreadID (threadID, iterator))
            return;

    iterator->sendData (MessagesGenerator::makeStopThread (threadID));
}

void ProcessPeersWorker::continueThread(GlobalThreadID threadID)
{
    NetworkProtocolPool::ProtocolsType::Iterator iterator;

    if (!getConnectionByThreadID (threadID, iterator))
            return;

    iterator->sendData (MessagesGenerator::makeContinueThread (threadID));
}

void ProcessPeersWorker::killThread(GlobalThreadID threadID)
{
    NetworkProtocolPool::ProtocolsType::Iterator iterator;

    if (!getConnectionByThreadID (threadID, iterator))
            return;

    iterator->sendData (MessagesGenerator::makeKillThread (threadID));
}

UniquePointer<GlobalThread> ProcessPeersWorker::runThread(LocalThread &localThread) {
    localThread.stop ();
    RWLock::ScopedWriteLock lock{mConnectionsTableLock};



    mThreadIDToConnection.insert (localThread.getThreadID (),

                                           localThread)
}

bool ProcessPeersWorker::getLocalThreadByGlobalThreadID(GlobalThreadID thread,
                                                        LocalThread &localThread)
{
    {
        RWLock::ScopedReadLock lock (mLocalThreadsTableLock);

        auto iterator = mProcessLocalThreads.find (thread);
        if (iterator == mProcessLocalThreads.end ()) {
            DEBUG_CHECK_REPORT_NOT_REACHED ();
            return false;
        }

        localThread = *iterator;
    }

    return true;
}

bool ProcessPeersWorker::getConnectionByThreadID(GlobalThreadID thread,
                                                 NetworkProtocolPool::ProtocolsType::Iterator &connection)
{
    {
        RWLock::ScopedReadLock lock (mConnectionsTableLock);

        auto iterator = mThreadIDToConnection.find (threadID);
        if (iterator == mThreadIDToConnection.end ()) {
            DEBUG_CHECK_REPORT_NOT_REACHED ();

            return false;
        }

        connection = *iterator;
    }

    return true;
}

bool ProcessPeersWorker::getTaskByID(ProcessPeersWorker::TaskID id, TasksTable::Iterator &iterator)
{
    {
        RWLock::ScopedReadLock lock (mTasksTableLock);

        iterator = mIdentiferToTask.find (id);
        if (iterator == mIdentiferToTask.end ()) {
            DEBUG_CHECK_REPORT_NOT_REACHED ();

            return false;
        }
    }

    return true;
}

void ProcessPeersWorker::onContiniueThread(GlobalThreadID threadID) {
    LocalThread thread;
    if (! getLocalThreadByGlobalThreadID (threadID, thread))
        return;

    thread.cont ();
}

void ProcessPeersWorker::onStopThreadReceived(GlobalThreadID threadID) {
    LocalThread thread;
    if (! getLocalThreadByGlobalThreadID (threadID, thread))
        return;

    thread.stop ();
}

void ProcessPeersWorker::onKillThreadReceived(GlobalThreadID threadID) {
    LocalThread thread;
    if (! getLocalThreadByGlobalThreadID (threadID, thread))
        return;

    thread.kill ();
}

void ProcessPeersWorker::onGetMemoryReceived(const SharedPointer<NetworkProtocol> &protocol,
                                             ZqAddress address,
                                             SizeType length)
{

}

void ProcessPeersWorker::onGivePageReceived(ZqAddress page,
                                            const SharedVector<Byte> &page)
{

}

void ProcessPeersWorker::onRunThreadReceived(GlobalThreadID newThreadID, MemoryMap &currentMemoryMap, ThreadState &state)
{

}

void ProcessPeersWorker::onWriteMemoryReceived(ZqAddress address,
                                               const SharedVector<Byte> &memory)
{

}

void ProcessPeersWorker::onGetMemoryResultReceived(MessagesGenerator::IdentiferType id,
                                                   const SharedVector<Byte> &memory)
{
    typename TasksTable::Iterator iterator;

    if (! getTaskByID (id, iterator))
        return;

    // Copy the memory and all of that.

    iterator->requestThread.cont ();
}

void ProcessPeersWorker::onGoodbyeReceived(UglyArray<GlobalThreadID> threads)
{
    // Delete this stream.
}

void ProcessPeersWorker::onRunThreadOKReceived()
{

}

} // namespace Ziqe
