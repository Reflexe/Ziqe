/**
 * @file MessageStream.cpp
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
#include "MessageStream.hpp"

#include "Base/FieldReader.hpp"

namespace Ziqe {

MessageStream::MessageStream()
{
}

void MessageStream::onPacketReceived(Net::NetworkProtocol &protocol,
                                     Base::UniquePointer<Net::NetworkPacket> &&packet) {
    pCurrentPacket = std::move (packet);
    ZQ_UNUSED (protocol);

    Base::LittleEndianFieldReader<Base::ExtendedVector<uint8_t>> reader{Base::ExtendedVector<uint8_t>{std::move (pCurrentPacket->getData ())}};

    if (! reader.canReadT <uint16_t> ()) {
        // TODO: logging.
#if 0
        ZQ_DEBUG ("Dropping invalid message");
#endif

        return;
    }

    Message::Type type = static_cast<Message::Type>(reader.readT <uint16_t> ());

    // TODO: check for valid message type, how should I do that?
    return onMessageReceived (type, reader.getVector ().getVector (),
                              protocol);
}



} // namespace Ziqe
