/**
 * @file UdpServer.hpp
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
#ifndef ZIQE_NET_UDPSERVER_HPP
#define ZIQE_NET_UDPSERVER_HPP

#include "Base/Socket.hpp"

#include "Network/Server.hpp"
#include "Network/UdpStream.hpp"

namespace Ziqe {
namespace Net {

class UdpServer : implements public Server
{
public:
    UdpServer(Base::Socket &&listeingSocket);

    enum class ListenError {
        Other
    };

    static Base::Expected<UdpServer,ListenError> Listen(const Address &address, const Port &port);

    static UdpServer CreateFromStream (UdpStream &&udpStream);

    /**
       @brief
       @return
       @note Doesn't return a UdpStream!
     */
    virtual Base::UniquePointer<Stream> acceptClient () override;

private:
    Base::SharedPointer<Base::Socket> mSocket;

};

} // namespace Net
} // namespace Ziqe

#endif // ZIQE_NET_UDPSERVER_HPP
