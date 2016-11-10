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

void UdpStream::send(const Stream::DataType &data) const
{
    mSocket.send (data.toRawArray ());
}

Base::Expected<Stream::DataType, Stream::ReceiveError> UdpStream::receive() const{
    auto maybeData = mSocket.receive ();

    if (! maybeData)
        return {socketReceiveErrorToError (maybeData.getError ())};

    return {std::move (*maybeData)};
}

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

Base::Expected<UdpStream, UdpStream::CreateError>
UdpStream::Connect(const UdpStream::Address &address, UdpStream::Port port) {
    auto maybeUdpSocket = Base::Socket::Connect (Base::Socket::SocketAddress::CreateIn6 (address, port),
                                                 Base::Socket::Type::Datagram);

    if (! maybeUdpSocket)
        return {CreateError::Other};

    return {Base::move (*maybeUdpSocket)};
}

Base::Expected<UdpStream, UdpStream::CreateError> UdpStream::CreateBroadcast(const Stream::Address &address, const Stream::Port &port) {
    auto maybeUdpStream = Connect (address, port);

    if (! maybeUdpStream)
        return maybeUdpStream;

    maybeUdpStream->setBroadcast (true);

    return maybeUdpStream;
}

} // namespace Net
} // namespace Ziqe
