/**
 * @file ProcessPeers.h
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
#ifndef ZIQE_PROCESSPEERS_H
#define ZIQE_PROCESSPEERS_H

#include "Base/Macros.h"
#include "Base/LocalThread.h"
#include "Base/LocalProcess.h"

#include "Network/NetworkProtocolPool.h"

#include "Core/GlobalThread.h"

namespace Ziqe {

class ProcessPeers final : public NetworkProtocolPool::Callback
{
public:
    ProcessPeers();

    struct Callback {
        Callback() = default;
        ALLOW_COPY_AND_MOVE (Callback)
        virtual ~Callback() = 0;

        /// Server callbacks
        virtual void onGetMemoryReceived (const SharedPointer<NetworkProtocol> &protocol,
                                          ZqAddress address, SizeType length) = 0;
        virtual void onWriteMemoryReceived (ZqAddress address,
                                            const SharedVector<Byte> &memory) = 0;

        virtual void onGivePageReceived (ZqAddress page, const SharedVector<Byte> &page) = 0;

        virtual void onStopThreadReceived (GlobalThreadID threadID) = 0;
        virtual void onContiniueThread    (GlobalThreadID threadID) = 0;

        virtual void onRunThreadReceived () = 0;
        virtual void onKillThreadReceived (GlobalThreadID threadID) = 0;

        /// Client callbacks.
        virtual void onGetMemoryResultReceived ();
        virtual void onRunThreadProposeReceived ();
        virtual void onGoodbyeReceived ();
    };

    ProcessPeers(UniquePointer<Callback> &&callback);

    SharedVector<Byte> getMemory (ZqAddress address,
                                  SizeType size);

    void writeMemory (const SharedVector<Byte> &data);

    void stopThread (GlobalThread &thread);
    void continueThread (GlobalThread &thread);

    void killThread (GlobalThread &thread);
    UniquePointer<GlobalThread> runThread (LocalThread &localThread);

    void addProcessConnection (UniquePointer<NetworkProtocol> &&processConnection);

private:
    virtual void onPacketReceived (const SharedPointer<NetworkProtocol> &protocol,
                                   UniquePointer<NetworkPacket> &&packet) override;

    UniquePointer<Callback> mCallback;

    LocalProcess mProcess;
};

} // namespace Ziqe

#endif // ZIQE_PROCESSPEERS_H
