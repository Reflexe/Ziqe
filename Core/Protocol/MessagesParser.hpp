/**
 * @file MessagesParser.hpp
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
#ifndef ZIQE_MESSAGESPARSER_H
#define ZIQE_MESSAGESPARSER_H

#include "Base/Vector.hpp"

#include "Base/FieldReader.hpp"

#include "Core/ZiqeProtocol/Message.hpp"

namespace Ziqe {
namespace Protocol {

class MessagesParser
{
public:
    MessagesParser();

    // TOOD: parsering - should MessageStream do it?
#if 0
    typedef Base::Vector<uint8_t> MessageContainer;
    typedef uint64_t IdentiferType;

    template<class Arg>
    static bool readArg (FieldReader &reader)
    {
        return true;
    }

    template<class Arg, class... Args>
    static bool readArg (FieldReader &reader, Arg &arg, Args&... args) {
        if (! reader.canReadT<Arg> ())
            return false;

        arg = reader.readT<Arg> ();

        return readArg (reader, args...);
    }

    template<class...Args>
    static bool readVector (FieldReader &reader, Args&&...args) {

        if (! readArg (reader, args...))
            return false;

        return true;
    }

    static bool readMessageType (FieldReader &reader, Message::Type &messageType) {
        DWord dwordMessageType;

        if (! reader.canReadT<DWord>())
            return false;

        dwordMessageType = reader.readT<DWord>();
        messageType = static_cast<Message::Type>(dwordMessageType);

        return true;
    }
#endif
};

} // namespace Ziqe
} // namespace Protocol

#endif // ZIQE_MESSAGESPARSER_H
