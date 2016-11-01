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
#include "Base/Expected.hpp"

namespace Ziqe {
namespace Base {

class Socket
{
public:
    static const SizeType ReceiveBufferSize = 512;

    /**
       @brief   Possible socket option layers.
     */
    enum class OptionLevel : ZqSocketOptionLevel {
        /// SOL_SOCKET
        Socket = ZQ_LEVEL_SOCKET
    };

    /**
       @brief   Possible socket option names.
     */
    enum class OptionName : ZqSocketOptionName {
        /// SO_BROADCAST
        Broadcast = ZQ_OPTION_BROADCAST
    };

    enum class Type : ZqSocketType {
        /// SOCK_STREAM
        Stream = ZQ_SOCKET_TYPE_STREAM,
        /// SOCK_DGRAM
        Datagram = ZQ_SOCKET_TYPE_DGRAM
    };

    enum class Family : ZqSocketFamily {
        /// AF_UNSPEC
        Unsec = ZQ_AF_UNSPEC,
        /// AF_INET
        IPv4 = ZQ_AF_INET,
        /// AF_INET6
        IPv6 = ZQ_AF_INET6,
    };

    /**
       @brief   An creator and container of ZqSocketAddress structs.
     */
    struct SocketAddress {
        SocketAddress() = default;

        static SocketAddress CreateIn6(Zq_in6_addr address, ZqPort port) {
            SocketAddress socketAddress;

            socketAddress.mSocketAddress.socket_family     = ZQ_AF_INET6;
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

            socketAddress.mSocketAddress.socket_family
                    = socketAddress.mSocketAddress.in.sin_family = ZQ_AF_INET;

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

    Socket(Family family, Type type, ZqSocketProtocol protocol=0);
    Socket(const SocketAddress &address, Type type, ZqSocketProtocol protocol=0);

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

    enum class ConnectError {
        Timeout,
        Other
    };

    /**
       @brief Try to create a socket and connect it.
       @param address   The socket address to connect to.
       @param type      The socket type to be created.
       @param protocol  The socket protocol.
       @return On success: A pointer to the newly created socket on success.
               On failure: nullptr.
     */
    static Expected<Socket,ConnectError> Connect(const SocketAddress &address, Type type, ZqSocketProtocol protocol=0);

    enum class ListenError{
        Other
    };

    /**
       @brief Try to create a socket and listen for new connections.
       @param address   The socket address to listen to.
       @param type      The socket type to be created.
       @param backlog   The maximum number of waiting clients (ZQ_NO_BACKLOG for no limit).
       @param protocol  The socket protocol.
       @return On success: A pointer to the newly created socket on success.
               On failure: nullptr.
     */
    static Expected<Socket, ListenError> Listen(const SocketAddress &address,
                                                Type type,
                                                ZqSizeType backlog=ZQ_NO_BACKLOG,
                                                ZqSocketProtocol protocol=0);

    enum class BindError{
        Other
    };

    /**
       @brief Try to create a socket and bind it.
       @param address   The socket address to bind this socket to.
       @param type      The socket type to be created.
       @param protocol  The socket protocol.
       @return On success: A pointer to the newly created socket on success.
               On failure: nullptr.
     */
    static Expected<Socket,BindError> Bind(const SocketAddress &address, Type type, ZqSocketProtocol protocol=0);

    /**
       @brief Check if this socket is open and valid or closed or invalid.
     */
    bool isOpen() const;

    /**
       @brief If open, close this socket.
     */
    void close();

    /**
       @brief Send an array of bytes to this socket.
       @param array
     */
    void send(const RawArray<const uint8_t> &array) const;

    enum class ReceiveError {
        Disconnected,
        Timeout,
        Other
    };

    /**
       @brief   Receive an array of bytes from this socket.
       @return  The received bytes.
     */
    Expected<Vector<uint8_t>,ReceiveError> receive() const;

    /**
       @brief Wait until a new client try to connect.
       @return a Pair:
                - first:    the client's socket.
                - second:   the client's SocketAddress.
     */
    Pair<Socket, SocketAddress> acceptClient();

    /**
       @brief Send a bytes array to @a socketAddress.
       @param socketAddress
       @param array
     */
    void sendToAddress(const SocketAddress &socketAddress, const RawArray<const uint8_t> &array) const;

    /**
       @brief   Receive a bytes array and the sender's SocketAddress.
       @return  a Pair:
                    - first: The received bytes.
                    - second: The sender's socket address.
     */
    Expected<Pair<Vector<uint8_t>, SocketAddress>, ReceiveError> receiveWithAddress() const;

    /**
      @brief Type-safely set a socket option.

      @param level  This option's level, one from Z

      A wapper around ZqSocketSetOption that provides type safety.
     */
    template<class T>
    void setSocketOption (OptionLevel level, OptionName name, const T &optionValue) {
        ZqSocketSetOption (mSocket, static_cast<ZqSocketOptionLevel>(level), static_cast<ZqSocketOptionName>(name),
                           static_cast<ZqConstKernelAddress> (&optionValue), sizeof (optionValue));
    }

    /**
       @brief Bind a SocketAddress to this socket.
       @param address
       @return
     */
    bool bind(const SocketAddress &address);

    /**
       @brief Connect a SocketAddress to this socket.
       @param address
       @return
     */
    bool connect (const SocketAddress &address);

    /**
       @brief Tell a socket to listen (Hi Jim, LISTEN!)
       @param address
       @param backlog
     */
    bool listen(ZqSizeType backlog=ZQ_NO_BACKLOG);

private:
    /**
       @brief  This socket's ZqSocket.
     */
    ZqSocket mSocket;

};

} // namespace Base
} // namespace Ziqe

#endif // ZIQE_BASE_SOCKET_HPP
