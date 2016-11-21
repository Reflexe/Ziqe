/**
 * @file TcpStream.hpp
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
#ifndef ZIQE_NET_TCPSTREAM_HPP
#define ZIQE_NET_TCPSTREAM_HPP

#include "Network/Stream.hpp"

#include "Base/Socket.hpp"

namespace Ziqe {
namespace Net {

class TcpStream final : implements public Stream
{
public:
    ZQ_ALLOW_COPY_AND_MOVE (TcpStream)

    typedef Base::Socket::ListenError ListenError;
    typedef Base::Socket::ConnectError ConnectError;

    static Base::Expected<TcpStream,ConnectError> Connect(const Address &address, const Port &port);

    virtual Base::Pair<Address, Port> getStreamInfo () const override;

    TcpStream(Base::Socket &&readySocket, Base::Pair<Address, Port> &&addressAndPort);

    virtual Base::UniquePointer<InputStreamVector> getInputStreamVector() const override;

    virtual Base::UniquePointer<OutputStreamVector> getOutputStreamVector() const override;

private:
    class TcpInputStreamVector : public InputStreamVector {
        TcpInputStreamVector(TcpStream &stream);

        Base::Expected<DataType, ReceiveError> receiveData () const override;

    private:
        TcpStream &mStream;

    };

    class TcpOutputStreamVector : public OutputStreamVector {
        TcpOutputStreamVector(TcpStream &stream);

        void sendCurrentSegment () override;

    private:
        TcpStream &mStream;

    };

    Base::Expected<DataType, ReceiveError> receiveRawPacket() const;

    void sendRawPacket(const DataType &data) const
    {
        mSocket.send (data.toRawArray ());
    }

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

    Base::Pair<Address, Port> mStreamAddressAndPort;
};

} // namespace Net
} // namespace Ziqe

#endif // ZIQE_NET_TCPSTREAM_HPP
