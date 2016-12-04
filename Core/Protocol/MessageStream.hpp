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

#include "Common/Types.hpp"
#include "Protocol/Message.hpp"
#include "Protocol/MemoryRevision.hpp"

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

    // TODO: move these functions to a factory class ({Tcp,Udp}MessageStreamFactory)
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
    typedef Net::Stream::InputStreamVector InputDataType;

    typedef Net::Stream::InputStreamVector InputStreamType;
    typedef Net::Stream::OutputStreamVector OutputStreamType;

    /**
     * @brief MessageFieldReader  FieldReader used in this stream.
     */
    typedef Base::LittleEndianFieldReader<InputStreamType, Base::UniquePointer<InputStreamType>> MessageFieldReader;
    typedef Base::LittleEndianFieldWriter<OutputStreamType, Base::UniquePointer<OutputStreamType>> MessageFieldWriter;

//    typedef Base::ExtendedVectorWritable<Net::Stream::OutputStreamVector> MessageType;

    enum class ReceiveMessageError {
        Other
    };

    Base::Expected<Base::Pair<Message::Type, Base::RawPointer<MessageFieldReader>>, ReceiveMessageError> receiveMessage();

    template<class MessageType>
    void sendMessage(const MessageType &messageData) {
        messageData.writeToWriter (mWriter);
        mWriter.getVector ().sync ();
    }

    Base::Pair<Address, Port>
    getInfo () const
    {
        return mStream->getStreamInfo ();
    }

private:
    Base::UniquePointer<Net::Stream> mStream;

    MessageFieldWriter mWriter;
    MessageFieldReader mReader;
};

} // namespace Ziqe
} // namespace Protocol

#endif // ZIQE_MESSAGESTREAM_H
