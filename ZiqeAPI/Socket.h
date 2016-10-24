/**
 * @file Socket
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
#ifndef ZIQEAPI_SOCKET_H
#define ZIQEAPI_SOCKET_H

#include "Macros.h"
#include "Types.h"
#include "Memory.h"

#include "SocketConfig.h.generated"

#define ZQ_NO_BACKLOG (0)

typedef struct {
    ZqSizeType socklen;
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
ZqBool ZqSocketListen (ZqSocket socket,
                           ZqSizeType backlog);

/**
 * @brief Bind a client / connectionless socket.
 * @param socket
 * @param sockaddr
 * @return
 */
ZqBool ZqSocketConnect (ZqSocket zqsocket,
                            const ZqSocketAddress *sockaddr);

ZqBool ZqSocketBind(ZqSocket zqsocket,
                    const ZqSocketAddress *sockaddr);

/**
 * @brief Receive data from a socket.
 * @param socket
 * @param buffer
 * @param bufferSize
 * @return
 */
ZqBool ZqSocketReceive(ZqSocket zqsocket,
                       ZqKernelAddress buffer,
                       ZqSizeType bufferSize,
                       ZqSizeType *bytesReceived);

/**
 * @brief Send data to a socket.
 * @param socket
 * @param buffer
 * @param bufferSize
 * @return
 */
ZqBool ZqSocketSend(ZqSocket zqsocket,
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
ZqBool ZqSocketReceiveFrom(ZqSocket zqsocket,
                           ZqKernelAddress buffer,
                           ZqSizeType bufferSize,
                           ZqSocketAddress *sockaddr,
                           ZqSizeType *bytesReceived);

/**
 * @brief Send data to a socket.
 * @param socket
 * @param buffer
 * @param bufferSize
 * @return
 */
ZqBool ZqSocketSendTo(ZqSocket zqsocket,
                      ZqConstKernelAddress buffer,
                      ZqSizeType bufferSize,
                      const ZqSocketAddress *sockaddr,
                      ZqSizeType *bytesSent);

ZqBool ZqSetSocketOption (ZqSocket zqsocket,
                          ZqSocketOptionLevel level,
                          ZqSocketOptionName name,
                          ZqConstKernelAddress optionValue,
                          ZqSizeType optionSize);

ZqBool ZqAccept(ZqSocket zqsocket,
                )

ZQ_END_C_DECL

#endif // ZIQEAPI_SOCKET_H
