/**
 * @file Vector.hpp
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
#ifndef ZIQE_VECTOR_H
#define ZIQE_VECTOR_H

#include "Types.hpp"
#include "Macros.hpp"
#include "Memory.hpp"
#include "IteratorTools.hpp"

namespace Ziqe {
namespace Base {

/**
 * @brief A very simple vector implementation, with no insert,push_back,pop_back,erase and more.
 *        Basicly, it's a normal dynamic array: what std::vector should really be.
 */
template<class T, class Allocator=Allocator<T>, class Constructor=Constructor<T>>
class Vector
{
public:
    typedef SizeType SizeType;
    typedef T* PointerType;
    typedef T* Iterator;
    typedef const T* ConstIterator;

    Vector() = default;

    template<class InputIterator>
    Vector(const InputIterator &begin, const InputIterator &end)
        : Vector{begin, end, countRange(begin, end)}
    {
    }

    template<class InputIterator>
    Vector(const InputIterator &begin,
           const InputIterator &end,
           SizeType beginToEnd)
    {
        resizeBuffer (beginToEnd);
        insertToUninitilizedBuffer (0, begin, end);
    }

    ~Vector()
    {
        deleteAll (mPointer, mSize);
    }

    Vector(Vector &&other)
        : mPointer{Base::move (other.mPointer)}, mSize{other.mSize}
    {
        other.mSize = 0;
        other.mPointer = nullptr;
    }

    Vector &operator = (Vector &&other) {
        deleteAll (mPointer, mSize);

        mPointer = other.mPointer;
        mSize    = other.mSize;

        other.mSize = 0;
        other.mPointer = nullptr;

        return *this;
    }

    Vector(const Vector &other)
        : Vector{other.mPointer, other.mPointer+other.mSize, other.mSize}
    {
    }

    T &operator[] (SizeType index)
    {
        return *(begin() + index);
    }

    const T &operator[] (SizeType index) const
    {
        return *(begin() + index);
    }

    Vector &operator = (const Vector &other) {
        // assign will deleteAll for us.

        assign (other.mPointer, other.mPointer+other.mSize, other.mSize);

        return *this;
    }

    ZQ_DEFINE_CONST_AND_NON_CONST (RawArray<const T>, RawArray<T>, toRawArray, (), { return {data (), size ()}; })

    template<class InputIterator>
    void assign(const InputIterator &begin, const InputIterator &end)
    {
        return assign (begin, end, countRange(begin, end));
    }

    template<class InputIterator>
    void assign(const InputIterator &begin, const InputIterator &end, SizeType beginToEnd) {
        if (mSize == beginToEnd)
        {
            insertToUninitilizedBuffer (0, begin, end);
        }

        deleteAll (mPointer, mSize);
        reset ();

        resizeBuffer (beginToEnd);
        insertToUninitilizedBuffer (0, begin, end);
    }

    template<class ...Args>
    void resize(SizeType newSize, Args &&...args) {
        if (newSize == mSize)
            return;

        if (newSize == 0) {
            deleteAll (mPointer, mSize);
            reset ();

            return;
        }

        SizeType oldSize = mSize;
        resizeBuffer (newSize);

        // Construct the new objects (If there are).
        if (newSize > oldSize)
            mConstrutor.constructN (mPointer+oldSize, newSize-oldSize, Base::forward<Args>(args)...);
    }

    void shrinkWithoutFree(SizeType howMuch) {
        DEBUG_CHECK (mSize >= howMuch);

        mSize -= howMuch;
    }

    template<class ...Args>
    void expand (SizeType size, Args&&...args)
    {
        resize (mSize + size, Base::forward<Args>(args)...);
    }

    template<class InputIterator>
    void expand (InputIterator begin, InputIterator end, SizeType beginToEnd) {
        SizeType oldSize = mSize;

        resizeBuffer (mSize + beginToEnd);
        insertToUninitilizedBuffer (oldSize, begin, end);
    }

    template<class InputIterator>
    void expand (InputIterator begin, InputIterator end)
    {
        expand (begin, end, countRange (begin, end));
    }

    /**
     * @brief Insert a few pairs of [begin,end].
     */
    template<class...InputIteratorsPairsOrTriples>
    void expandFew (const InputIteratorsPairsOrTriples&... beginAndEnds) {
        SizeType oldSize = mSize;
        SizeType size = plusArgs<SizeType> (countRange (beginAndEnds)...);

        resizeBuffer (mSize + size);
        expandFewInsertLoop (oldSize, beginAndEnds...);
    }

    T *data()
    {
        return mPointer;
    }

    const T *data() const
    {
        return mPointer;
    }

    T *begin ()
    {
        return data();
    }

    const T *begin () const
    {
        return data();
    }

    T *end ()
    {
        return begin() + mSize;
    }

    const T *end () const
    {
        return begin() + mSize;
    }

    SizeType size() const
    {
        return mSize;
    }

private:
    template<class Arg, class...Args>
    void expandFewInsertLoop (SizeType currnetIndex,
                              const Arg &beginAndEnd,
                              const Args&...args)
    {
        currnetIndex += insertToUninitilizedBuffer (currnetIndex,
                                                    beginAndEnd.first,
                                                    beginAndEnd.second);
        return expandFewInsertLoop (currnetIndex, args...);
    }

    void reset() {
        mSize = 0;
        mPointer = nullptr;
    }

    void resizeBuffer (SizeType newSize) {
        if (mSize == newSize) {
            // destruct without free ing.
            mConstrutor.destruct (mPointer, mSize);
            return;
        }

        PointerType pointer{Base::move (mPointer)};
        auto pointerSize = mSize;

        mPointer = mAllocator.allocate (newSize);
        mSize = newSize;

        if (pointerSize != 0) {
            moveOldBufferToNew (pointer, pointerSize, newSize);
            deleteAll (pointer, pointerSize);
        }
    }

    // Move oldBuffer to mPointer (maybe only particly).
    void moveOldBufferToNew(PointerType &oldBuffer, SizeType oldSize, SizeType newSize) {
        insertToUninitilizedBuffer (0,
                                    makeMoveIterator (oldBuffer),
                                    makeMoveIterator (oldBuffer + min(oldSize, newSize)));
    }

    template<class InputIterator>
    SizeType insertToUninitilizedBuffer (const SizeType index,
                                         const InputIterator &begin,
                                         const InputIterator &end) {
        auto currentIndex = index;

        for (auto iterator{begin}; iterator != end; ++iterator, ++currentIndex)
        {
            mConstrutor.construct (&mPointer[currentIndex], *iterator);
        }

        return currentIndex-index;
    }

    void deleteAll (PointerType pointer, SizeType size) {
        if (size == 0)
            return;

        mConstrutor.destruct (pointer, size);
        mAllocator.deallocate (pointer);
    }

    PointerType mPointer = nullptr;
    SizeType mSize = 0;

    Allocator mAllocator;
    Constructor mConstrutor;
};

} // namespace Base
} // namespace Ziqe

#endif // ZIQE_VECTOR_H
