/**
 * @file Vector.h
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
#ifndef ZIQE_VECTOR_H
#define ZIQE_VECTOR_H

#include "Types.h"
#include "Macros.h"
#include "Memory.h"
#include "IteratorTools.h"

namespace Ziqe {

/**
 * @brief Vector A very simple vector implementation, with no insert,push_back,pop_back,erase and more.
 *               Basicly, it's a normal dynamic array: what std::vector should really be.
 */
template<class T, class Allocator=Allocator<T>, class Constructor=Constructor<T>>
class Vector
{
public:
    typedef SizeType SizeType;
    typedef T* PointerType;
    typedef T* Iterator;
    typedef const T* ConstIterator;

    Vector()
    {
    }

    template<class InputIterator>
    Vector(const InputIterator &begin, const InputIterator &end)
        : Vector{begin, end, countRange(begin, end)}
    {
    }

    template<class InputIterator>
    Vector(const InputIterator &begin, const InputIterator &end, SizeType beginToEnd) {
        resizeBuffer (beginToEnd);
        insertToExistBuffer (0, begin, end);
    }

    ~Vector()
    {
        deleteAll (mPointer, mSize);
    }

    Vector(Vector &&other)
        : mPointer{move (other.mPointer)}, mSize{other.mSize}
    {
        other.mSize = 0;
    }

    Vector &operator = (Vector &&other) {
        deleteAll (mPointer, mSize);

        mPointer = move (other.mPointer);
        mSize    = other.mSize;

        other.mSize = 0;
        other.mPointer = nullptr;

        return *this;
    }

    Vector(const Vector &other)
        : Vector(other.mPointer, other.mPointer+other.mSize, other.mSize)
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

    template<class InputIterator>
    void assign(const InputIterator &begin, const InputIterator &end)
    {
        return assign (begin, end, countRange(begin, end));
    }

    template<class InputIterator>
    void assign(const InputIterator &begin, const InputIterator &end, SizeType beginToEnd) {
        if (mSize == beginToEnd)
        {
            insertToExistBuffer (0, begin, end);
        }

        deleteAll (mPointer, mSize);
        reset ();

        resizeBuffer (beginToEnd);
        insertToExistBuffer (0, begin, end);
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
            mConstrutor.constructN (mPointer+oldSize, newSize-oldSize, std::forward<Args>(args)...);
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
    void reset() {
        mSize = 0;
        mPointer = nullptr;
    }

    void resizeBuffer (SizeType newSize) {
        PointerType pointer{std::move (mPointer)};
        auto pointerSize = mSize;

        if (mSize == newSize)
        {
            // destruct without free ing.
        }

        mPointer = mAllocator.allocate (newSize);
        mSize = newSize;

        if (pointerSize != 0) {
            moveOldBufferToNew (pointer, pointerSize, newSize);
            deleteAll (pointer, pointerSize);
        }
    }

    // Move oldBuffer to mPointer (maybe only particly).
    void moveOldBufferToNew(PointerType &oldBuffer, SizeType oldSize, SizeType newSize) {
        insertToExistBuffer (0,
                             makeMoveIterator (oldBuffer),
                             makeMoveIterator (oldBuffer + min(oldSize, newSize)));
    }

    void deleteAll (PointerType pointer, SizeType size) {
        if (!size)
            return;

        mConstrutor.destruct (pointer, size);
        mAllocator.deallocate (pointer, size);
    }

    template<class InputIterator>
    void insertToExistBuffer (SizeType index, const InputIterator &begin, const InputIterator &end) {
        for (auto iterator = begin; iterator != end; ++iterator, ++index)
        {
            mConstrutor.construct (&mPointer[index], *iterator);
        }
    }

    PointerType mPointer = nullptr;
    SizeType mSize = 0;

    Allocator mAllocator;
    Constructor mConstrutor;
};

} // namespace Ziqe

#endif // ZIQE_VECTOR_H
