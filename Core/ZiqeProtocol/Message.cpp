/**
 * @file Message.cpp
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
#include "Message.h"

namespace Ziqe {

Message::Message(MessageType type)
{

}

Message::Message(FieldReader &reader)
    : Message{reader.readT<DWord>()}
{
}

bool Message::isP2PMessage() const{
    switch (mMessageType) {
    case MessageType::GetMemory:
    case MessageType::WriteMemory:
    case MessageType::GivePage:
    case MessageType::RunSharedProcessThread:
    case MessageType::StopThread:
    case MessageType::DoSystemCall:
    case MessageType::SystemCallResult:
        return true;
    default:
        return false;
    }
}

bool Message::isGlobalMessage() const{
    switch (mMessageType) {
    case MessageType::RunThreadPeerLookup:
        return true;
    default:
        return false;
    }
}

} // namespace Ziqe
