/**
 * @file Socket.cpp
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

    // If it is not a valid socket, return an error.
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

void Socket::sendAll(const RawArray<const uint8_t> &array) const{
    ZqSizeType bytesSent = 0;

    while (bytesSent < array.size ()) {
        ZqSizeType lastBytesSent;

        auto result = ZqSocketSend (mSocket,
                                    static_cast<ZqConstKernelAddress>(array.get () + bytesSent),
                                    array.size () - bytesSent, &lastBytesSent);
        ZQ_ASSERT (result);

        bytesSent += lastBytesSent;
    }
}

Expected<Vector<uint8_t>,Socket::ReceiveError> Socket::receive() const{
    ZqSizeType bytesReceived;
    ZqKernelAddress bufferAddress;

    auto result = ZqSocketReceive (mSocket,
                                   &bufferAddress,
                                   &bytesReceived);

    if (result == ZQ_E_AGAIN)
        return {Socket::ReceiveError::Timeout};
    else if (result != ZQ_E_OK)
        return {Socket::ReceiveError::Other};
    else if (bytesReceived == 0)
        return {Socket::ReceiveError::Disconnected};

    return {Vector<uint8_t>::TakeRawArray (RawArray<uint8_t>(static_cast<uint8_t*>(bufferAddress),
                                                             bytesReceived))};
}

void Socket::sendAllToAddress(const Socket::SocketAddress &socketAddress, const RawArray<const uint8_t> &array) const{
    ZqSizeType bytesSent = 0;

    while (bytesSent < array.size ()) {
        ZqSizeType lastBytesSent;

        auto result = ZqSocketSendTo (mSocket, array.get () + bytesSent,
                                      array.size () - bytesSent,
                                      &socketAddress.get (),
                                      &lastBytesSent);
        ZQ_ASSERT (result == ZQ_E_OK);

        bytesSent += lastBytesSent;
    }
}

Expected<Pair<Vector<uint8_t>, Socket::SocketAddress>, Socket::ReceiveError>
Socket::receiveWithAddress() const{
    ZqKernelAddress bufferAddress;
    ZqSizeType bytesReceived;
    SocketAddress socketAddress;

    auto result = ZqSocketReceiveFrom (mSocket,
                                       &bufferAddress,
                                       &socketAddress.get (),
                                       &bytesReceived);

    if (result == ZQ_E_AGAIN)
        return {Socket::ReceiveError::Timeout};
    else if (result != ZQ_E_OK)
        return {Socket::ReceiveError::Other};
    else if (bytesReceived == 0)
        return {Socket::ReceiveError::Disconnected};

    return {Vector<uint8_t>::TakeRawArray (RawArray<uint8_t>(static_cast<uint8_t*>(bufferAddress), bytesReceived)),
            Base::move (socketAddress)};
}

bool Socket::bind(const SocketAddress &address) {
    if (ZqSocketBind (mSocket, &address.get ()) != ZQ_E_OK)
        return false;

    return true;
}

bool Socket::connect(const SocketAddress &address) {
    if (ZqSocketConnect (mSocket, &address.get ()) != ZQ_E_OK)
       return false;

    return true;
}

bool Socket::listen(ZqSizeType backlog) {
    if (ZqSocketListen (mSocket, backlog) != ZQ_E_OK)
        return false;

    return true;
}

Expected<Base::Pair<Socket, Socket::SocketAddress>, Socket::AccpetError> Socket::accept() {
    ZqSocketAddress sockaddr;
    auto maybeNewSocket = ZqSocketAccept (mSocket, &sockaddr);

    if (maybeNewSocket == ZQ_INVALID_SOCKET)
        return {AccpetError::Other};
    else
        return {maybeNewSocket, SocketAddress::CreateFromSockaddr (sockaddr)};
}

Socket::Socket(ZqSocket socket)
    : mSocket{socket}
{
}

SizeType Socket::sendToAddress(const SocketAddress &socketAddress, const RawArray<const uint8_t> &array) const{
    ZqSizeType bytesSent = 0;

    auto result = ZqSocketSendTo (mSocket, array.get (),
                                  array.size (),
                                  &socketAddress.get (),
                                  &bytesSent);
    ZQ_ASSERT (result == ZQ_E_OK);

    return bytesSent;
}

Expected<ZqSizeType, Socket::SendError> Socket::send(const RawArray<const uint8_t> &array) const{
    ZqSizeType bytesSent = 0;

    auto result = ZqSocketSend (mSocket,
                                array.get (),
                                array.size (),
                                &bytesSent);
    ZQ_ASSERT (result == ZQ_E_OK);

    return {bytesSent};
}

} // namespace Base
} // namespace Ziqe
