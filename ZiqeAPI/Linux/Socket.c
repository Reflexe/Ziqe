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

#include "../Socket.h"

#include <linux/socket.h>
#include <linux/net.h>

#define zqsocket_to_socket(zqsocket) ((struct socket *) zqsocket)

static int ziqe_sendmsg (struct socket *sock, ZqKernelAddress buffer, ZqSizeType buffer_size,
                         struct sockaddr *sockaddr, ZqSizeType sockaddr_len);
static int ziqe_recvmsg (struct socket *sock, ZqKernelAddress buffer,
                         ZqSizeType buffer_size, struct sockaddr *sockaddr,
                         ZqSizeType sockaddr_len);

ZqSocket ZqSocketOpen(ZqSocketFamily family,
                      ZqSocketType type,
                      ZqSocketProtocol protocol) {
    struct socket *sock;
    int retval;

     retval = sock_create(family, type, protocol, &sock);
     if (retval != 0)
         return ZQ_INVALID_SOCKET;
     else
         return (ZqSocket) sock;
}

void ZqSocketClose(ZqSocket zqsocket) {
    struct socket *sock = zqsocket_to_socket (zqsocket);

    sock_release(sock);
}

ZqBool ZqSocketBindListen(ZqSocket zqsocket,
                          ZqSocketAddress sockaddr,
                          ZqSizeType backlog)
{
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int retval;

    retval = sock->ops->bind(sock, (struct sockaddr *) &sockaddr.storage,
                             sockaddr.socklen);
    if (retval)
        return ZQ_FALSE;

    retval = sock->ops->listen (sock, backlog);
    if (retval)
        return ZQ_FALSE;

    return ZQ_TRUE;
}

ZqBool ZqSocketBindConnect(ZqSocket zqsocket, ZqSocketAddress sockaddr) {
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int retval;

    retval = sock->ops->connect(sock, (struct sockaddr *) &sockaddr.storage,
                                sockaddr.socklen, 0);

    if (retval)
        return ZQ_FALSE;
    else
        return ZQ_TRUE;
}

ZqBool ZqSocketReceive(ZqSocket zqsocket, ZqKernelAddress buffer, ZqSizeType bufferSize,
                       ZqSizeType *bytesReceived) {
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int ret;

    ret = ziqe_recvmsg (sock, buffer, bufferSize, NULL, 0);
    if (ret < 0)
        return ZQ_FALSE;

    if (bytesReceived)
        *bytesReceived = (ZqSizeType) ret;

    return ZQ_TRUE;
}

ZqBool ZqSocketSend(ZqSocket zqsocket, ZqKernelAddress buffer, ZqSizeType bufferSize, ZqSizeType *bytesSent) {
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int ret;

    ret = ziqe_sendmsg (sock, buffer, bufferSize, NULL, 0);
    if (ret < 0)
        return ZQ_FALSE;

    if (bytesSent)
        *bytesSent = (ZqSizeType) ret;

    return ZQ_TRUE;
}

ZqBool ZqSocketReceiveFrom(ZqSocket zqsocket, ZqKernelAddress buffer,
                           ZqSizeType bufferSize, ZqSocketAddress sockaddr,
                           ZqSizeType *bytesReceived) {
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int ret;

    ret = ziqe_recvmsg (sock, buffer,
                        bufferSize, (struct sockaddr *) &sockaddr.storage,
                        sockaddr.socklen);
    if (ret < 0)
        return ZQ_FALSE;

    if (bytesReceived)
        *bytesReceived = (ZqSizeType) ret;

    return ZQ_TRUE;
}

ZqBool ZqSocketSendTo(ZqSocket zqsocket, ZqKernelAddress buffer,
                      ZqSizeType bufferSize, ZqSocketAddress sockaddr,
                      ZqSizeType *bytesSent) {
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int ret;

    ret = ziqe_sendmsg (sock, buffer,
                        bufferSize, (struct sockaddr *) &sockaddr.storage,
                        sockaddr.socklen);
    if (ret < 0)
        return ZQ_FALSE;

    if (bytesSent)
        *bytesSent = (ZqSizeType) ret;

    return ZQ_TRUE;
}


static int ziqe_sendmsg (struct socket *sock,
                         ZqKernelAddress buffer, ZqSizeType buffer_size,
                         struct sockaddr *sockaddr, ZqSizeType sockaddr_len) {
    struct msghdr msg;
    struct kvec kvec[1];
    int retval;

    msg.msg_name     = sockaddr;
    msg.msg_namelen  = sockaddr_len;
    msg.msg_control  = NULL;
    msg.msg_controllen = 0;
    msg.msg_flags    = 0;

    kvec[0].iov_base = buffer;
    kvec[0].iov_len = buffer_size;

    retval = kernel_sendmsg(sock, &msg, kvec, 1, kvec[0].iov_len);

    return retval;
}

static int ziqe_recvmsg (struct socket *sock, ZqKernelAddress buffer,
                         ZqSizeType buffer_size, struct sockaddr *sockaddr,
                         ZqSizeType sockaddr_len) {
    struct msghdr msg;
    struct kvec kvec[1];
    int retval;

    msg.msg_name     = sockaddr;
    msg.msg_namelen  = sockaddr_len;
    msg.msg_control  = NULL;
    msg.msg_controllen = 0;
    msg.msg_flags    = 0;

    kvec[0].iov_base = buffer;
    kvec[0].iov_len = buffer_size;

    retval = kernel_recvmsg(sock, &msg, kvec, 1, kvec[0].iov_len, 0);

    return retval;
}

ZqBool ZqSetSocketOption(ZqSocket zqsocket,
                         ZqSocketOptionLevel level,
                         ZqSocketOptionName name,
                         ZqKernelAddress optionValue,
                         ZqSizeType optionSize)
{
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int ret;

    ret = sock->ops->setsockopt (sock, level, name, optionValue, optionSize);

    if (ret)
        return ZQ_FALSE;
    else
        return ZQ_TRUE;
}
