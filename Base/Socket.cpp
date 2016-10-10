/**
 * @file Socket.cpp
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
#include "Socket.hpp"

namespace Ziqe {
namespace Base {

Socket::Socket(ZqSocketFamily family, ZqSocketType type, ZqSocketProtocol protocol)
{
    mSocket = ZqSocketOpen (family, type, protocol);
}

Socket Socket::Connect(const Socket::SocketAddress &address, ZqSocketType type, ZqSocketProtocol protocol) {
    auto socket = Socket{address.getFamily (), type, protocol};

    ZqSocketConnect (socket.mSocket, &address.get ());

    return socket;
}

Socket Socket::Listen(const Socket::SocketAddress &address, ZqSocketType type,
                      ZqSizeType backlog, ZqSocketProtocol protocol) {
    auto socket = Socket{address.getFamily (), type, protocol};

    ZqSocketBind(socket.mSocket, &address.get ());
    ZqSocketListen (socket.mSocket, backlog);

    return socket;
}

Socket Socket::Bind(const Socket::SocketAddress &address, ZqSocketType type, ZqSocketProtocol protocol) {
    auto socket = Socket{address.getFamily (), type, protocol};

    ZqSocketBind(socket.mSocket, &address.get ());

    return socket;
}

bool Socket::isOpen() const
{
    return mSocket != ZQ_INVALID_SOCKET;
}

void Socket::close() {
    if (mSocket == ZQ_INVALID_SOCKET)
        return;

    ZqSocketClose (mSocket);
    mSocket = ZQ_INVALID_SOCKET;
}

void Socket::send(const RawArray<const uint8_t> &array) const{
    ZqSizeType bytesSent = 0;

    while (bytesSent < array.size ()) {
        ZqSizeType lastBytesSent;

        auto result = ZqSocketSend (mSocket,
                                    static_cast<ZqConstKernelAddress>(array.get () + bytesSent),
                                    array.size () - bytesSent, &lastBytesSent);
        DEBUG_CHECK (result);

        bytesSent += lastBytesSent;
    }
}

Vector<uint8_t> Socket::receive() const{
    Vector<uint8_t> vector;
    ZqSizeType bytesReceived;

    vector.resize(ReceiveBufferSize);


    auto result = ZqSocketReceive (mSocket,
                                   vector.data (),
                                   vector.size (),
                                   &bytesReceived);
    DEBUG_CHECK (result);

    // Remove the unrequired bytes from the vector.
    vector.shrinkWithoutFree (vector.size () - bytesReceived);
    return vector;
}

void Socket::sendToAddress(const Socket::SocketAddress &socketAddress, const RawArray<uint8_t> &array) const{
    ZqSizeType bytesSent = 0;

    while (bytesSent < array.size ()) {
        ZqSizeType lastBytesSent;

        auto result = ZqSocketSendTo (mSocket, array.get () + bytesSent,
                                      array.size () - bytesSent,
                                      &socketAddress.get (),
                                      &lastBytesSent);
        DEBUG_CHECK (result);

        bytesSent += lastBytesSent;
    }
}

Pair<Vector<uint8_t>, Socket::SocketAddress> Socket::receiveWithAddress() const{
    Vector<uint8_t> vector;
    ZqSizeType bytesReceived;
    SocketAddress socketAddress;

    vector.resize(ReceiveBufferSize);

    auto result = ZqSocketReceiveFrom (mSocket,
                                       vector.data (),
                                       vector.size (),
                                       &socketAddress.get (),
                                       &bytesReceived);
    DEBUG_CHECK (result);

    // Remove the unrequired bytes from the vector.
    vector.shrinkWithoutFree (vector.size () - bytesReceived);
    return {vector, socketAddress};
}

} // namespace Base
} // namespace Ziqe
