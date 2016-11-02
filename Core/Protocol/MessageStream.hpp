/**
 * @file MessageStream.hpp
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
#ifndef ZIQE_MESSAGESTREAM_H
#define ZIQE_MESSAGESTREAM_H

#include "Base/FieldReader.hpp"

#include "Network/UdpStream.hpp"
#include "Network/TcpStream.hpp"

#include "Core/Types.hpp"
#include "Core/Protocol/Message.hpp"
#include "Core/Protocol/MemoryRevision.hpp"

namespace Ziqe {
namespace Protocol {

/**
   @brief  A stream to send and receive Ziqe protocol messages.
 */
class MessageStream
{
public:
    typedef typename Net::Stream::Port Port;
    typedef typename Net::Stream::Address Address;

    enum class CreateError {
        Other
    };

    MessageStream(Base::UniquePointer<Net::Stream> &&stream);

    static Base::Expected<MessageStream,CreateError>
    CreateTCPConnection(const Address &address, const Port &port);

    static Base::Expected<MessageStream,CreateError>
    CreateGlobalTCPConnection(const Address &address, const Port &port);

    static Base::Expected<MessageStream,CreateError>
    CreateUDPConnection (const Address &address, const Port &port);

    static Base::Expected<MessageStream,CreateError>
    CreateGlobalUDPConnection (const Address &broadcastAddress, const Port &port);

    ZQ_ALLOW_COPY_AND_MOVE (MessageStream)
    virtual ~MessageStream();

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

    /**
     * @brief receiveMessage  Receive one message and call the callback.
     * @param stream
     */
    Base::Expected<Base::Pair<Message, MessageFieldReader>, int> receiveMessage() const;

    void sendMessage(const OutputDataType &messageData) const
    {
        mStream->send (messageData);
    }

    void retransmitLastMessage()
    {
        mStream->send (mLastOutput);
    }

private:
    OutputDataType mLastOutput;

    Base::UniquePointer<Net::Stream> mStream;

};

} // namespace Ziqe
} // namespace Protocol

#endif // ZIQE_MESSAGESTREAM_H
