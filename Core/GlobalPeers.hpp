/**
 * @file GlobalPeers.hpp
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
#ifndef ZIQE_GLOBALPEERS_H
#define ZIQE_GLOBALPEERS_H

#include "Base/LocalThread.hpp"
#include "Base/Optional.hpp"

#include "Core/GlobalThread.hpp"
#include "Core/GlobalProcess.hpp"
#include "Core/MessageStreamFactoryInterface.hpp"

#include "Protocol/MessageStream.hpp"
#include "Protocol/MessagesGenerator.hpp"
#include "Protocol/MessageServer.hpp"

namespace Ziqe {

/**
   @brief The GlobalPeers class  A Global manager for other peers lookup and
                                 creating new threads.
 */
class GlobalPeers
{
public:
    GlobalPeers(Base::UniquePointer<MessageStreamFactoryInterface> &&streamFactory);

    enum class RunThreadError {
        Other
    };

    /**
       @brief runThread     Try to run a thread on other computer.
       @param localThread   The local thread to run.
       @param globalProcess The GlobalProcess for this @a localThread 's
                            process, nullptr if this process doesn't has
                            a GlobalProcess.

       @return The new Thread or null on failure.

       @todo Thread safety and locks.

       This function will lookup for other machines on the network
       that running Ziqe. If there are, it will choose one of them
       to run this thread.

     */
    Base::Expected<GlobalThread, RunThreadError> runThread(const LocalThread &localThread,
                                                           const Base::SharedPointer<GlobalProcess> &globalProcess = {});

private:
    void runThreadRequestWorker (Protocol::MessageStream &stream);

    /**
       @brief onMessageReceived
       @param type
       @param fieldReader
       @param messageStream
     */
    void onMessageReceived (const Protocol::Message &type,
                            Protocol::MessageStream::MessageFieldReader &fieldReader,
                            Protocol::MessageStream &&messageStream);

    struct Task {
        enum class Type {
            RunThreadSent
        };

        Task(Type type)
            : mType{type}, mIsComplete{false}
        {
        }

        bool isComplete() const { return mIsComplete; }

        void setComplete ()
        {
            mIsComplete = true;
        }

    private:
        Type mType;
        bool mIsComplete;
    };

    struct RunThreadSentTask : Task {
        RunThreadSentTask()
            : Task{Task::Type::RunThreadSent}
        {
        }

        void setProposedClient (Protocol::MessageStream &&stream)
        {
            mMaybeProposedClient.construct (Base::move (stream));
            setComplete ();
        }

        Protocol::MessageStream &getProposedClient() {
            DEBUG_CHECK (mMaybeProposedClient.isValid ());

            return *mMaybeProposedClient;
        }

    private:
        Base::Optional<Protocol::MessageStream> mMaybeProposedClient;
    };

    void waitForCurrentTask (Task &task, Protocol::MessageServer &server) {
        while (! task.isComplete ()) {
            auto client = server.acceptClient ();

            auto maybeMessage = client.receiveMessage ();
            if (! maybeMessage)
                continue;

            onMessageReceived (Base::move (maybeMessage->first), maybeMessage->second, Base::move (client));
        }
    }

    RunThreadSentTask mCurrentThreadSentTask;

    Base::UniquePointer<MessageStreamFactoryInterface> mStreamFactory;
};

} // namespace Ziqe

#endif // ZIQE_GLOBALPEERS_H
