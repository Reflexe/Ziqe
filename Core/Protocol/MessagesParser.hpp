/**
 * @file MessagesParser.hpp
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
#ifndef ZIQE_MESSAGESPARSER_H
#define ZIQE_MESSAGESPARSER_H

#include "Base/Vector.hpp"
#include "Base/FieldReader.hpp"

#include "Protocol/Message.hpp"

namespace Ziqe {
namespace Protocol {

template<class FieldReaderType>
class MessagesParser
{
    FieldReaderType mFieldReader;

public:
    // Stupid C++ strandard doesn't allow partical specialization for functions.
    // C++17 allows infering of templates argument from constructors, so here we
    // in the same problem.
    template<class T>
    struct MessageParser
    {
    };

    enum class ParseError {
        TooShort,
        InvalidType,
        Other
    };

    template<Message::Type type>
    struct MessageParser<MessageWithThreadID<type>>
    {

    };

    MessagesParser(FieldReaderType &&fieldReader)
        : mFieldReader{Base::move (fieldReader)}
    {
    }

    Base::Expected<ContiniueThreadMessage, ParseError> parseContiniueThread ()
    {

    }
};

} // namespace Ziqe
} // namespace Protocol

#endif // ZIQE_MESSAGESPARSER_H
