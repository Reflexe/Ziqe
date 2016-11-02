/**
 * @file TcpServer.hpp
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
#include "TcpServer.hpp"
#include "TcpStream.hpp"

namespace Ziqe {
namespace Net {

TcpServer::TcpServer(Base::Socket &&socket)
    : mSocket{Base::move (socket)}
{

}

Base::Expected<TcpServer, TcpServer::ListenError> TcpServer::Listen(const Server::Address &address,
                                                                    const Server::Port &port)
{
    auto maybeListeningSocket = Base::Socket::Listen (Base::Socket::SocketAddress::CreateIn6 (address, port),
                                                      Base::Socket::Type::Stream, ZQ_NO_BACKLOG);

    if (! maybeListeningSocket)
        return ListenError::Other;
    else
        return {Base::move (*maybeListeningSocket)};
}

Base::UniquePointer<Stream> TcpServer::acceptClient() {
    auto maybeNewSocket = mSocket.accept ();

    if (! maybeNewSocket)
        return {};
    else
        return Base::makeUnique<TcpStream>(Base::move (*maybeNewSocket));
}

} // namespace Net
} // namespace Ziqe
