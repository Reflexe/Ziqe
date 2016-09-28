/**
 * @file FieldWriter.hpp
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
#ifndef ZIQE_FIELDWRITER_H
#define ZIQE_FIELDWRITER_H

#include "Base/ExtendedVector.hpp"

namespace Ziqe {
namespace Base {

template<class VectorType>
struct BigEndianWriter {
    void write (uint8_t value, VectorType &vector)
    {
        vector[0] = value;
    }

    void write (uint16_t value, VectorType &vector) {
        vector[0] = static_cast<uint8_t>((static_cast<unsigned int>(value) & 0xff00u) >> 8);
        vector[1] = static_cast<uint8_t>((static_cast<unsigned int>(value) & 0x00ffu));
    }

    void write (uint32_t value, VectorType &vector) {
        vector[0] = static_cast<uint8_t>((static_cast<uint32_t>(value) & 0xff000000u) >> 24);
        vector[1] = static_cast<uint8_t>((static_cast<uint32_t>(value) & 0x00ff0000u) >> 16);
        vector[2] = static_cast<uint8_t>((static_cast<uint32_t>(value) & 0x0000ff00u) >> 8);
        vector[3] = static_cast<uint8_t>((static_cast<uint32_t>(value) & 0x000000ffu));
    }

    void write (uint64_t value, VectorType &vector) {
        vector[0] = static_cast<uint8_t>((value & 0xff00000000000000u) >> 56);
        vector[1] = static_cast<uint8_t>((value & 0x00ff000000000000u) >> 48);
        vector[2] = static_cast<uint8_t>((value & 0x0000ff0000000000u) >> 40);
        vector[3] = static_cast<uint8_t>((value & 0x000000ff00000000u) >> 32);
        vector[4] = static_cast<uint8_t>((value & 0x00000000ff000000u) >> 24);
        vector[5] = static_cast<uint8_t>((value & 0x0000000000ff0000u) >> 16);
        vector[6] = static_cast<uint8_t>((value & 0x000000000000ff00u) >> 8);
        vector[7] = static_cast<uint8_t>((value & 0xff00000000000000u));
    }
};

template<class VectorType>
struct LittleEndianWriter {
    void write (uint8_t value, VectorType &vector)
    {
        vector[0] = value;
    }

    void write (uint16_t value, VectorType &vector) {
        vector[1] = static_cast<uint8_t>((static_cast<unsigned int>(value) & 0xff00u) >> 8);
        vector[0] = static_cast<uint8_t>((static_cast<unsigned int>(value) & 0x00ffu));
    }

    void write (uint32_t value, VectorType &vector) {
        vector[3] = static_cast<uint8_t>((static_cast<uint32_t>(value) & 0xff000000u) >> 24);
        vector[2] = static_cast<uint8_t>((static_cast<uint32_t>(value) & 0x00ff0000u) >> 16);
        vector[1] = static_cast<uint8_t>((static_cast<uint32_t>(value) & 0x0000ff00u) >> 8);
        vector[0] = static_cast<uint8_t>((static_cast<uint32_t>(value) & 0x000000ffu));
    }

    void write (uint64_t value, VectorType &vector) {
        vector[7] = static_cast<uint8_t>((value & 0xff00000000000000u) >> 56);
        vector[6] = static_cast<uint8_t>((value & 0x00ff000000000000u) >> 48);
        vector[5] = static_cast<uint8_t>((value & 0x0000ff0000000000u) >> 40);
        vector[4] = static_cast<uint8_t>((value & 0x000000ff00000000u) >> 32);
        vector[3] = static_cast<uint8_t>((value & 0x00000000ff000000u) >> 24);
        vector[2] = static_cast<uint8_t>((value & 0x0000000000ff0000u) >> 16);
        vector[1] = static_cast<uint8_t>((value & 0x000000000000ff00u) >> 8);
        vector[0] = static_cast<uint8_t>((value & 0xff00000000000000u));
    }
};

template<class WriterType, class VectorType=ExtendedVector<uint8_t>>
class FieldWriter
{
public:
    FieldWriter(VectorType &&vector)
        : mVector{Base::move (vector)}
    {

    }

    FieldWriter() = default;

    /// Like sizeOfArgs but with an overload for Vector<uint8_t> and SharedVector<uint8_t>.
    template<class T, class ...Args>
    static constexpr SizeType mySizeOfArgs (const T &,
                                     const Args &...args)
    {
        return sizeof (T) + mySizeOfArgs (args...);
    }

    template<class ...Args>
    static SizeType mySizeOfArgs (const Vector<uint8_t> &v,
                                            const Args &...args)
    {
        return v.size () + mySizeOfArgs (args...);
    }

    template<class ...Args>
    static SizeType mySizeOfArgs (const RawArray<uint8_t> &v,
                                  const Args &...args)
    {
        return v.size () + mySizeOfArgs (args...);
    }

    template<class A, class B, class ...Args>
    static SizeType mySizeOfArgs (const ExtendedVector<uint8_t,A,B> &v,
                                  const Args &...args)
    {
        return v.size () + mySizeOfArgs (args...);
    }

    static constexpr SizeType mySizeOfArgs ()
    {
        return 0;
    }

    template<class ...Args>
    void writeT (Args... args) {
        mVector.getVector ().expand (mySizeOfArgs (args...));

        pack_foreach ([this](auto arg) {
                          writeOneT (arg);
                      }, std::forward<Args>(args)...);
    }

    VectorType &getVector()
    {
        return mVector;
    }

private:
    template<class T>
    void writeOneT (T value) {
        mWriter.write (value, mVector);
        mVector.increaseBegin (sizeof (T));
    }

    void writeOneT (const Vector<uint8_t> &vector)
    {
        mVector.insertVectorAtBegin (vector);
    }

    template<class A, class B>
    void writeOneT (const ExtendedVector<uint8_t, A, B> &vector)
    {
        mVector.insertVectorAtBegin (vector);
    }

    template<class T>
    void writeOneT (const RawArray<T> &array) {
        for (SizeType i = 0; i < array.size (); ++i)
        {
            writeOneT (array[i]);
        }
    }

    WriterType mWriter;

    VectorType mVector;
};

template<class VectorType=ExtendedVector<uint8_t>>
using BigEndianFieldWriter=FieldWriter<BigEndianWriter<VectorType>,
                                       VectorType> ;
template<class VectorType=ExtendedVector<uint8_t>>
using LittleEndianFieldWriter=FieldWriter<BigEndianWriter<VectorType>,
                                          VectorType> ;

} // namespace Base
} // namespace Ziqe

#endif // ZIQE_FIELDWRITER_H
