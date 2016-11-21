/**
 * @file UdpStream.hpp
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
#ifndef ZIQE_NET_UDPSTREAM_HPP
#define ZIQE_NET_UDPSTREAM_HPP

#include "Network/Stream.hpp"
#include "Network/UdpMessage.hpp"

#include "Base/Socket.hpp"

namespace Ziqe {
namespace Net {

/**
   @brief A Udp implementation of Stream.

   this stream is not a raw UDP stream, it sends a few headers before each
   packet. Mainly to allow basic fragmention for big UdpPackets.

 */
class UdpStream final : implements public Stream
{
public:
    enum class CreateError {
        Other
    };

    UdpStream(Base::Socket &&socket);
    virtual ~UdpStream() override;

    ZQ_ALLOW_COPY_AND_MOVE (UdpStream)

    static Base::Expected<UdpStream,CreateError> Connect (const Address &address, Port port);

    static Base::Expected<UdpStream,CreateError> CreateBroadcast (const Address &address, const Port &port);

    virtual Base::Pair<Address, Port> getStreamInfo () const override;

    virtual Base::UniquePointer<InputStreamVector> getInputStreamVector () const override;

    virtual Base::UniquePointer<OutputStreamVector> getOutputStreamVector () const override;

    /**
       @brief Set whether this Stream is a broadcast stream or not.
     */
    void setBroadcast (bool isBroadcast);

private:
    friend class UdpServer;

    Base::Expected<Base::Vector<uint8_t>, Base::Socket::ReceiveError> receiveRawPacket();

    SizeType sendRawPacket (const Base::RawArray<const uint8_t> &array);

    class UdpOutputVector final : public Stream::OutputStreamVector {
        UdpOutputVector(UdpStream &stream, bool isCreateConnection);

    private:
        void createNewSegment ();

        virtual void sendCurrentSegment () override;

        UdpMessage mNextMessage;
        UdpStream &mStream;
    };

    class UdpInputVector : public Stream::InputStreamVector {
        UdpInputVector (UdpStream &stream);

    private:
        virtual Base::Expected<DataType,ReceiveError> receiveData () const override;

    protected:
        UdpStream &mStream;
    };

    static ReceiveError socketReceiveErrorToError(const Base::Socket::ReceiveError &receiveError) {
        using SocketReceiveError=Base::Socket::ReceiveError;

        switch (receiveError) {
        case SocketReceiveError::Disconnected:
        case SocketReceiveError::Other:
            return ReceiveError::Other;
        case SocketReceiveError::Timeout:
            return ReceiveError::Timeout;
        }
    }

    Base::Pair<Address, Port> mAddressAndPort;

    Base::Socket mSocket;
};

} // namespace Net
} // namespace Ziqe

#endif // ZIQE_NET_UDPSTREAM_HPP
