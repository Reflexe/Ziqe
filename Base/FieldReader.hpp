/**
 * @file FieldReader.hpp
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
#ifndef ZIQE_FIELDREADER_H
#define ZIQE_FIELDREADER_H

#include "Base/Memory.hpp"
#include "Base/ExtendedVector.hpp"
#include "Base/Macros.hpp"

namespace Ziqe {
namespace Base {

template<class VectorType>
struct LittleEndianValueReader {
    void readValue (uint8_t &value, VectorType &vector) {
        value = vector[0];
    }

    void readValue (uint16_t &value, VectorType &vector) {
        value = static_cast<uint16_t> ((static_cast<uint32_t>(vector[1] << 8)
                                       | (static_cast<uint32_t>(vector[0]))));
    }

    void readValue (uint32_t &value, VectorType &vector) {
        value = (  (static_cast<uint32_t>(vector[3]) << 24)
                 | (static_cast<uint32_t>(vector[2]) << 16)
                 | (static_cast<uint32_t>(vector[1]) << 8)
                 | (static_cast<uint32_t>(vector[0])));
    }

    void readValue (uint64_t &value, VectorType &vector) {
        value = (  (static_cast<uint64_t>(vector[7]) << 56)
                 | (static_cast<uint64_t>(vector[6]) << 48)
                 | (static_cast<uint64_t>(vector[5]) << 40)
                 | (static_cast<uint64_t>(vector[4]) << 32)
                 | (static_cast<uint64_t>(vector[3]) << 24)
                 | (static_cast<uint64_t>(vector[2]) << 16)
                 | (static_cast<uint64_t>(vector[1]) << 8)
                 | (static_cast<uint64_t>(vector[0])));
    }
};

template<class VectorType>
struct BigEndianValueReader {
    void readValue (uint8_t &value, VectorType &vector) {
        value = vector[0];
    }

    void readValue (uint16_t &value, VectorType &vector) {
        value = static_cast<uint16_t> ((static_cast<uint32_t>(vector[0] << 8)
                                       | (static_cast<uint32_t>(vector[1]))));
    }

    void readValue (uint32_t &value, VectorType &vector) {
        value = (  (static_cast<uint32_t>(vector[0]) << 24)
                 | (static_cast<uint32_t>(vector[1]) << 16)
                 | (static_cast<uint32_t>(vector[2]) << 8)
                 | (static_cast<uint32_t>(vector[3])));
    }

    void readValue (uint64_t &value, VectorType &vector) {
        value = (  (static_cast<uint64_t>(vector[0]) << 56)
                 | (static_cast<uint64_t>(vector[1]) << 48)
                 | (static_cast<uint64_t>(vector[2]) << 40)
                 | (static_cast<uint64_t>(vector[3]) << 32)
                 | (static_cast<uint64_t>(vector[4]) << 24)
                 | (static_cast<uint64_t>(vector[5]) << 16)
                 | (static_cast<uint64_t>(vector[6]) << 8)
                 | (static_cast<uint64_t>(vector[7])));
    }
};

template<class ValueReader, class _VectorType=ExtendedVector<uint8_t>>
class FieldReader
{
public:
    typedef _VectorType VectorType;

    FieldReader(VectorType &&vector)
        : mVector{Base::move (vector)}
    {
    }

    ZQ_ALLOW_COPY_AND_MOVE (FieldReader)

    template <typename T, SizeType sByteLength=sizeof (T)>
    T readT() {
        T value;
        mValueReader.readValue (value, mVector);
        mVector.increaseBegin (sByteLength);

        return value;
    }

    void skipBytes (SizeType length)
    {
        mVector.increaseBegin (length);
    }

    bool haveBytes (SizeType length) const
    {
        return mVector.size () >= length;
    }

    RawPointer<uint8_t> getCurrent()
    {
        return &(mVector[0]);
    }

    template<class T, SizeType sByteLength=sizeof (T)>
    bool canReadT() const
    {
        return haveBytes (sByteLength);
    }

    VectorType &getVector ()
    {
        return mVector;
    }

private:
    ValueReader mValueReader;

    VectorType mVector;
};


template<class VectorType=ExtendedVector<uint8_t>>
using LittleEndianFieldReader=FieldReader<LittleEndianValueReader<VectorType>,VectorType>;

template<class VectorType=ExtendedVector<uint8_t>>
using BigEndianFieldReader=FieldReader<BigEndianValueReader<VectorType>,VectorType>;

} // namespace Base
} // namespace Ziqe

#endif // ZIQE_FIELDREADER_H
