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

#include <asm/ioctls.h>
#include <linux/ioctl.h>
#include <linux/net.h>

#include "CppCore/Socket.h"

#define zqsocket_to_socket(zqsocket) ((struct socket *) zqsocket)

static int ziqe_sendmsg (struct socket *sock, ZqConstKernelAddress buffer, ZqSizeType buffer_size,
                         struct sockaddr *sockaddr, ZqSizeType sockaddr_len);
static int ziqe_recvmsg (struct socket *sock, ZqKernelAddress buffer,
                         ZqSizeType buffer_size, struct sockaddr *sockaddr,
                         ZqSizeType *sockaddr_len);

static ZqSizeType get_next_packet_size (struct socket *sock) {
    ZqSizeType packet_size = 0;

    if (kernel_sock_ioctl (sock, SIOCINQ, (unsigned long)&packet_size) < 0)
        BUG ();

    return packet_size;
}

ZqSocket ZqSocketOpen(ZqSocketFamily family,
                      ZqSocketType type,
                      ZqSocketProtocol protocol) {
    struct socket *sock;
    int retval;

    // Create a non-kernel socket. I may do some security problems
    // But the user should not have access to this socket anyway.
    // (it has no fd).
    retval = sock_create (family, type, protocol, &sock);
    if (retval)
        return ZQ_INVALID_SOCKET;
    else
        return (ZqSocket) sock;
}

void ZqSocketClose(ZqSocket zqsocket) {
    struct socket *sock = zqsocket_to_socket (zqsocket);

    sock_release(sock);
}

ZqError ZqSocketListen(ZqSocket zqsocket,
                      ZqSizeType backlog)
{
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int retval;

    retval = kernel_listen (sock, backlog);
    if (retval)
        return retval;

    return ZQ_E_OK;
}

ZqError ZqSocketBind(ZqSocket zqsocket, const ZqSocketAddress *sockaddr) {
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int retval;

    retval = kernel_bind(sock, (struct sockaddr *) &sockaddr->in,
                         sockaddr->socklen);
    if (retval)
        return retval;

    return ZQ_E_OK;
}


ZqError ZqSocketConnect(ZqSocket zqsocket, const ZqSocketAddress *sockaddr) {
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int retval;

    retval = kernel_connect(sock, (struct sockaddr *) &sockaddr->in,
                           sockaddr->socklen, 0);

    if (retval)
        return retval;
    else
        return ZQ_E_OK;
}

ZqError ZqSocketReceive(ZqSocket zqsocket, ZqKernelAddress *pbuffer, ZqSizeType *bytesReceived) {
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int ret;
    ZqSizeType bufferSize = get_next_packet_size (sock);
    ZqKernelAddress packet_buffer;

    if (bufferSize != 0)
        packet_buffer = ZQ_SYMBOL(ZqAllocate) (bufferSize);
    else
        packet_buffer = NULL;

    ret = ziqe_recvmsg (sock, packet_buffer, bufferSize, NULL, 0);
    if (ret < 0) {
        return ret;
    }

    if (bytesReceived)
        *bytesReceived = (ZqSizeType) ret;

    *pbuffer = packet_buffer;

    return ZQ_E_OK;
}

ZqError ZqSocketSend(ZqSocket zqsocket, ZqConstKernelAddress buffer, ZqSizeType bufferSize, ZqSizeType *bytesSent) {
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int ret;

    ret = ziqe_sendmsg (sock, buffer, bufferSize, NULL, 0);
    if (ret < 0)
        return ret;

    if (bytesSent)
        *bytesSent = (ZqSizeType) ret;

    return ZQ_E_OK;
}

ZqError ZqSocketReceiveFrom(ZqSocket zqsocket, ZqKernelAddress *pbuffer, ZqSocketAddress *sockaddr,
                           ZqSizeType *bytesReceived) {
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int ret;
    ZqSizeType socket_length;

    ZqSizeType bufferSize = get_next_packet_size (sock);
    ZqKernelAddress packet_buffer;

    if (bufferSize != 0)
        packet_buffer = ZQ_SYMBOL(ZqAllocate) (bufferSize);
    else
        packet_buffer = NULL;

    ret = ziqe_recvmsg (sock, packet_buffer,
                        bufferSize, (struct sockaddr *) &sockaddr->in,
                        &socket_length);
    if (ret < 0)
        return ret;

    sockaddr->socklen = socket_length;

    if (bytesReceived)
        *bytesReceived = (ZqSizeType) ret;

    *pbuffer = packet_buffer;

    return ZQ_E_OK;
}

ZqError ZqSocketSendTo(ZqSocket zqsocket, ZqConstKernelAddress buffer,
                      ZqSizeType bufferSize, const ZqSocketAddress *sockaddr,
                      ZqSizeType *bytesSent) {
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int ret;

    ret = ziqe_sendmsg (sock, buffer,
                        bufferSize, (struct sockaddr *) &sockaddr->in,
                        sockaddr->socklen);
    if (ret < 0)
        return ret;

    if (bytesSent)
        *bytesSent = (ZqSizeType) ret;

    return ZQ_E_OK;
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

    // Don't send us SIGPIPE on send failure, only return EPIPE.
    msg.msg_flags    = MSG_NOSIGNAL;

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

ZqError ZqSocketSetOption(ZqSocket zqsocket,
                         ZqSocketOptionLevel level,
                         ZqSocketOptionName name,
                         ZqConstKernelAddress optionValue,
                         ZqSizeType optionSize)
{
    struct socket *sock = zqsocket_to_socket (zqsocket);
    int ret;

    ret = kernel_setsockopt(sock, level, name, (void*)optionValue, optionSize);

    if (ret)
        return ret;
    else
        return ZQ_E_OK;
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
