/**
 * @file SocketConfig.gen
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
#ifndef LINUX_SOCKETCONFIG_GEN_H
#define LINUX_SOCKETCONFIG_GEN_H

#ifdef _LINUX
// struct sockaddr_storage, AF_*, SOCK_*, SO_*, SOL_*
#include <uapi/linux/socket.h>

// IPPROTO_*
#include <uapi/linux/ip.h>

// sockaddr_in[6]
#include <uapi/linux/in.h>
#include <uapi/linux/in6.h>
#include <asm-generic/errno.h>
#else
// This headers are "C++ Safe".

// struct sockaddr_storage, AF_*, SOCK_*, SO_*, SOL_*
#include <sys/socket.h>

// IPPROTO_*
// sockaddr_in[6]
#include <netinet/in.h>
#include <netinet/ip6.h>
#include <netinet/ip.h>
#include <sys/errno.h>
#endif

typedef ZqKernelAddress ZqSocket;
#define ZQ_INVALID_SOCKET (NULL)

typedef int ZqSocketFamily;
typedef int ZqSocketType;
typedef int ZqSocketProtocol;
typedef int ZqSocketOptionLevel;
typedef int ZqSocketOptionName;

typedef struct in_addr ZqIpv4Address;
typedef struct in6_addr ZqIpv6Address;
typedef uint16_t ZqPort;

#define ZQ_AF_INET AF_INET
#define ZQ_AF_INET6 AF_INET6
#define ZQ_AF_UNSPEC AF_UNSPEC

#define ZQ_SOCKET_TYPE_STREAM SOCK_STREAM
#define ZQ_SOCKET_TYPE_DGRAM SOCK_DGRAM

#define ZQ_OPTION_BROADCAST SO_BROADCAST
#define ZQ_LEVEL_SOCKET SOL_SOCKET

#define ZQ_INADDR_ANY INADDR_ANY
#define ZQ_INADDR6_ANY IN6ADDR_ANY_INIT
#define ZQ_PORT_ANY (0)

typedef struct __kernel_sockaddr_storage Zq_sockaddr_storage;
typedef struct sockaddr_in Zq_sockaddr_in;
typedef struct sockaddr_in6 Zq_sockaddr_in6;

typedef struct in_addr Zq_in_addr;
typedef struct in6_addr Zq_in6_addr;

/* Error types & values */
typedef int ZqError;

#define ZQ_E_AGAIN EAGAIN
#define ZQ_E_CON_RESET ECONNRESET
#define ZQ_E_DEST_REQUIRED EDESTADDRREQ
#define ZQ_E_MEM_FAULT EFAULT
#define ZQ_E_INVALID_ARG EINVAL
#define ZQ_E_SIZE EMSGSIZE
#define ZQ_E_OK 0

#endif // LINUX_SOCKETCONFIG_GEN_H
