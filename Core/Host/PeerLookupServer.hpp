/**
 * @file PeerLookupServer.hpp
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
#ifndef PEERLOOKUPSERVER_HPP
#define PEERLOOKUPSERVER_HPP

#include "Protocol/MessageServer.hpp"
#include "Common/MessageStreamFactoryInterface.hpp"

#include "Host/Thread.hpp"

namespace Ziqe {
namespace Host {

class PeerLookupServer
{
public:
    PeerLookupServer(Base::UniquePointer<MessageStreamFactoryInterface> &&factory);

    void run();

private:
    void handleMessage (const Protocol::MessageStream &clientStream, const Protocol::Message &message,
                        const Protocol::MessageStream::MessageFieldReader &reader);

    Protocol::MessageServer mServer;

    Base::UniquePointer<MessageStreamFactoryInterface> mStreamFactory;
};

} // namespace Host
} // namespace Ziqe

#endif // PEERLOOKUPSERVER_HPP
