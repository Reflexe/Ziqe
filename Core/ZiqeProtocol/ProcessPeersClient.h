/**
 * @file ProcessPeersClient.h
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
#ifndef ZIQE_PROCESSPEERSCLIENT_H
#define ZIQE_PROCESSPEERSCLIENT_H

#include "Base/Memory.h"
#include "Base/RWLock.h"
#include "Base/LocalThread.h"

#include "Network/NetworkProtocol.h"

#include "Core/ZiqeProtocol/ProcessPeersServer.h"

namespace Ziqe {

/// Every global thread run this.
// Every global thread has a port and a list of the currenly connected threads.
// Every process instance also have a port (server).

// Global thread id: the process owner's thread id.
// Local Allocations: Allowed, but the page should be registred by the ProcessOwner first. The method: GetAndAllocateNewPage.
// Get and Write memory are broadcasted.
// The ProcessPeers Server is getting new peers connections meessages.
// ProcessPeersClient is managing runThread requests.
// It's the newly created thread responsibility to notify the ProcessPeers[Servers] and say hello and goodbyte.
//
class ProcessPeersClient : private NetworkProtocol::Callback
{
public:
    ProcessPeersClient();
    ~ProcessPeersClient();

    ALLOW_COPY_AND_MOVE (ProcessPeersClient)

    UniquePointer<ProcessPeersClient> runThread (LocalThread &localThread);

    SharedVector<Byte> getMemory (ZqAddress address,
                                  SizeType size);

    void writeMemory (const SharedVector<Byte> &data, ZqAddress address);

    void killRemoteThread (GlobalThreadID threadID);

    void stopRemoteThread (GlobalThreadID threadID);

    void continiueRemoteThread (GlobalThreadID threadID);

    // Sent to the thread owner.
    ZqRegisterType doSystemCall(ZqSystemCallIDType id,
                       const UglyArray<ZqRegisterType> parameters);

    ZqAddress getAndReserveMemory (SizeType pagesCount);

private:
    struct Task {
        enum class Type {
            RunThread,
            DoSystemCall,
            GetMemory,
            GetAndReserveMemory
        };

        Task(Type type)
            : mTaskType{type}
        {
        }

        const Type mTaskType;
        bool isComplete = false;
    };

    struct RunThreadTask : Task {
        RunThreadTask()
            : Task{Type::RunThread}
        {}
    };

    struct GetMemoryTask : Task {
        GetMemoryTask()
            : Task{Type::GetMemory}
        {}

        SharedVector<Byte> *mMemory;
    };

    struct DoSystemCallTask : Task {
        DoSystemCallTask()
            : Task{Type::DoSystemCall}
        {}

        ZqRegisterType result;
        MemoryMap map;
    };

    struct GetAndReserveMemoryTask : Task {
        GetAndReserveMemoryTask()
            : Task{Type::GetAndReserveMemory}
        {}

        ZqAddress address;
    };

    // Say hello all
    void sayHello (/* NetworkPacket::PacketInfo &serverInfo */);
    void sayGoodbye ();

    void sendMessageToProcessPeers (UniquePointer<Vector<Byte>> &&vector) {
        RWLock::ScopedReadLock lock{mConnections->first};
        auto iterator = mConnections->second.begin ();
        auto end      = mConnections->second.end ();

        for (; iterator != end; ++iterator)
        {
            iterator->second->sendData (*vector);
        }
    }

    void sendThreadMessage (GlobalThreadID threadID, UniquePointer<Vector<Byte>> &vector) {
        RWLock::ScopedReadLock lock{mConnections->first};

        auto iterator = mConnections->second.find (threadID);
        if (iterator == mConnections->second.end ())
            return;

        iterator->second->sendData (*vector);
    }

    void sendThreadOwnerMessage (UniquePointer<Vector<Byte>> &vector)
    {
        mThreadOwnerServerStream->sendData (*vector);
    }

    void waitUntilTaskComplete () {
        do {
            mOurPort->callbackReceivePacket (*this);
        } while (mCurrentTask->isComplete == false);
    }

    void onGetMemoryResultReceived (const SharedVector<Byte> &memory);
    void onRunThreadOKReceived ();
    void onSystemCallResultReceived (ZqRegisterType result);

    ProcessPeersServer::ConnectionsType mConnections;

    UniquePointer<Task> mCurrentTask;
    GlobalThreadID mMyGlobalThreadID;

    UniquePointer<OutputStreamInterface> mThreadOwnerServerStream;

    UniquePointer<NetworkProtocol> mOurPort;
};

} // namespace Ziqe

#endif // ZIQE_PROCESSPEERSCLIENT_H
