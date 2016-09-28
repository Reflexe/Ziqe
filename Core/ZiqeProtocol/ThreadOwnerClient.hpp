/**
 * @file ThreadOwnerClient.hpp
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
#ifndef ZIQE_THREADOWNERCLIENT_H
#define ZIQE_THREADOWNERCLIENT_H

#include "Base/Memory.hpp"
#include "Base/LocalThread.hpp"

#include "Network/NetworkProtocol.hpp"

#include "Core/ZiqeProtocol/ProcessPeersServer.hpp"
#include "Core/ZiqeProtocol/MemoryRevision.hpp"

namespace Ziqe {

// Local Allocations: Allowed, but the page should be registred by the
// ProcessOwner first. The method: GetAndAllocateNewPage.
class ThreadOwnerClient : private Base::InputStreamInterface::Callback
{
public:
    ThreadOwnerClient(Base::UniquePointer<Net::NetworkProtocol> &&ioStream);

    ZqRegisterType doSystemCall(ZqSystemCallIDType id,
                                const Base::RawArray<ZqRegisterType> parameters,
                                MemoryRevision &revision);

    ZqUserAddress getAndReserveMemory (SizeType bytesCount);

    void onDataReceived (const InputDataType &data) override;

private:
    struct Task {
        enum class Type {
            None,
            DoSystemCall,
            GetAndReserveMemory
        };

        Task(Type type)
            : mTaskType{type}
        {
        }

        const Type mTaskType;
        bool isComplete = false;
    };

    struct DoSystemCallTask : Task {
        DoSystemCallTask()
            : Task{Type::DoSystemCall}
        {}

        ZqRegisterType result;
        MemoryRevision newRevision;
    };

    struct GetAndReserveMemoryTask : Task {
        GetAndReserveMemoryTask()
            : Task{Type::GetAndReserveMemory}
        {}

        ZqUserAddress address;
    };

    // Receivers


    void sendThreadOwnerMessage (const Base::Vector<uint8_t> &vector)
    { // TODO
        mNetworkProtocol->sendData (Base::copy (vector));
    }

    void waitUntilTaskComplete (const Task &task) {
        mCurrentTaskType = task.mTaskType;

        do {
            mNetworkProtocol->callbackReceiveData (*this);
        } while (task.isComplete == false);
    }

    Task::Type mCurrentTaskType = Task::Type::None;

    // Instead of allocating this two classes, just allocate it once.
    DoSystemCallTask mSystemCallTask;
    GetAndReserveMemoryTask mGetAndReserveMemoryTask;

    Base::UniquePointer<Net::NetworkProtocol> mNetworkProtocol;
};

} // namespace Ziqe

#endif // ZIQE_THREADOWNERCLIENT_H
