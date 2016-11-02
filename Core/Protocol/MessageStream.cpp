/**
 * @file MessageStream.cpp
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
#include "MessageStream.hpp"

#include "Base/FieldReader.hpp"

namespace Ziqe {
namespace Protocol {

MessageStream::MessageStream(Base::UniquePointer<Net::Stream> &&stream)
    : mStream{Base::move(stream)}
{
}

Base::Expected<MessageStream, MessageStream::CreateError>
MessageStream::CreateUDPConnection(const MessageStream::Address &address,
                                   const MessageStream::Port &port)
{
    auto maybeUdpStream = Net::UdpStream::Connect (address, port);

    if (! maybeUdpStream)
        return CreateError::Other;
    else
        return {Base::makeUnique<Net::UdpStream>(Base::move (*maybeUdpStream))};
}

Base::Expected<MessageStream, MessageStream::CreateError>
MessageStream::CreateGlobalUDPConnection(const MessageStream::Address &broadcastAddress, const MessageStream::Port &port) {
    auto maybeUdpStream = Net::UdpStream::CreateBroadcast (broadcastAddress, port);

    if (! maybeUdpStream)
        return CreateError::Other;
    else
        return {Base::makeUnique<Net::UdpStream>(Base::move (*maybeUdpStream))};
}

Base::Expected<MessageStream,MessageStream::CreateError>
MessageStream::CreateConnection(const Address &address, const Port &port)
{
    auto maybeStream = Net::UdpStream::create (address, port);

    if (! maybeStream)
        return {maybeStream.getError ()};
    else
        return {Base::move (*maybeStream)};
}

Base::Expected<MessageStream, MessageStream::CreateError>
MessageStream::CreateGlobalConnection(const Address &croadcastAddress,
                                      const Port &port) {
    auto maybeBroadcastStream = StreamType::createBroadcast (croadcastAddress, port);

    if (! maybeBroadcastStream)
        return {maybeBroadcastStream.getError ()};
    else
        return {Base::move (*maybeBroadcastStream)};
}

MessageStream::~MessageStream()
{
}

Base::Expected<Base::Pair<Message, MessageStream::MessageFieldReader>, int> MessageStream::receiveMessage() const{
    auto vector = mStream->getStreamVector ();

    MessageFieldReader reader{Base::move(vector)};
    if (! reader.canReadT<Message::Type>()) {
        DEBUG_CHECK_REPORT_NOT_REACHED ("Message too short");
        return;
    }

    Message::Type type = static_cast<Message::Type>(reader.readT <uint16_t> ());

    if (! Message::isValidMessageType (type)) {
        DEBUG_CHECK_REPORT_NOT_REACHED ("Invalid message type received");

        return;
    }

    return {Message{type}, InitializerDelimeter{}, Base::move (reader)};
}

} // namespace Ziqe
} // namespace Protocol
