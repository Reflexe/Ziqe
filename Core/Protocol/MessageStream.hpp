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

#include "Base/FieldReader.hpp"

#include "Network/Stream.hpp"

#include "Core/Types.hpp"
#include "Core/Protocol/Message.hpp"
#include "Core/Protocol/MemoryRevision.hpp"

namespace Ziqe {
namespace Protocol {

class MessageStream
{
public:
    MessageStream(Base::UniquePointer<Net::Stream> &&stream);
    ZQ_ALLOW_COPY_AND_MOVE (MessageStream)

    /**
    * @brief DataType  The type used to transform received data.
    */
    typedef Net::Stream::StreamVector InputDataType;

    /**
     * @brief OutputDataType  Used to send data.
     */
    typedef Base::Vector<uint8_t> OutputDataType;

    /**
     * @brief MessageFieldReader  FieldReader used in this stream.
     */
    typedef Base::LittleEndianFieldReader<InputDataType> MessageFieldReader;

    struct Callback {
        Callback() = default;
        virtual ~Callback() = default;
        ZQ_ALLOW_COPY_AND_MOVE (Callback)

        typedef MessageFieldReader MessageFieldReader;

        virtual void onMessageReceived (const Message &type,
                                        MessageFieldReader &fieldReader,
                                        const MessageStream &messageStream) = 0;
    };

    /**
     * @brief receiveMessage  Receive one message and call the callback.
     * @param stream
     */
    void receiveMessage (Callback &callback) const{
        auto vector = mStream->getStreamVector ();
        MessageFieldReader reader{Base::move(vector)};
        Message::Type type = static_cast<Message::Type>(reader.readT <uint16_t> ());

        if (! Message::isValidMessageType (type)) {
            // TODO: logging - Log this incident.

            return;
        }

        return callback.onMessageReceived (type, reader, *this);
    }

    void sendMessage(const OutputDataType &messageData) const
    {
        mStream->send (messageData);
    }

private:
    Base::UniquePointer<Net::Stream> mStream;
};

} // namespace Ziqe
} // namespace Protocol

#endif // ZIQE_MESSAGESTREAM_H
