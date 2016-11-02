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

#include "Base/Socket.hpp"

namespace Ziqe {
namespace Net {

class UdpStream : implements public Stream
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

    virtual void send (const DataType &data) const override;

    virtual Base::Expected<DataType,ReceiveError> receive () const override;

    /**
       @brief Set whether this Stream is a broadcast stream or not.
     */
    void setBroadcast (bool isBroadcast);

    /**
       @brief Turn a server socket into a client.
       @param otherSideAddress
       @param otherSidePort
     */
    bool turnToClient(const Address &otherSideAddress, const Port &otherSidePort);

private:    
    friend class UdpServer;

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

    Base::Socket mSocket;
};

} // namespace Net
} // namespace Ziqe

#endif // ZIQE_NET_UDPSTREAM_HPP
