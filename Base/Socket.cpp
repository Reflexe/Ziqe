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

Socket::Socket(Family family, Type type, ZqSocketProtocol protocol)
{
    mSocket = ZqSocketOpen (static_cast<ZqSocketFamily>(family),
                            static_cast<ZqSocketType>(type),
                            protocol);
}

Expected<Socket,Socket::ConnectError>
Socket::Connect(const Socket::SocketAddress &address, Socket::Type type, ZqSocketProtocol protocol) {
    // first of all, create the socket.
    Socket socket{static_cast<Family>(address.getFamily ()), type, protocol};

    // If it is not a valid socket, return nullptr.
    if (! socket.isOpen ())
        return {ConnectError::Other};

    if (! socket.connect (address))
        return {ConnectError::Timeout};

    return {std::move (socket)};
}

Expected<Socket,Socket::ListenError>
Socket::Listen(const Socket::SocketAddress &address, Socket::Type type,
               ZqSizeType backlog, ZqSocketProtocol protocol)
{
    // We should bind the socket before listening to it.
    auto bindedSocket = Bind (address, type, protocol);

    if (! bindedSocket)
        return {ListenError::Other};

    if (! bindedSocket->listen (backlog))
        return {ListenError::Other};

    return {std::move (bindedSocket.get ())};
}

Expected<Socket,Socket::BindError>
Socket::Bind(const Socket::SocketAddress &address,
             Socket::Type type, ZqSocketProtocol protocol)
{
    // first of all, create the socket.
    Socket socket{static_cast<Family>(address.getFamily ()), type, protocol};

    // If it is not a valid socket, return nullptr.
    if (! socket.isOpen ())
        return {BindError::Other};

    if (! socket.bind (address))
        return {BindError::Other};

    return {std::move (socket)};
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

Expected<Vector<uint8_t>,Socket::ReceiveError> Socket::receive() const{
    Vector<uint8_t> vector;
    ZqSizeType bytesReceived;

    vector.resize(ReceiveBufferSize);


    auto result = ZqSocketReceive (mSocket,
                                   vector.data (),
                                   vector.size (),
                                   &bytesReceived);
    if (! result)
        return {Socket::ReceiveError::Timeout};
    else if (bytesReceived == 0)
        return {Socket::ReceiveError::Disconnected};

    // Remove the unrequired bytes from the vector.
    vector.shrinkWithoutFree (vector.size () - bytesReceived);
    return {std::move (vector)};
}

void Socket::sendToAddress(const Socket::SocketAddress &socketAddress, const RawArray<const uint8_t> &array) const{
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

Expected<Pair<Vector<uint8_t>, Socket::SocketAddress>, Socket::ReceiveError>
Socket::receiveWithAddress() const{
    Vector<uint8_t> vector;
    ZqSizeType bytesReceived;
    SocketAddress socketAddress;

    vector.resize(ReceiveBufferSize);

    auto result = ZqSocketReceiveFrom (mSocket,
                                       vector.data (),
                                       vector.size (),
                                       &socketAddress.get (),
                                       &bytesReceived);

    if (! result)
        return {Socket::ReceiveError::Timeout};
    else if (bytesReceived == 0)
        return {Socket::ReceiveError::Disconnected};

    // Remove the unrequired bytes from the vector.
    vector.shrinkWithoutFree (vector.size () - bytesReceived);
    return {vector, socketAddress};
}

bool Socket::bind(const SocketAddress &address) {
    if (! ZqSocketBind (mSocket, &address.get ()))
        return false;

    return true;
}

bool Socket::connect(const SocketAddress &address) {
    if (! ZqSocketConnect (mSocket, &address.get ()))
       return false;

    return true;
}

bool Socket::listen(ZqSizeType backlog) {
    if (! ZqSocketListen (mSocket, backlog))
        return false;

    return true;
}

} // namespace Base
} // namespace Ziqe
