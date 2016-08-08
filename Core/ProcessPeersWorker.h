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

#include "Core/ZiqeProtocol/ProcessPeers.h"
#include "Core/ZiqeProtocol/MessagesGenerator.h"

#include "Base/LocalProcess.h"
#include "Base/Mutex.h"

namespace Ziqe {

class ProcessPeersWorker final : private ProcessPeers::Callback
{
public:
    ProcessPeersWorker();

    /// Server.
    void onContiniueThread (GlobalThreadID threadID) override;
    void onGetMemoryReceived (const SharedPointer<NetworkProtocol> &protocol);
    void onGivePageReceived (ZqAddress page, const SharedVector<Byte> &page);
    void onKillThreadReceived (GlobalThreadID threadID) override;
    void onRunThreadReceived () override;
    void onStopThreadReceived (GlobalThreadID threadID) override;
    void onWriteMemoryReceived (ZqAddress address, const SharedVector<Byte> &memory);

    /// Client.
    void onGetMemoryResultReceived () override;
    void onGoodbyeReceived () override;
    void onRunThreadProposeReceived () override;

private:
    LocalProcess mProcess;

    HashTable<GlobalThreadID, NetworkProtocolPool::ProtocolsType::Iterator> mThreadIDToConnection;

    /// A request that should get response.
    struct Task {
        Task() = default;
        ALLOW_COPY_AND_MOVE (Task)
        virtual ~Task() = 0;

        enum class TaskType {
            GetMemory,
            RunThread
        };

        virtual TaskType getType () = 0;

        LocalThread requestThread;
    };

    HashTable<uint64_t, Task> mIdentiferToTask;

    mutable Mutex mMutex;
};

} // namespace Ziqe

#endif // ZIQE_PROCESSPEERSWORKER_H
