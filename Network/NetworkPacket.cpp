/**
 * @file NetworkPacket.cpp
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
#include "NetworkPacket.hpp"

namespace Ziqe {
namespace Net {

NetworkPacket::NetworkPacket()
{

}

NetworkPacket::PacketInfo::PacketInfo(ZqIpv4Address address, uint16_t transportPort)
{
    mSockaddr.socklen = sizeof (mSockaddr.in);

    mSockaddr.in.sin_family = static_cast<ZqSocketFamily>(Type::IPv4);
    mSockaddr.in.sin_addr = address;
    mSockaddr.in.sin_port = transportPort;
}

NetworkPacket::PacketInfo::PacketInfo(ZqIpv6Address address, uint16_t transportPort)
{
    mSockaddr.socklen = sizeof (mSockaddr.in6);

    mSockaddr.in6.sin_family = static_cast<ZqSocketFamily>(Type::IPv6);
    mSockaddr.in6.sin6_addr = address;
    mSockaddr.in6.sin6_port = transportPort;

    mSockaddr.in6.sin6_scope_id = 0;
    mSockaddr.in6.sin6_flowinfo = 0;
}

NetworkPacket::PacketInfo::PacketInfo(ZqSocketAddress sockaddr)
    : mSockaddr{sockaddr}
{
}

bool NetworkPacket::PacketInfo::operator !=(const NetworkPacket::PacketInfo &info) {
    return (mSockaddr.socklen != info.mSockaddr.socklen)
            || (memcmp (static_cast<const void*>(&info.mSockaddr),
                        static_cast<const void*>(&mSockaddr),
                        mSockaddr.socklen) != 0);
}

} // namespace Net
} // namespace Ziqe
