/**
 * @file NetworkProtocol.cpp
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
#include "NetworkProtocol.hpp"

namespace Ziqe {
namespace Net {

NetworkProtocol::NetworkProtocol(NetworkProtocol::InternetProtocol internetProtocol,
                                 NetworkProtocol::TransportProtocol transportProtocol) {
    mSocket = ZqSocketOpen (static_cast<ZqSocketFamily>(internetProtocol),
                            static_cast<ZqSocketType>(transportProtocol),
                            0);

    DEBUG_CHECK (mSocket != ZQ_INVALID_SOCKET);
    
    ZqSocketBindConnect ();
}

NetworkProtocol::NetworkProtocol(NetworkPacket::PacketInfo &packetInfo)
{
    
}

NetworkProtocol::~NetworkProtocol() {
    if (mSocket != ZQ_INVALID_SOCKET)
        ZqSocketClose (mSocket);
}

Base::UniquePointer<NetworkPacket> NetworkProtocol::receivePacket() {
    Base::ExtendedVector<uint8_t> buffer;
    ZqSocketAddress sockaddr;
    ZqSizeType bytesReceived;

    // MAYBE: build a memory pool api and use it here.
    // Allocate memory.
    buffer.getVector ().resize (sReceiveBufferSize);

    // Receive the packet.
    ZqBool result = ZqSocketReceiveFrom (mSocket,
                                         buffer.data (),
                                         sReceiveBufferSize,
                                         &sockaddr, &bytesReceived);
    DEBUG_CHECK (result);

    // "Resize" the buffer to the received size.
    // NOTE: in reality, nothing is changing: only the end index
    //       in buffer.
    buffer.setEnd (bytesReceived);

    return Base::makeUnique<NetworkPacket>(NetworkPacket::PacketInfo{sockaddr}, buffer);
}

void NetworkProtocol::sendPacket(NetworkPacket &&networkPacket) {
    ZqSizeType bytesSent;

    ZqBool result = ZqSocketSendTo (mSocket,
                                    &networkPacket.getData ()[0],
                                    networkPacket.getData ().size (),
                                    networkPacket.getInfo ().getSockaddr (),
                                    &bytesSent);
    DEBUG_CHECK (result);

    // TODO: may not send all of the packet.
}

Base::UniquePointer<NetworkProtocol> NetworkProtocol::CreateFromPacket(const NetworkPacket &packet)
{
    return Base::makeUnique<NetworkProtocol>(packet.getInfo ());
}

} // namespace Net
} // namespace Ziqe
