/**
 * @file UdpServer.cpp
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
#include "UdpServer.hpp"

#include "Base/Optional.hpp"

namespace Ziqe {
namespace Net {

UdpServer::UdpServer(Base::Socket &&listeingSocket)
    : mSocket{Base::makeShared<Base::Socket>(Base::move (listeingSocket))}
{

}

Base::Expected<UdpServer, UdpServer::ListenError> UdpServer::Listen(const Server::Address &address, const Server::Port &port) {
    auto maybeUdpSocket = Base::Socket::Bind (Base::Socket::SocketAddress::CreateIn6 (address, port),
                                               Base::Socket::Type::Datagram);

     if (! maybeUdpSocket)
         return {ListenError::Other};

     return {std::move (maybeUdpSocket.get ())};

}

UdpServer UdpServer::CreateFromStream(UdpStream &&udpStream)
{
    return UdpServer{Base::move (udpStream.mSocket)};
}

/**
   @brief  A class simulates a udp stream.

   We have a problem with UDP servers' `accept` method: the only way
   to "accept" a new client is by receiving a new packet. In order
   to solve it, we'll receive a packet and hold it in a special
   stream container that will give it to the user on `receive`.
 */
class ServerToClientUdpStream : public UdpStream
{
public:
    ServerToClientUdpStream(Stream::DataType &&data,
                            const Base::Pair<Address,Port> &info);

    virtual Base::Pair<Address,Port> getStreamInfo () const override;

private:
    Base::Socket::SocketAddress mDestSocketAddress;

    Base::SharedPointer<Base::Socket> mSharedSocket;

    DataType mReceiveData;
};

Base::UniquePointer<Stream> UdpServer::acceptClient() {
    auto maybeDataAndSocketAddress = mSocket->receiveWithAddress ();

    if (! maybeDataAndSocketAddress)
        return {};

    // FIXME: a big packets would not work, we should receive the next packet.
    return Base::makeUnique<ServerToClientUdpStream>();
}

void ServerToClientUdpStream::send(const Stream::DataType &data) const
{
    mSharedSocket->sendAllToAddress (mDestSocketAddress, data.toRawArray ());
}

Base::Pair<Stream::Address, Stream::Port> ServerToClientUdpStream::getStreamInfo() const{
    ZQ_ASSERT (mDestSocketAddress.get ().socket_family == ZQ_AF_INET6);

    return {mDestSocketAddress.get ().in6.sin6_addr,
            mDestSocketAddress.get ().in6.sin6_port};
}

ServerToClientUdpStream::ServerToClientUdpStream(const Base::SharedPointer<Base::Socket> &serverSocket, Stream::DataType &&data,
                                                 Base::Socket::SocketAddress &&socketAddress)
    : mDestSocketAddress{Base::move (socketAddress)}, mSharedSocket{serverSocket}, mReceiveData{Base::move (data)}
{
}

Base::Expected<Stream::DataType, Stream::ReceiveError> ServerToClientUdpStream::receive() const{
    ZQ_ASSERT (mReceiveData.size () != 0);

    return {Base::move (mReceiveData)};
}

} // namespace Net
} // namespace Ziqe
