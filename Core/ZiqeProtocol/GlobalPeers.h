/**
 * @file GlobalPeers.h
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

#include "Base/LocalThread.h"

#include "Network/NetworkProtocolPool.h"

#include "Core/GlobalThread.h"

namespace Ziqe {

class GlobalPeers : private NetworkProtocolPool::Callback
{
public:
    struct Callback {
        Callback() = default;
        virtual ~Callback() = 0;
        ALLOW_COPY_AND_MOVE (Callback)

        virtual void onRunThreadPeerLookup (UniquePointer<NetworkPacket> &&packet,
                                            FieldReader &&fieldsReader,
                                            Message     &&message) = 0;
        virtual void onPropose ();
        virtual void onAcceptPropose ();
    };

    GlobalPeers();

    void run ()
    {
        mNetworkProtocol->callbackReceivePacket (this);
    }

    // first=the new thread. second=the new process connection.
    Pair<UniquePointer<GlobalThread>, UniquePointer<NetworkProtocol>> runThread(const LocalThread &localThread);

private:
    virtual void onPacketReceived (const SharedPointer<NetworkProtocol> &protocol,
                                   UniquePointer<NetworkPacket> &&packet);

    void processGlobalMessage (Message &&message,
                               FieldReader &&reader,
                               UniquePointer<NetworkPacket> &&packet);

    UniquePointer<Callback> mCallback;
    SharedPointer<NetworkProtocol> mNetworkProtocol;
};

} // namespace Ziqe

#endif // ZIQE_GLOBALPEERS_H
