/**
 * @file GlobalPeers.cpp
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
#include "GlobalPeers.hpp"

namespace Ziqe {

GlobalPeers::GlobalPeers()
{

}

Base::UniquePointer<GlobalThread> GlobalPeers::runThread(const LocalThread &localThread) {
    // Choose the next identifer: should be the previous one plus 1.
    auto messageIdentifer = mNextIdentifer;
    ++mNextIdentifer;

    mGlobalMessageStream.sendMessage (MessagesGenerator::makeRunThreadPeerLookup (messageIdentifer));

    ProcessPeersServer processPeers;
}

void GlobalPeers::onMessageReceived(const Protocol::Message &type,
                                    Protocol::MessageStream::Callback::MessageFieldReader &fieldReader,
                                    const Protocol::MessageStream &messageStream)
{
    using Protocol::Message;

    // TODO:
    switch (type.getType ()) {
    case Message::Type::RunThreadPeerLookup:
        break;
    case Message::Type::RunThreadPeerLookupAcceptPropose:
        break;
    case Message::Type::RunThreadPeerLookupPropose:
        break;
    default:
        return;
    }
}

} // namespace Ziqe
