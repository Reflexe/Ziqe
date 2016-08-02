/**
 * @file Message.h
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
#ifndef ZIQE_MESSAGE_H
#define ZIQE_MESSAGE_H

#include "Base/Types.h"
#include "Base/SharedVector.h"
#include "Base/FieldReader.h"

namespace Ziqe {

class Message
{
public:
    Message(MessageType type);
    Message(FieldReader &reader);

    enum class Type : DWord {
        /// Peer to Peer messages
        ///
        GetMemory           = 0,
        WriteMemory         = 1,
        GivePage            = 2,
        RunThread           = 3,
        StopThread          = 4,
        DoSystemCall        = 5,
        SystemCallResult    = 6,

        NewProcessPeer      = 7,
        ProcessPeerDeleted  = 8,

        /// Global messages
        ///
        PeerHello           = 5000,
        PeerGoodbye         = 5001,
    };

    typedef Type MessageType;

    MessageType getType() const
    {
        return mMessageType;
    }

    bool isP2PMessage () const;
    bool isGlobalMessage() const;

private:
    MessageType mMessageType;
};

} // namespace Ziqe

#endif // ZIQE_MESSAGE_H
