/**
 * @file NetworkPacket.hpp
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
#ifndef ZIQE_NETWORKPACKET_H
#define ZIQE_NETWORKPACKET_H

#include "Base/ExtendedVector.hpp"
#include "Base/HashTable.hpp"
#include "Base/Types.hpp"

#include "ZiqeAPI/Socket.h"

namespace Ziqe {
namespace Net {

class NetworkPacket
{
public:
    // Use extended vector to enable more flexability and less
    // copy & free.
    typedef Base::ExtendedVector<uint8_t> DataType;

    struct PacketInfo {
        PacketInfo(ZqIpv4Address address,
                   uint16_t transportPort);

        PacketInfo(ZqIpv6Address address,
                   uint16_t transportPort);

        PacketInfo(ZqSocketAddress sockaddr);

        enum class Type : ZqSocketFamily {
            IPv4 = ZQ_AF_INET,
            IPv6 = ZQ_AF_INET6,
        };

        ZQ_ALLOW_COPY_AND_MOVE (PacketInfo)
        friend struct Base::Hash<Net::NetworkPacket::PacketInfo>;

        bool operator != (const PacketInfo &info);

        const ZqSocketAddress &getSockaddr () const
        {
            return mSockaddr;
        }
        
    private:
        ZqSocketAddress mSockaddr;
    };

    NetworkPacket();

    DataType &getData ()
    {
        return mData;
    }

    ZQ_DEFINE_CONST_AND_NON_CONST (const PacketInfo&, PacketInfo&, getInfo, (), { return mPacketInfo; })

private:
    DataType mData;
    PacketInfo mPacketInfo;
};

} // namespace Net

namespace Base {
template<>
struct Hash<Net::NetworkPacket::PacketInfo>
{
    SizeType operator() (const Net::NetworkPacket::PacketInfo &info)
    {
        DEBUG_CHECK ((sizeof (info.mSockaddr)-sizeof (info.mSockaddr.socklen)) >= info.mSockaddr.socklen);
        return mHash (Base::makeBytesArray (info.mSockaddr.in, info.mSockaddr.socklen));
    }

private:
    Hash<RawArray<const uint8_t>> mHash;
};
} // namespace Base

} // namespace Ziqe

#endif // ZIQE_NETWORKPACKET_H
