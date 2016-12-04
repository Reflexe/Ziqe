/**
 * @file FieldWriter.hpp
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
        vector[7] = static_cast<uint8_t>((value & 0x00000000000000ffu));
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
        vector[0] = static_cast<uint8_t>((value & 0x00000000000000ffu));
    }
};

template<class WriterType, class VectorType=ExtendedVector<uint8_t>, class ReferenceType=VectorType>
class FieldWriter
{
public:
    FieldWriter(ReferenceType &&vector)
        : mVector{Base::forward<ReferenceType> (vector)}
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

    template<class T, class ...Args>
    static SizeType mySizeOfArgs (const Vector<T> &v,
                                  const Args &...args)
    {
        return (v.size () * sizeof (T)) + mySizeOfArgs (args...);
    }

    template<class T, class ...Args>
    static SizeType mySizeOfArgs (const RawArray<T> &v,
                                  const Args &...args)
    {
        return (v.size () * sizeof (T)) + mySizeOfArgs (args...);
    }

    template<class T, class A, class B, class ...Args>
    static SizeType mySizeOfArgs (const ExtendedVector<T,A,B> &v,
                                  const Args &...args)
    {
        return (v.size () * sizeof (T)) + mySizeOfArgs (args...);
    }

    template<class Writable, typename = decltype (Writable::writableSize()), class ...Args>
    static SizeType mySizeOfArgs (Writable &writable,
                                  const Args &...args)
    {
        return (writable.writableSize ()) + mySizeOfArgs (args...);
    }

    static constexpr SizeType mySizeOfArgs ()
    {
        return 0;
    }

    template<class ...Args>
    void writeT (Args&&... args) {
        auto argsSize = mySizeOfArgs (args...);

        // Make sure we have enough bytes in the vector for args.
        if (getVector().size () < argsSize)
            getVector().expand (argsSize - getVector().size());

        pack_foreach ([this](const auto &arg) {
                          this->writeOneT (arg);
                      }, Base::forward<Args>(args)...);
    }

    VectorType &getVector()
    {
        return mVector;
    }

private:
    /// Write integers.
    void writeOneT (const uint8_t &value) {
        mWriter.write (value, getVector());
        getVector().increaseBegin (sizeof (value));
    }

    void writeOneT (const uint16_t &value) {
        mWriter.write (value, getVector());
        getVector().increaseBegin (sizeof (value));
    }

    void writeOneT (const uint32_t &value) {
        mWriter.write (value, getVector());
        getVector().increaseBegin (sizeof (value));
    }

    void writeOneT (const uint64_t &value) {
        mWriter.write (value, getVector());
        getVector().increaseBegin (sizeof (value));
    }

    // Write containers.
    template<class T>
    void writeOneT (const Vector<T> &vector) {
        for (auto &element : vector) {
            writeOneT (element);
        }
    }

    template<class T, class A, class B>
    void writeOneT (const ExtendedVector<T, A, B> &vector) {
        for (auto &element : vector) {
            writeOneT (element);
        }
    }

    template<class T>
    void writeOneT (const RawArray<T> &array) {
        for (auto &element : array) {
            writeOneT (element);
        }
    }

    /**
      @brief Write a writable object.

      A writable object is object that has the following functions:
        - writeToWriter(writer): takes a writer as a argument and write to it.
        - writableSize: return the size of bytes writeToWriter.
     */
    template<class Writable>
    void writeOneT (const Writable &writable)
    {
        writable.writeToWriter (*this);
    }

    WriterType mWriter;
    ReferenceType mVector;
};

template<class VectorType=ExtendedVector<uint8_t>, class ReferenceType=VectorType>
using BigEndianFieldWriter=FieldWriter<BigEndianWriter<VectorType>, VectorType, ReferenceType>;
template<class VectorType=ExtendedVector<uint8_t>, class ReferenceType=VectorType>
using LittleEndianFieldWriter=FieldWriter<BigEndianWriter<VectorType>, VectorType, ReferenceType>;

} // namespace Base
} // namespace Ziqe

#endif // ZIQE_FIELDWRITER_H
