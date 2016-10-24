/**
 * @file TcpStream.hpp
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
#ifndef ZIQE_NET_TCPSTREAM_HPP
#define ZIQE_NET_TCPSTREAM_HPP

#include "Network/Stream.hpp"

#include "Base/Socket.hpp"

namespace Ziqe {
namespace Net {

class TcpStream final : implements public Stream
{
public:
    typedef Zq_in6_addr Address;
    typedef ZqPort Port;

    ZQ_ALLOW_COPY_AND_MOVE (TcpStream)

    static TcpStream Connect(const Address &address, Port port);
    static TcpStream Listen(Port port, SizeType backlog=0, const Address &address=ZQ_INADDR6_ANY);

    virtual DataType receive() const override ;

    virtual void send(const DataType &data) const override ;

    TcpStream acceptConnection();

private:
    // connect constructor.
    TcpStream(Port port, const Address &address);

    // listen constructor.
    TcpStream(const Address &address, Port port, SizeType backlog);

    Base::Socket mSocket;

};

} // namespace Net
} // namespace Ziqe

#endif // ZIQE_NET_TCPSTREAM_HPP
