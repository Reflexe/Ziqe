/**
 * @file ThreadOwnerClient.hpp
 * @author Shmuel Hazan (shmuelhazan0@gmail.com)
 *
 * Ziqe: copyright (C) 2016 Shmuel Hazan
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

#include "Core/Protocol/MessageStream.hpp"

#include "Core/ProcessPeersServer.hpp"
#include "Core/Protocol/MemoryRevision.hpp"

namespace Ziqe {

// Local Allocations: Allowed, but the page should be registred by the
// ProcessOwner first. The method: GetAndAllocateNewPage.
class ThreadOwnerClient final
{
public:
    ThreadOwnerClient(Base::UniquePointer<Protocol::MessageStream> &&stream);

    ZqRegisterType doSystemCall(ZqSystemCallIDType id,
                                const Base::RawArray<ZqRegisterType> parameters,
                                Protocol::MemoryRevision &revision);

    ZqUserAddress getAndReserveMemory (SizeType bytesCount);

private:
    void onMessageReceived (const Protocol::Message &type,
                            Protocol::MessageStream::MessageFieldReader &fieldReader,
                            const Protocol::MessageStream &messageStream);

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
        Protocol::MemoryRevision newRevision;
    };

    struct GetAndReserveMemoryTask : Task {
        GetAndReserveMemoryTask()
            : Task{Type::GetAndReserveMemory}
        {}

        ZqUserAddress address;
    };

    // Receivers
    void sendThreadOwnerMessage (const Protocol::MessageStream::OutputDataType &vector)
    {
        mThreadOwnerStream->sendMessage (vector);
    }

    void waitUntilTaskComplete (const Task &task) {
        mCurrentTaskType = task.mTaskType;

        do {
            mThreadOwnerStream->receiveMessage ();
        } while (task.isComplete == false);
    }

    Task::Type mCurrentTaskType = Task::Type::None;

    // Instead of allocating this two classes, just allocate it once.
    DoSystemCallTask mSystemCallTask;
    GetAndReserveMemoryTask mGetAndReserveMemoryTask;

    Base::UniquePointer<Protocol::MessageStream> mThreadOwnerStream;
};

} // namespace Ziqe

#endif // ZIQE_THREADOWNERCLIENT_H
