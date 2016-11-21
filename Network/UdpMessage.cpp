/**
 * @file UdpMessage.cpp
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
#include "UdpMessage.hpp"

namespace Ziqe {
namespace Net {

UdpMessage::UdpMessage(UdpMessage::Flags flags, MessageNumberType messageNumber)
    : mFlags{flags}, mMessageNumber{messageNumber}
{

}

Base::Expected<UdpMessage, UdpMessage::ReadFromError>
UdpMessage::ReadFrom(InputExtendedVectorType &vector) {
    FieldReaderType reader{vector};

    // Read the flags.
    if (!reader.canReadT<FlagsIntegerType>())
        return {ReadFromError::NotEnoughData};

    auto flags = static_cast<Flags>(reader.readT<FlagsIntegerType>());

    if (! reader.canReadT<MessageNumberType>())
        return {ReadFromError::NotEnoughData};

    auto messageNumber = reader.readT<MessageNumberType>();

    UdpMessage message{flags, messageNumber};

    return message;
}

void UdpMessage::writeTo(OutputExtendedVectorType &vector) {
    FieldWriterType writer{vector};

    writer.writeT(static_cast<FlagsIntegerType> (mFlags), mMessageNumber);
}

} // namespace Net
} // namespace Ziqe
