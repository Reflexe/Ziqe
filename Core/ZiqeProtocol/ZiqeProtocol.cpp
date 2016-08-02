/**
 * @file ZiqeProtocol.cpp
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
#include "ZiqeProtocol.h"

namespace Ziqe {

ZiqeProtocol::ZiqeProtocol(UniquePointer<InputStreamInterface> &globaInputStream,
                           UniquePointer<OutputStreamInterface> &globalOutputStream)
{

}

void ZiqeProtocol::onDataReceived(InputStreamInterface::DataType &&data) {
    FieldReader reader{std::move (data)};
    Message message{reader};

    // Generate An Abstract Packet Info that has the capabity to
    // be hashed and to be comperaed to others (virtual functions)
    // It should be filled in the NetworkProtocol.

    // PROBLEM: HashTable doesn't support virtual/abstract keys.
    // SOVLE:   using of UniquePointer as keys and replace the
    //          compare func with a custom one (that will not check
    //          only the address).

    if (! message.isGlobalMessage ())
    {   // Invalid message.
        return;
    }

    return processGlobalMessage (message);
}

void ZiqeProtocol::processGlobalMessage(Message &&message) {
    switch (message.getType ())
    {
    // How to get its ip? i should have a PeerID that i generate from the
    // peer's address and more.
    case Message::Type::PeerHello:

        break;
    case Message::Type::PeerGoodbye:
        break;
    default:
        DEBUG_CHECK_NOT_REACHED ();
        return;
    }


}

}

