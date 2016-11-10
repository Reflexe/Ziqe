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

#define _LINUX

#include "../Socket.h"

#include <linux/socket.h>
#include <linux/net.h>

#define zqsocket_to_socket(zqsocket) ((struct socket *) zqsocket)

static int ziqe_sendmsg (struct socket *sock, ZqConstKernelAddress buffer, ZqSizeType buffer_size,
                         struct sockaddr *sockaddr, ZqSizeType sockaddr_len);
static int ziqe_recvmsg (struct socket *sock, ZqKernelAddress buffer,
                         ZqSizeType buffer_size, struct sockaddr *sockaddr,
                         ZqSizeType *sockaddr_len);

ZqSocket ZqSocketOpen(ZqSocketFamily family,
                      ZqSocketType type,
                      ZqSocketProtocol protocol) {
    struct socket *sock;
    int retval;

     retval = sock_create_lite (family, type, protocol, &sock);
     if (retval != 0)
         return ZQ_INVALID_SOCKET;
     else
         return (ZqSocket) sock;
}

void ZqSocketClose(ZqSocket zqsocket) {
    struct socket *sock = zqsocket_to_socket (zqsocket);

    sock_release(sock);
}

ZqBool ZqSocketListen(ZqSocket zqsocket,
                      ZqSizeType backlog)
{
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int retval;

    retval = kernel_listen (sock, backlog);
    if (retval)
        return ZQ_FALSE;

    return ZQ_TRUE;
}

ZqBool ZqSocketBind(ZqSocket zqsocket, const ZqSocketAddress *sockaddr) {
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int retval;

    retval = kernel_bind(sock, (struct sockaddr *) &sockaddr->in,
                         sockaddr->socklen);
    if (retval)
        return ZQ_FALSE;

    return ZQ_TRUE;
}


ZqBool ZqSocketConnect(ZqSocket zqsocket, const ZqSocketAddress *sockaddr) {
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int retval;

    retval = kernel_connect(sock, (struct sockaddr *) &sockaddr->in,
                           sockaddr->socklen, 0);

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

ZqBool ZqSocketSend(ZqSocket zqsocket, ZqConstKernelAddress buffer, ZqSizeType bufferSize, ZqSizeType *bytesSent) {
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
                           ZqSizeType bufferSize, ZqSocketAddress *sockaddr,
                           ZqSizeType *bytesReceived) {
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int ret;
    ZqSizeType socket_length;

    ret = ziqe_recvmsg (sock, buffer,
                        bufferSize, (struct sockaddr *) &sockaddr->in,
                        &socket_length);
    if (ret < 0)
        return ZQ_FALSE;

    sockaddr->socklen = socket_length;

    if (bytesReceived)
        *bytesReceived = (ZqSizeType) ret;

    return ZQ_TRUE;
}

ZqBool ZqSocketSendTo(ZqSocket zqsocket, ZqConstKernelAddress buffer,
                      ZqSizeType bufferSize, const ZqSocketAddress *sockaddr,
                      ZqSizeType *bytesSent) {
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int ret;

    ret = ziqe_sendmsg (sock, buffer,
                        bufferSize, (struct sockaddr *) &sockaddr->in,
                        sockaddr->socklen);
    if (ret < 0)
        return ZQ_FALSE;

    if (bytesSent)
        *bytesSent = (ZqSizeType) ret;

    return ZQ_TRUE;
}


static int ziqe_sendmsg (struct socket *sock,
                         ZqConstKernelAddress buffer, ZqSizeType buffer_size,
                         struct sockaddr *sockaddr, ZqSizeType sockaddr_len) {
    struct msghdr msg;
    struct kvec kvec[1];
    int retval;

    msg.msg_name     = sockaddr;
    msg.msg_namelen  = sockaddr_len;
    msg.msg_control  = NULL;
    msg.msg_controllen = 0;
    msg.msg_flags    = 0;

    kvec[0].iov_base = (void*) buffer;
    kvec[0].iov_len = buffer_size;

    retval = kernel_sendmsg(sock, &msg, kvec, 1, kvec[0].iov_len);

    return retval;
}

static int ziqe_recvmsg (struct socket *sock, ZqKernelAddress buffer,
                         ZqSizeType buffer_size, struct sockaddr *sockaddr,
                         ZqSizeType *sockaddr_len) {
    struct msghdr msg;
    struct kvec kvec[1];
    int retval;

    msg.msg_name     = sockaddr;
    msg.msg_namelen  = (sockaddr_len ? *sockaddr_len : 0);
    msg.msg_control  = NULL;
    msg.msg_controllen = 0;
    msg.msg_flags    = 0;

    kvec[0].iov_base = buffer;
    kvec[0].iov_len = buffer_size;

    retval = kernel_recvmsg(sock, &msg, kvec, 1, kvec[0].iov_len, 0);

    if (sockaddr_len)
        *sockaddr_len = msg.msg_namelen;

    return retval;
}

ZqBool ZqSocketSetOption(ZqSocket zqsocket,
                         ZqSocketOptionLevel level,
                         ZqSocketOptionName name,
                         ZqConstKernelAddress optionValue,
                         ZqSizeType optionSize)
{
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int ret;

    ret = kernel_setsockopt(sock, level, name, (void*)optionValue, optionSize);

    if (ret)
        return ZQ_FALSE;
    else
        return ZQ_TRUE;
}

ZqSocket ZqSocketAccept(ZqSocket zqsocket, ZqSocketAddress *maybeSocketAddress) {
    struct socket *server_sock = zqsocket_to_socket (zqsocket);
    struct socket *client_socket;
    int ret;

    ret = kernel_accept (server_sock, &client_socket, 0);

    if (ret)
        return ZQ_INVALID_SOCKET;

    // The user want this socket's address.
    if (maybeSocketAddress != NULL) {
        int socklen = 0;

        kernel_getsockname (client_socket, (struct sockaddr *) &maybeSocketAddress->in, &socklen);

        maybeSocketAddress->socklen = (ZqSizeType) socklen;
        maybeSocketAddress->socket_family = ((struct sockaddr *)&maybeSocketAddress->in)->sa_family;
    }

    return (ZqSocket)client_socket;
}
