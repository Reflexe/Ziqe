/**
 * @file ProcessPeersClient.hpp
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

#include "Base/Memory.hpp"
#include "Base/RWLock.hpp"
#include "Base/LocalThread.hpp"

#include "Core/ProcessPeersServer.hpp"
#include "Core/Protocol/MemoryRevision.hpp"

#include "Core/Protocol/MessageStream.hpp"

namespace Ziqe {

class ProcessPeersClient : private Protocol::MessageStream::Callback
{
public:
    ProcessPeersClient(ProcessPeersServer &localServer);
    ~ProcessPeersClient();

    ZQ_ALLOW_COPY_AND_MOVE (ProcessPeersClient)

    /**
     * @brief Continue a remote thread [blocking]
     * @param threadID
     *
     * This method send a continue thread message wait until an 'continued'
     * method received.
     */
    void continueRemoteThread (GlobalThreadID threadID);

    /**
     * @brief Stop a remote thread [blocking]
     * @param threadID
     */
    void stopRemoteThread(GlobalThreadID threadID);

    /**
     * @brief Kill a remote thread [blocking]
     * @param threadID
     */
    void killRemoteThread(GlobalThreadID threadID);

    /**
     * @brief Run a new thread [blocking].
     * @param localThread
     * @return
     */
    Base::UniquePointer<ProcessPeersClient> runNewRemoteThread(LocalThread &localThread);

private:
    struct Task {
        enum class Type {
            RunThread,
            StopRemoteThread,
            KillRemoteThread,
            ContinueRemoteThread
        };

        Task() = default;

        Task(Type type)
            : mTaskType{type},
              mIsComplete{false}
        {
        }

        bool isComplete() const
        {
            return mIsComplete;
        }

        Type getType () const
        {
            return mTaskType;
        }

    private:
        Type mTaskType;
        bool mIsComplete;
    };

    void waitUntilCurrentTaskComplete (const Protocol::MessageStream &messageStream) {
        do {
            messageStream.receiveMessage(*this);
        } while (! mCurrentTask.isComplete ());
    }

    void onMessageReceived (const Protocol::Message &type,
                            MessageFieldReader &fieldReader,
                            const Protocol::MessageStream &messageStream);
    void onRunThreadOKReceived ();

    /**
     * @brief sendThreadMessage  Send a message to a specific process instance that holds a thread.
     * @param threadID
     * @param vector
     */
    bool sendThreadMessage (GlobalThreadID threadID, Protocol::MessageStream::OutputDataType &&vector)
    {
        return mConnections->getRead ().first.sendThreadMessage (threadID, Base::move (vector));
    }

    /**
     * @brief mCurrentTask  The current task that being processed.
     */
    Task mCurrentTask;

    /**
     * @brief mConnections  A shared variable between the ProcessPeers{Client,Server}
     *                      that lists all the current peers for this process.
     */
    ProcessPeersServer::ConnectionsType mConnections;
};

} // namespace Ziqe

#endif // ZIQE_PROCESSPEERSCLIENT_H
