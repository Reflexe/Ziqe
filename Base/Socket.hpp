/**
 * @file Socket.hpp
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
#ifndef ZIQE_BASE_SOCKET_HPP
#define ZIQE_BASE_SOCKET_HPP

#include "ZiqeAPI/Socket.h"

#include "Base/Memory.hpp"
#include "Base/Vector.hpp"

namespace Ziqe {
namespace Base {

class Socket
{
public:
    static const SizeType ReceiveBufferSize = 512;

    struct SocketAddress {
        SocketAddress() = default;

        static SocketAddress CreateIn6(Zq_in6_addr address, ZqPort port) {
            SocketAddress socketAddress;

            socketAddress.mSocketAddress.in6.sin6_family   = ZQ_AF_INET6;
            socketAddress.mSocketAddress.in6.sin6_addr     = address;
            socketAddress.mSocketAddress.in6.sin6_port     = port;
            socketAddress.mSocketAddress.in6.sin6_flowinfo = 0;
            socketAddress.mSocketAddress.in6.sin6_scope_id = 0;
            socketAddress.mSocketAddress.socklen = sizeof(socketAddress.mSocketAddress.in6);

            return socketAddress;
        }

        static SocketAddress CreateIn4(Zq_in_addr address, ZqPort port) {
            SocketAddress socketAddress;

            socketAddress.mSocketAddress.in.sin_family = ZQ_AF_INET;
            socketAddress.mSocketAddress.in.sin_addr   = address;
            socketAddress.mSocketAddress.in.sin_port   = port;
            socketAddress.mSocketAddress.socklen = sizeof(socketAddress.mSocketAddress.in);

            return socketAddress;
        }

        ZQ_DEFINE_CONST_AND_NON_CONST (const ZqSocketAddress &, ZqSocketAddress &, get, (), { return mSocketAddress; })

        ZqSocketFamily getFamily() const
        {
            return mSocketAddress.in.sin_family;
        }

    private:
        ZqSocketAddress mSocketAddress;
    };

    Socket(ZqSocketFamily family, ZqSocketType type, ZqSocketProtocol protocol);
    Socket(const SocketAddress &address, ZqSocketType type, ZqSocketProtocol protocol=0);

    static Socket Connect(const SocketAddress &address, ZqSocketType type, ZqSocketProtocol protocol=0);
    static Socket Listen(const SocketAddress &address, ZqSocketType type, ZqSizeType backlog=ZQ_NO_BACKLOG, ZqSocketProtocol protocol=0);
    static Socket Bind(const SocketAddress &address, ZqSocketType type, ZqSocketProtocol protocol=0);

    ZQ_DISALLOW_COPY(Socket)
    Socket(Socket &&socket)
        : mSocket{socket.mSocket}
    {
        socket.mSocket = ZQ_INVALID_SOCKET;
    }

    Socket &operator =(Socket &&socket) {
        close();

        mSocket = socket.mSocket;
        socket.mSocket = ZQ_INVALID_SOCKET;

        return *this;
    }

    bool isOpen() const;

    void close();

    void send(const RawArray<const uint8_t> &array) const;

    Vector<uint8_t> receive() const;

    SocketAddress acceptClient();

    void sendToAddress(const SocketAddress &socketAddress, const RawArray<uint8_t> &array) const;

    Pair<Vector<uint8_t>, SocketAddress> receiveWithAddress() const;

private:
    void bind(const SocketAddress &socketAddress);

    ZqSocket mSocket;

};

} // namespace Base
} // namespace Ziqe

#endif // ZIQE_BASE_SOCKET_HPP
