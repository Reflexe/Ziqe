/**
 * @file TcpServer.cpp
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
#ifndef ZIQE_NET_TCPSERVER_HPP
#define ZIQE_NET_TCPSERVER_HPP

#include "Base/Socket.hpp"

#include "Network/Server.hpp"

namespace Ziqe {
namespace Net {

class TcpServer : public Server
{
public:
    TcpServer(Base::Socket &&socket);

    enum class ListenError {
        Other
    };

    static Base::Expected<TcpServer, ListenError> Listen (const Address &address,
                                                          const Port &port);

    Base::UniquePointer<Stream> acceptClient () override;

private:    
    Base::Socket mSocket;

};

} // namespace Net
} // namespace Ziqe

#endif // ZIQE_NET_TCPSERVER_HPP
