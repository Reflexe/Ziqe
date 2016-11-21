/**
 * @file UdpStream.cpp
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
#include "UdpStream.hpp"

namespace Ziqe {
namespace Net {

/**
   @brief
   TODO: A shared max data size.
 */

Base::Pair<Stream::Address, Stream::Port> UdpStream::getStreamInfo() const
{
    return mAddressAndPort;
}

UdpStream::UdpStream(Base::Socket &&socket)
    : mSocket{Base::move (socket)}
{
}

UdpStream::~UdpStream()
{
}

void UdpStream::setBroadcast(bool isBroadcast)
{
    int integerIsBroadcast = isBroadcast;

    // Make this socket a broadcast socket.
    mSocket.setSocketOption(Base::Socket::OptionLevel::Socket,
                            Base::Socket::OptionName::Broadcast,
                            integerIsBroadcast);
}

Base::Expected<Base::Vector<uint8_t>,Base::Socket::ReceiveError> UdpStream::receiveRawPacket()
{
    return mSocket.receive ();
}

SizeType UdpStream::sendRawPacket(const Base::RawArray<const uint8_t> &array)
{
    return mSocket.send (array);
}

Base::Expected<UdpStream, UdpStream::CreateError>
UdpStream::Connect(const UdpStream::Address &address, UdpStream::Port port) {
    auto maybeUdpSocket = Base::Socket::Connect (Base::Socket::SocketAddress::CreateIn6 (address, port),
                                                 Base::Socket::Type::Datagram);

    if (! maybeUdpSocket)
        return {CreateError::Other};

    return {Base::move (*maybeUdpSocket)};
}

Base::Expected<UdpStream, UdpStream::CreateError>
UdpStream::CreateBroadcast(const Stream::Address &address, const Stream::Port &port) {
    auto maybeUdpStream = Connect (address, port);

    if (! maybeUdpStream)
        return maybeUdpStream;

    maybeUdpStream->setBroadcast (true);

    return maybeUdpStream;
}

UdpStream::UdpOutputVector::UdpOutputVector(UdpStream &stream, bool isCreateConnection)
    : mStream{stream},
      mNextMessage{(isCreateConnection ? UdpMessage::Flags::SYN : UdpMessage::Flags::None), 0}
{
    createNewSegment ();
}

void UdpStream::UdpOutputVector::createNewSegment() {
    mVector.setEnd (0);
    mVector.setBegin (0);

    // Write the message to the vector.
    mNextMessage.writeTo (mVector);

    // Advance the message number for the next one.
    mNextMessage.setMessageNumber (mNextMessage.getMessageNumber () + 1);
}

void UdpStream::UdpOutputVector::sendCurrentSegment() {
    // It is the end of the stream (sync has been called), send LastFragment stream.
    if (mAutoSendDataSize > mVector.getVector ().size ()) {
        mNextMessage.setLastFragmentFlag (true);
        mNextMessage.setMessageNumber (mNextMessage.getMessageNumber () - 1);

        // Rewrite the previous message with  the last fragment flag and clear the SYN flag.
        auto vectorBegin = mVector.getIndexBegin ();
        mVector.setBegin (0);
        mNextMessage.writeTo (mVector);
        mVector.setBegin (vectorBegin);

        mNextMessage.setLastFragmentFlag (false);
    }

    mStream.sendRawPacket (mVector.getVector ().toRawArray ());

    createNewSegment ();
}

UdpStream::UdpInputVector::UdpInputVector(UdpStream &stream)
    : mStream{stream}
{
}

Base::Expected<Stream::DataType, Stream::ReceiveError> UdpStream::UdpInputVector::receiveData() const {
    auto maybeData = mStream.receiveRawPacket ();

    if (! maybeData)
        return {socketReceiveErrorToError (maybeData.getError ())};

    UdpMessage::InputExtendedVectorType vector{*maybeData};
    auto maybeMessage = UdpMessage::ReadFrom (vector);

    if (! maybeMessage) {
        ZQ_LOG ("Invalid message received");

        return {ReceiveError::ParseError};
    }

    return {std::move (*maybeData)};
}

} // namespace Net
} // namespace Ziqe
