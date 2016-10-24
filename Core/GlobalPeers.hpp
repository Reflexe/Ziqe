/**
 * @file GlobalPeers.hpp
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
#ifndef ZIQE_GLOBALPEERS_H
#define ZIQE_GLOBALPEERS_H

#include "Base/LocalThread.hpp"

#include "Core/GlobalThread.hpp"
#include "Protocol/MessageStream.hpp"
#include "Protocol/MessagesGenerator.hpp"

namespace Ziqe {

/**
 * @brief The GlobalPeers class  A Global manager for other peers lookup and
 *                               creating new threads.
 */
class GlobalPeers : private Protocol::MessageStream::Callback
{
public:
    GlobalPeers();

    /**
     * @brief runThread  Try to run a thread on other computer.
     * @param localThread  The local thread to run.
     * @return The new Thread or null on failure.
     *
     * This function will lookup for other machines on the network
     * that running Ziqe. If there are, it will choose one of them
     * to run this thread.
     *
     */
    Base::UniquePointer<GlobalThread> runThread(const LocalThread &localThread,
                                                const Base::SharedPointer<GlobalProcess>);

private:
    /**
     * @brief onMessageReceived
     * @param type
     * @param fieldReader
     * @param messageStream
     */
    virtual void onMessageReceived (const Protocol::Message &type,
                                    MessageFieldReader &fieldReader,
                                    const Protocol::MessageStream &messageStream) override;

    Protocol::MessageStream mGlobalMessageStream;
    Protocol::MessagesGenerator::IdentiferType mNextIdentifer;

};

} // namespace Ziqe

#endif // ZIQE_GLOBALPEERS_H
