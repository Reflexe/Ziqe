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
#ifndef ZIQE_NETWORKSESSION_H
#define ZIQE_NETWORKSESSION_H

#include "Base/Memory.hpp"
#include "Base/Socket.hpp"
#include "Base/IOStreamInterface.hpp"

#include "NetworkPacket.hpp"

namespace Ziqe {
namespace Net {

struct Ziqe {

};

class NetworkSession
{
public:
    enum class InternetProtocol : ZqSocketFamily {
        IPv4 = ZQ_AF_INET,
        IPv6 = ZQ_AF_INET6
    };

    enum class TransportProtocol : ZqSocketType {
        TCP = ZQ_SOCKET_TYPE_STREAM,
        UDP = ZQ_SOCKET_TYPE_DGRAM
    };

    NetworkSession(InternetProtocol internetProtocol,
                   TransportProtocol transportProtocol);
    NetworkSession(NetworkPacket::PacketInfo &packetInfo);
    ~NetworkSession();

    Base::UniquePointer<NetworkPacket> receivePacket ();

    void sendPacket (NetworkPacket &&networkPacket);

    static Base::UniquePointer<NetworkSession> CreateFromPacket (const NetworkPacket &packet);

    template<class Callback>
    void callbackReceivePacket (Callback &callback)
    {
        callback.onPacketReceived (*this, receivePacket ());
    }

private:
    ZqSocket mSocket;
};

} // namespace Net
} // namespace Ziqe

#endif // ZIQE_NETWORKSESSION_H
