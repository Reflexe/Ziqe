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
#include "Base/Expected.hpp"

ZQ_BEGIN_NAMESPACE
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

template<class ValueReader, class _VectorType=ExtendedVector<uint8_t>, class _VectorReferenceType=_VectorType>
class FieldReader
{
public:
    typedef _VectorType VectorType;

    FieldReader(_VectorReferenceType &&vector)
        : mVector{Base::forward<_VectorReferenceType&&> (vector)}
    {
    }

    ZQ_ALLOW_COPY_AND_MOVE (FieldReader)

    template <typename T, SizeType sByteLength=sizeof (T)>
    T readT() {
        T value;
        mValueReader.readValue (value, mVector);
        getVector ().increaseBegin (sByteLength);

        return value;
    }

    template<typename T, SizeType sByteLength=sizeof (T)>
    bool canReadVectorT(SizeType numberOfElements)
    {
        return hasBytes (numberOfElements * sByteLength);
    }

    template<typename T, SizeType sByteLength=sizeof (T)>
    bool canReadVectorT(SizeType numberOfElements) const
    {
        return hasBytes (numberOfElements * sByteLength);
    }

    template <typename T, SizeType sByteLength=sizeof (T)>
    Base::Vector<T> readTVector (SizeType numberOfElements) {
        Base::Vector<T> elements;

        // FIXME: initilized in here and then in the next line.
        elements.resize (numberOfElements);

        for (typename Base::Vector<T>::SizeType i = 0; i < numberOfElements; ++i) {
            elements[i] = readT ();
        }

        return elements;
    }

    enum class ReadError{
        NoEnoguhLength
    };

    template<class T>
    Base::Expected<T, ReadError> tryReadT () {
        if (! canReadT<T> ())
            return {ReadError::NoEnoguhLength};

        return {readT<T>()};
    }

    template<class T>
    Base::Expected<Base::Vector<T>, ReadError> tryReadTVector (SizeType numberOfElements) {
        if (! canReadVectorT<T>(numberOfElements))
            return {ReadError::NoEnoguhLength};

        return {readTVector<T>(numberOfElements)};
    }

    void skipBytes (SizeType length)
    {
        getVector ().increaseBegin (length);
    }

    bool hasBytes (SizeType length) const
    {
        return getVector ().hasLength (length);
    }

    bool hasBytes (SizeType length)
    {
        return getVector ().hasLength (length);
    }

    RawPointer<uint8_t> getCurrent()
    {
        return &(getVector ()[0]);
    }

    template<class T, SizeType sByteLength=sizeof (T)>
    bool canReadT() const
    {
        return hasBytes (sByteLength);
    }

    template<class T, SizeType sByteLength=sizeof (T)>
    bool canReadT()
    {
        return hasBytes (sByteLength);
    }

    VectorType &getVector ()
    {
        return mVector;
    }

private:
    ValueReader mValueReader;

    _VectorReferenceType mVector;
};

template<class VectorType=ExtendedVector<uint8_t>, class ReferenceType=VectorType>
using LittleEndianFieldReader=FieldReader<LittleEndianValueReader<VectorType>,VectorType, ReferenceType>;

template<class VectorType=ExtendedVector<uint8_t>, class ReferenceType=VectorType>
using BigEndianFieldReader=FieldReader<BigEndianValueReader<VectorType>,VectorType, ReferenceType>;

} // namespace Base
ZQ_END_NAMESPACE

#endif // ZIQE_FIELDREADER_H
