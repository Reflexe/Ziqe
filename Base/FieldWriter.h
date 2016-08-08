/**
 * @file FieldWriter.h
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

#include "Base/SharedVector.h"

namespace Ziqe {

class FieldWriter
{
public:
    FieldWriter(SharedVector<Byte> &&vector);
    FieldWriter();


    /// Like sizeOfArgs but with an overload for Vector<Byte> and SharedVector<Byte>.
    template<class T, class ...Args>
    static constexpr SizeType mySizeOfArgs (const T &,
                                     const Args &...args)
    {
        return sizeof (T) + mySizeOfArgs (args...);
    }

    template<class T, class ...Args>
    static constexpr SizeType mySizeOfArgs (const Vector<Byte> &v,
                                            const Args &...args)
    {
        return v.size () + mySizeOfArgs (args...);
    }

    template<class T, class ...Args>
    static constexpr SizeType mySizeOfArgs (const SharedVector<Byte> &v,
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

        ExpandArgs(writeOneT (args)...);
    }

    SharedVector<Byte> &getVector()
    {
        return mVector;
    }

private:
    template<class T>
    void writeOneT (T value) {
        *reinterpret_cast<T*>(&(mVector.front ())) = value;
        mVector.increaseBegin (sizeof (T));
    }

    void writeOneT (const Vector<Byte> &vector)
    {
        mVector.insertVectorAtBegin (vector);
    }

    void writeOneT (const SharedVector<Byte> &vector)
    {
        mVector.insertVectorAtBegin (vector);
    }

    SharedVector<Byte> mVector;
};

} // namespace Ziqe

#endif // ZIQE_FIELDWRITER_H
