/**
 * @file SharedVector.h
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
#ifndef ZIQE_SHAREDVECTOR_H
#define ZIQE_SHAREDVECTOR_H

#include "Memory.h"
#include "Macros.h"
#include "Vector.h"
#include "Checks.h"

namespace Ziqe {

template<class T, class VectorType=Vector<T>, class PointerType=SharedPointer<Vector<T>>>
class SharedVector
{
public:
    typedef typename VectorType::SizeType SizeType;

    const static SizeType kNoIndex = static_cast<SizeType>(-1);

    SharedVector(const PointerType &vector)
        : mVector{vector}, mIndexBegin{0}, mIndexEnd{vector->size()}
    {
    }

    SharedVector(const PointerType &vector,
                 const SizeType             indexBegin,
                 const SizeType             indexEnd=kNoIndex)
        : mVector{vector},
          mIndexBegin{indexBegin},
          mIndexEnd{(indexEnd == kNoIndex) ? mVector->size() : indexEnd}
    {
        DEBUG_CHECK (mIndexEnd > mIndexBegin);
    }

    ALLOW_COPY_AND_MOVE (SharedVector)

    T &operator [](SizeType index) {
        DEBUG_CHECK_ADD_OVERFLOW (index, mIndexBegin);

        return (*mVector)[index + mIndexBegin];
    }

    const T &operator [](SizeType index) const {
        DEBUG_CHECK_ADD_OVERFLOW (index, mIndexBegin);

        return (*mVector)[index + mIndexBegin];
    }

    template<class _VectorType>
    void insertVectorAtBegin (const _VectorType &vector) {
        DEBUG_CHECK (vector.size () <= size ());

        SizeType i = 0;

        for (const auto &value : vector)
        {
            (*this)[i++] = value;
        }

        increaseBegin (i);
    }

    SizeType size() const
    {
        return mIndexEnd - mIndexBegin;
    }

    SizeType getSize() const
    {
        return size();
    }

    void increaseBegin (SizeType howMuch = 1) {
        DEBUG_CHECK_ADD_OVERFLOW (howMuch, mIndexBegin);
        DEBUG_CHECK (size() + howMuch <= mVector->size ());

        mIndexBegin += howMuch;
    }

    T &front ()
    {
        return (*this)[0];
    }

    T &back ()
    {
        return (*this)[mIndexEnd-1];
    }

    VectorType &getVector ()
    {
        return *mVector;
    }

    typedef typename VectorType::Iterator Iterator;
    typedef typename VectorType::ConstIterator ConstIterator;
    Iterator begin ()
    {
        return mVector->begin () + mIndexBegin;
    }

    Iterator end ()
    {
        return mVector->begin () + mIndexEnd;
    }

    ConstIterator begin () const
    {
        return mVector->begin () + mIndexBegin;
    }

    ConstIterator end () const
    {
        return mVector->begin () + mIndexEnd;
    }


private:
    PointerType mVector;

    SizeType mIndexBegin;
    SizeType mIndexEnd;
};

} // namespace Ziqe

#endif // ZIQE_SHAREDVECTOR_H
