/**
 * @file ExtendedVector.hpp
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
#ifndef ZIQE_EXTENDEDVECTOR_H
#define ZIQE_EXTENDEDVECTOR_H

#include "Base/Vector.hpp"
#include "Base/Checks.hpp"

namespace Ziqe {
namespace Base {

/**
  @brief An template used to extend vector's functionality and support
         offseted vectors.

  @note Some types implements this functionality by itself and not uses
        this template.
 */
template<class T, class VectorType=Vector<T>,
                  // By default it's the vector's container.
                  class VectorReferenceType=VectorType>
class ExtendedVector
{
public:
    typedef typename VectorType::SizeType SizeType;

    const static SizeType kNoIndex = static_cast<SizeType>(-1);

    ExtendedVector()
        : mIndexBegin{0}, mIndexEnd{0}
    {

    }

    explicit ExtendedVector(VectorReferenceType &&vector)
        : mVector{Base::forward<VectorReferenceType> (vector)}, mIndexBegin{0}, mIndexEnd{static_cast<SizeType>(getVector().size())}
    {
    }

    explicit ExtendedVector(VectorReferenceType &&vector,
                            const SizeType        indexBegin,
                            const SizeType        indexEnd=kNoIndex)
        : mVector{Base::move (vector)},
          mIndexBegin{indexBegin},
          mIndexEnd{(indexEnd == kNoIndex) ? static_cast<SizeType>(getVector().size()) : indexEnd}
    {
        ZQ_ASSERT (mIndexEnd > mIndexBegin);
    }

    ZQ_ALLOW_COPY_AND_MOVE (ExtendedVector)

    T &operator [](SizeType index) {
        DEBUG_CHECK_ADD_OVERFLOW (index, mIndexBegin);

        return (getVector())[static_cast<SizeType>(index + mIndexBegin)];
    }

    const T &operator [](SizeType index) const {
        DEBUG_CHECK_ADD_OVERFLOW (index, mIndexBegin);

        return (getVector())[static_cast<SizeType>(index + mIndexBegin)];
    }

    template<class _VectorType>
    void insertVectorAtBegin (const _VectorType &vector) {
        ZQ_ASSERT (vector.size () <= size ());

        SizeType i = 0;

        for (const auto &value : vector)
        {
            (*this)[i++] = value;
        }

        increaseBegin (i);
    }

    SizeType size() const
    {
        return static_cast<SizeType>(mIndexEnd - mIndexBegin);
    }

    SizeType getSize() const
    {
        return size();
    }

    bool hasLength (const SizeType length) const
    {
        return size () >= length;
    }

    void increaseBegin (SizeType howMuch = 1) {
        DEBUG_CHECK_ADD_OVERFLOW (howMuch, mIndexBegin);
        ZQ_ASSERT (mIndexBegin + howMuch <= mIndexEnd);

        mIndexBegin += howMuch;
    }

    void setEnd (SizeType newEnd) {
        ZQ_ASSERT (mIndexBegin <= newEnd);
        mIndexEnd = newEnd;
    }

    void setBegin (SizeType newBegin) {
        ZQ_ASSERT (mIndexEnd >= newBegin);
        mIndexBegin = newBegin;
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
        return mVector;
    }

    void expand (SizeType howMuch) {
        auto newSize = size() + howMuch;
        auto currentUnderlyingVectorSize = getVector ().size();

        // If the underlying vector has more element at the end,
        // "expand" the vector to them.
        if (mIndexEnd != currentUnderlyingVectorSize) {
            auto sizeVaildableForVirtualExpand = (currentUnderlyingVectorSize - mIndexEnd);
            auto virtualExpandSize = min (howMuch, sizeVaildableForVirtualExpand);

            newSize -= virtualExpandSize;
            mIndexEnd = sizeVaildableForVirtualExpand-virtualExpandSize;
        }

        // If we need to expand more elements, do it.
        if (newSize > 0) {
            getVector ().expand (howMuch);
        }
    }

    typedef typename VectorType::Iterator Iterator;
    typedef typename VectorType::ConstIterator ConstIterator;
    Iterator begin ()
    {
        return getVector().begin () + mIndexBegin;
    }

    Iterator end ()
    {
        return getVector().begin () + mIndexEnd;
    }

    ConstIterator begin () const
    {
        return getVector().begin () + mIndexBegin;
    }

    ConstIterator end () const
    {
        return getVector().begin () + mIndexEnd;
    }

    SizeType getIndexBegin () const
    {
        return mIndexBegin;
    }

    SizeType getIndexEnd () const
    {
        return mIndexEnd;
    }

private:
    VectorReferenceType mVector;

    SizeType mIndexBegin;
    SizeType mIndexEnd;
};

template<class T>
using SharedExtendedVector=ExtendedVector<T,
                                          Vector<T>,
                                          SharedPointer<Vector<T>>>;

template<class T>
using ReferenceExtendedVector=ExtendedVector<T,
                                            Vector<T>,
                                            Vector<T>&>;

} // namespace Base
} // namespace Ziqe

#endif // ZIQE_EXTENDEDVECTOR_H
