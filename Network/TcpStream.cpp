/**
 * @file TcpStream.cpp
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
#include "TcpStream.hpp"

namespace Ziqe {
namespace Net {

TcpStream::TcpStream(Port port, const TcpStream::Address &address)
    : mSocket{Base::Socket::Connect (Base::Socket::SocketAddress::CreateIn6 (address, port), ZQ_SOCKET_TYPE_STREAM)}
{
}

TcpStream::TcpStream(const TcpStream::Address &address, TcpStream::Port port, SizeType backlog)
    : mSocket{Base::Socket::Listen (Base::Socket::SocketAddress::CreateIn6 (address, port), ZQ_SOCKET_TYPE_STREAM, backlog)}
{
}

TcpStream TcpStream::Connect(const TcpStream::Address &address, TcpStream::Port port)
{
    return TcpStream{port, address};
}

TcpStream TcpStream::Listen(TcpStream::Port port, SizeType backlog, const TcpStream::Address &address)
{
    return TcpStream{address, port, backlog};
}

Stream::DataType TcpStream::receive() const
{
    return mSocket.receive ();
}

void TcpStream::send(const Stream::DataType &data) const
{
    mSocket.send (data.toRawArray ());
}

TcpStream TcpStream::acceptConnection()
{
    mSocket.Connect ()
}

} // namespace Net
} // namespace Ziqe
