/**
 * @file MessageStreamFactoryInterface.hpp
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
#ifndef ZIQE_MESSAGESTREAMFACTORYINTERFACE_HPP
#define ZIQE_MESSAGESTREAMFACTORYINTERFACE_HPP

#include "Protocol/MessageStream.hpp"
#include "Protocol/MessageServer.hpp"

namespace Ziqe {

class MessageStreamFactoryInterface
{
public:
    typedef Protocol::MessageStream::Address Address;
    typedef Protocol::MessageStream::Port Port;

    MessageStreamFactoryInterface();
    virtual ~MessageStreamFactoryInterface();

    /**
       @brief Create a regular message stream to @a address and @a port.
       @return
     */
    virtual Protocol::MessageStream createMessageStream (Address address, Port port) = 0;

    /**
       @brief   Create a global (broadcast) message stream.
       @return
     */
    virtual Protocol::MessageStream createGlobalMessageStream () = 0;

    /**
       @brief Create a message server with @a port.
       @param port
       @return
     */
    virtual Protocol::MessageServer createMessageServer(const Port &port = {ZQ_PORT_ANY}) = 0;

    /**
       @brief Create a server from @a stream 's port and resources.
       @param stream
       @return
     */
    virtual Protocol::MessageServer createMessageServerFromStream (Protocol::MessageStream &&stream) = 0;

};

} // namespace Ziqe

#endif // ZIQE_MESSAGESTREAMFACTORYINTERFACE_HPP
