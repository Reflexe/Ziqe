/**
 * @file MessageStream.hpp
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
#ifndef ZIQE_MESSAGESTREAM_H
#define ZIQE_MESSAGESTREAM_H

#include "Base/FieldWriter.hpp"

#include "Network/NetworkProtocol.hpp"

#include "Core/Types.hpp"
#include "Core/ZiqeProtocol/Message.hpp"
#include "Core/ZiqeProtocol/MemoryRevision.hpp"

namespace Ziqe {

class MessageStream : private Net::NetworkProtocol::Callback
{
public:
    MessageStream();
    virtual ~MessageStream() = default;
    ALLOW_COPY_AND_MOVE (MessageStream)

protected:
    typedef Net::NetworkPacket::DataType DataType;

    void receiveMessage (Net::NetworkProtocol &networkProtocol)
    {
        networkProtocol.callbackReceivePacket (*this);
    }

    virtual void onMessageReceived (const Message &type, DataType &vector,
                                    Net::NetworkProtocol &protocol) = 0;

    Base::UniquePointer<Net::NetworkPacket> pCurrentPacket;

private:
    void onPacketReceived (Net::NetworkProtocol &protocol,
                           Base::UniquePointer<Net::NetworkPacket> &&packet) override;
};

} // namespace Ziqe

#endif // ZIQE_MESSAGESTREAM_H
