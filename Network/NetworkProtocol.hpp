/**
 * @file NetworkProtocol.hpp
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
#ifndef ZIQE_NETWORKPROTOCOL_H
#define ZIQE_NETWORKPROTOCOL_H

#include "Base/Memory.hpp"
#include "Base/IOStreamInterface.hpp"

#include "NetworkPacket.hpp"

namespace Ziqe {
namespace Net {

class NetworkProtocol
{
public:
    enum class InternetProtocol : ZqSocketFamily {
        IPv4 = ZQ_SOCKET_INET,
        IPv6 = ZQ_SOCKET_INET6
    };

    enum class TransportProtocol : ZqSocketType {
        TCP = ZQ_SOCKET_TYPE_STREAM,
        UDP = ZQ_SOCKET_TYPE_DGRAM
    };

    static const SizeType sReceiveBufferSize = 1;

    NetworkProtocol(InternetProtocol internetProtocol,
                    TransportProtocol transportProtocol);
    NetworkProtocol(NetworkPacket::PacketInfo &packetInfo);
    ~NetworkProtocol();

    struct Callback {
        Callback() = default;
        virtual ~Callback () = 0;
        ALLOW_COPY_AND_MOVE (Callback)

        // MAYBE: remove these virtual function, there is no real need for them, they can be a regular
        //        functions if callbackReceiveData and callbackReceivePacket would get a template T param.
        virtual void onPacketReceived (NetworkProtocol &protocol, Base::UniquePointer<NetworkPacket> &&packet) = 0;
    };

    Base::UniquePointer<NetworkPacket> receivePacket ();

    void sendPacket (NetworkPacket &&networkPacket);

    static Base::UniquePointer<NetworkProtocol> CreateFromPacket (const NetworkPacket &packet);

    void callbackReceivePacket (Callback &callback)
    {
        callback.onPacketReceived (*this, receivePacket ());
    }

private:
    ZqSocket mSocket;
};

} // namespace Net
} // namespace Ziqe

#endif // ZIQE_NETWORKPROTOCOL_H
