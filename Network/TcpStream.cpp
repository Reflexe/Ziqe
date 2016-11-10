/**
 * @file TcpStream.cpp
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
#include "TcpStream.hpp"

namespace Ziqe {
namespace Net {

TcpStream::TcpStream(Base::Socket &&readySocket, Base::Pair<Address, Port> &&addressAndPort)
    : mSocket{std::move (readySocket)}, mStreamAddressAndPort{addressAndPort}
{
}

Base::Expected<TcpStream, TcpStream::ConnectError>
TcpStream::Connect(const TcpStream::Address &address, const Port &port) {
    auto maybeConnectedSocket = Base::Socket::Connect (Base::Socket::SocketAddress::CreateIn6 (address, port), Base::Socket::Type::Stream);

    if (! maybeConnectedSocket)
        return {Base::move (maybeConnectedSocket.getError ())};

    return {std::move (maybeConnectedSocket.get ()), Base::Pair<Address, Port>{address, port}};
}

Base::Expected<Stream::DataType, Stream::ReceiveError> TcpStream::receive() const{
    auto expectedReceivedData = mSocket.receive ();

    // If we received an error, convert the error to a stream error and return it.
    if (! expectedReceivedData)
        return {socketReceiveErrorToError (expectedReceivedData.getError ())};
    else
        return {Base::move (expectedReceivedData.get ())};
}

void TcpStream::send(const Stream::DataType &data) const
{
    mSocket.send (data.toRawArray ());
}

Base::Pair<Stream::Address, Stream::Port> TcpStream::getStreamInfo() const
{
    return mStreamAddressAndPort;
}

} // namespace Net
} // namespace Ziqe
