/**
 * @file Socket
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
#ifndef ZIQEAPI_SOCKET_H
#define ZIQEAPI_SOCKET_H

#include "CppCore/Macros.h"
#include "Types.h"
#include "Memory.h"

#include "SocketConfig.gen.h"

#define ZQ_NO_BACKLOG (0)

/**
  A POD type for holding socket address.
  */
typedef struct {
    /* The size of the current active sockaddr. */
    ZqSizeType socklen;
    ZqSocketFamily socket_family;

    union {
        Zq_sockaddr_in in;
        Zq_sockaddr_in6 in6;
    };
} ZqSocketAddress;

ZQ_BEGIN_C_DECL

/**
 * @brief Create a new socket.
 * @param family
 * @param type
 * @param protocol
 * @return
 */
ZqSocket ZqSocketOpen (ZqSocketFamily family,
                       ZqSocketType type,
                       ZqSocketProtocol protocol);
/**
 * @brief ZqSocketClose  Close a socket.
 * @param socket
 */
void ZqSocketClose (ZqSocket socket);

/**
 * @brief Bind a server socket.
 * @param socket
 * @param sockaddr
 * @param backlog
 * @return
 *
 * On supported streams, this function will try to create
 * a server that can accept connections.
 */
ZqError ZqSocketListen (ZqSocket socket,
                        ZqSizeType backlog);

/**
 * @brief Bind a client / connectionless socket.
 * @param socket
 * @param sockaddr
 * @return
 */
ZqError ZqSocketConnect (ZqSocket zqsocket,
                         const ZqSocketAddress *sockaddr);

ZqError ZqSocketBind(ZqSocket zqsocket,
                     const ZqSocketAddress *sockaddr);

/**
 * @brief Receive data from a socket.
 * @param socket
 * @param buffer
 * @param bufferSize
 * @return
   @todo The receive functions should return a pair of bytesReceived,buffer
         (instead of allocating a buffer).
   NOT thread safe.
 */
ZqError ZqSocketReceive(ZqSocket zqsocket,
                        ZqKernelAddress *pbuffer,
                       ZqSizeType *bytesReceived);

/**
 * @brief Send data to a socket.
 * @param socket
 * @param buffer
 * @param bufferSize
 * @return
 */
ZqError ZqSocketSend(ZqSocket zqsocket,
                     ZqConstKernelAddress buffer,
                     ZqSizeType bufferSize,
                     ZqSizeType *bytesSent);


/**
 * @brief Receive data from a socket.
 * @param socket
 * @param buffer
 * @param bufferSize
 * @return
 */
ZqError ZqSocketReceiveFrom(ZqSocket zqsocket,
                            ZqKernelAddress *pbuffer,
                            ZqSocketAddress *sockaddr,
                            ZqSizeType *bytesReceived);

/**
 * @brief Send data to a socket.
 * @param socket
 * @param buffer
 * @param bufferSize
 * @return
 */
ZqError ZqSocketSendTo(ZqSocket zqsocket,
                       ZqConstKernelAddress buffer,
                       ZqSizeType bufferSize,
                       const ZqSocketAddress *sockaddr,
                       ZqSizeType *bytesSent);

/**
   @brief Set a socket option.
   @param zqsocket  The Socket.
   @param level     The socket option level.
   @param name      The socket option name (ID).
   @param optionValue   The option value.
   @param optionSize    The size (in bytes) of the option value.
   @return ZQ_E_OK on success.
 */
ZqError ZqSocketSetOption (ZqSocket zqsocket,
                           ZqSocketOptionLevel level,
                           ZqSocketOptionName name,
                           ZqConstKernelAddress optionValue,
                           ZqSizeType optionSize);


/**
   @brief Accept a client.
   @param zqsocket  A server socket.
   @param maybeSocketAddress    NULL, or a pointer to a socket address to be filled with the new client's
                                socket address.
   @return The new client socket on success, ZQ_INVALID_SOCKET on failure.
 */
ZqSocket ZqSocketAccept (ZqSocket zqsocket,
                         ZqSocketAddress *maybeSocketAddress);

ZQ_END_C_DECL

#endif // ZIQEAPI_SOCKET_H
