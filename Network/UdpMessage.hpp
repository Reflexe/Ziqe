/**
 * @file UdpMessage.hpp
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
#ifndef UDPMESSAGE_HPP
#define UDPMESSAGE_HPP

#include "Base/Types.hpp"
#include "Base/Expected.hpp"
#include "Base/FieldReader.hpp"
#include "Base/FieldWriter.hpp"
#include "Base/ExtendedVectorWritable.hpp"

#include "Stream.hpp"

namespace Ziqe {
namespace Net {

class UdpMessage : public Base::ExtendedVectorWritable<Base::ExtendedVector<uint8_t>>
{
public:
    typedef Base::BigEndianFieldReader<InputExtendedVectorType, InputExtendedVectorType&> FieldReaderType;
    typedef Base::BigEndianFieldWriter<OutputExtendedVectorType, OutputExtendedVectorType&> FieldWriterType;

    typedef uint16_t FlagsIntegerType;
    enum class Flags : FlagsIntegerType{
        None = 0x0,
        /// Like TCP's SYN.
        SYN = 0x1,
        IsLastFragment = 0x4,
    };

    typedef uint16_t MessageNumberType;

    UdpMessage(UdpMessage::Flags flags, MessageNumberType messageNumber);

    enum class ReadFromError {
        NotEnoughData,
        OtherParseError,
    };

    /**
       @brief Read a UDP message from this
       @param reader
       @return
     */
    static Base::Expected<UdpMessage, ReadFromError> ReadFrom (InputExtendedVectorType &vector);

    virtual void writeTo (OutputExtendedVectorType &vector) const override;

    bool isCreateSession () const
    {
        return isFlagsBitOn (Flags::SYN);
    }

    bool isLastFragment() const
    {
        return isFlagsBitOn (Flags::IsLastFragment);
    }

    void setLastFragmentFlag (bool status)
    {
        setFlag (Flags::IsLastFragment, status);
    }

    void setCreateSessionFlag (bool status)
    {
        setFlag (Flags::SYN, status);
    }

    MessageNumberType getMessageNumber() const
    {
        return mMessageNumber;
    }

    void setMessageNumber (MessageNumberType messageNumber)
    {
        mMessageNumber = messageNumber;
    }

private:
    bool isFlagsBitOn (const Flags &flagBit) const
    {
        return static_cast<FlagsIntegerType>(mFlags) & static_cast<FlagsIntegerType>(flagBit);
    }

    void setFlag (const Flags &flagBit, bool status) {
        auto integerFlagBit = static_cast<FlagsIntegerType>(flagBit);

        if (! status)
            integerFlagBit = 0;

        mFlags = static_cast<Flags>(static_cast<FlagsIntegerType>(mFlags) & integerFlagBit);
    }

    Flags mFlags;

    MessageNumberType mMessageNumber;
};

} // namespace Net
} // namespace Ziqe

#endif // UDPMESSAGE_HPP
