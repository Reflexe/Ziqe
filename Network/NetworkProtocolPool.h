/**
 * @file NetworkProtocolPool.h
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
#ifndef ZIQE_NETWORKPROTOCOLPOOL_H
#define ZIQE_NETWORKPROTOCOLPOOL_H

#include "Base/LinkedList.h"

#include "Network/NetworkProtocol.h"

namespace Ziqe {

class NetworkProtocolPool
{
public:
    typedef LinkedList<NetworkProtocol> ProtocolsType;

    struct Callback {
        Callback () = default;
        ALLOW_COPY_AND_MOVE (Callback)
        virtual ~Callback () = 0;

        virtual void onPacketReceived (const SharedPointer<NetworkProtocol> &protocol,
                                       UniquePointer<NetworkPacket> &&packet) = 0;
    };

    NetworkProtocolPool();

    template<class ...Args>
    NetworkProtocolPool(Args &&...args)
    {
        ExpandArgs (mProtocols.emplace_back (std::forward<Args>(args))...);
    }

    ProtocolsType::Iterator addProtocol (UniquePointer<NetworkProtocol> &&protocol)
    {
        return mProtocols.emplace_back (std::move (protocol));
    }

    void run (Callback &callback);

    void sendData (SharedVector<Byte> &&data) {
        auto iterator = mProtocols.begin ();
        auto end = --mProtocols.end ();

        for (; iterator != end; ++iterator)
        {
            iterator->sendData (copy (data));
        }

        end->sendData (std::move (data));
    }

private:
    ProtocolsType mProtocols;
};

} // namespace Ziqe

#endif // ZIQE_NETWORKPROTOCOLPOOL_H
