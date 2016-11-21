/**
 * @file ProcessPeersClient.hpp
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
#ifndef ZIQE_PROCESSPEERSCLIENT_H
#define ZIQE_PROCESSPEERSCLIENT_H

#include "Base/Memory.hpp"
#include "Base/RWLock.hpp"
#include "Base/LocalThread.hpp"

#include "Common/ProcessPeersServer.hpp"
#include "Common/MessageStreamFactoryInterface.hpp"

#include "Protocol/MemoryRevision.hpp"

#include "Protocol/MessageStream.hpp"

namespace Ziqe {

/**
   @brief A client for ProcessPeersServer.

   When a task start, a stream created and a message being sent to
   the other Process Peers Server. Then, we'll wait for the task to
   end.
 */
class ProcessPeersClient
{
public:
    ProcessPeersClient(ProcessPeersServer &localServer,
                       Base::UniquePointer<MessageStreamFactoryInterface> &&factory);
    ~ProcessPeersClient();

    ZQ_ALLOW_COPY_AND_MOVE (ProcessPeersClient)

    /**
     * @brief Continue a remote thread [blocking]
     * @param threadID
     *
     * This method send a continue thread message wait until an 'continued'
     * method received.
     */
    void continueRemoteThread (HostedThreadID threadID);

    /**
     * @brief Stop a remote thread [blocking]
     * @param threadID
     */
    void stopRemoteThread(HostedThreadID threadID);

    /**
     * @brief Kill a remote thread [blocking]
     * @param threadID
     */
    void killRemoteThread(HostedThreadID threadID);

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

    void waitUntilCurrentTaskComplete (Protocol::MessageStream &messageStream) {
        do {
            auto message = messageStream.receiveMessage ();
            if (! message)
                continue;

            onMessageReceived (message->first, message->second);
        } while (! mCurrentTask.isComplete ());
    }

    void onMessageReceived (const Protocol::Message::Type &type,
                            Protocol::MessageStream::MessageFieldReader &fieldReader);

    void onRunThreadOKReceived ();

    /**
     * @brief sendThreadMessage  Send a message to a specific process instance that holds a thread.
     * @param threadID
     * @param vector
     */
    bool sendThreadMessageAndWait (HostedThreadID threadID, Protocol::MessageStream::OutputDataType &&data) {
        auto maybeStreamInfo = mConnections->getRead ().first.findThreadStreamInfo (threadID);
        if (! maybeStreamInfo)
            return false;

        auto newStream = mStreamFactory->createMessageStream (maybeStreamInfo->first, maybeStreamInfo->second);
        newStream.sendMessage (Base::move (data));

        waitUntilCurrentTaskComplete (newStream);

        return true;
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

    Base::UniquePointer<MessageStreamFactoryInterface> mStreamFactory;
};

} // namespace Ziqe

#endif // ZIQE_PROCESSPEERSCLIENT_H
