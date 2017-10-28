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

#include "Utils/Types.hpp"
#include "Utils/Macros.hpp"

#include "Utils/Constructor.hpp"

#include "Utils/Allocator.hpp"
#include "Utils/IteratorTools.hpp"
#include "Utils/RawPointer.hpp"
#include "Utils/Checks.hpp"
#include "Utils/UniquePointer.hpp"

ZQ_BEGIN_NAMESPACE
namespace Utils {

/**
 * @brief A very simple vector implementation, with no insert,push_back,pop_back,erase and more.
 *        Basicly, it's a normal dynamic array: what std::vector should really be.
 */
template<class T,
         class Allocator=Allocator<T>,
         class Constructor=Constructor<T>>
class Vector
{
public:
    typedef T ElementType;
    typedef Ziqe::SizeType SizeType;

    // TODO: change it to UniquePointer.
    typedef T* PointerType;
    typedef T* Iterator;
    typedef const T* ConstIterator;

    Vector() = default;

    template<class InputIterator>
    Vector(const InputIterator &begin, const InputIterator &end)
        : Vector{begin, end, iteratorsRange(begin, end)}
    {
    }

    static Vector TakeUniqueArray (UniquePointer<T[]> &&array)
    {
         return Vector{Utils::move (array)};
    }

    template<class InputIterator>
    Vector(const InputIterator &begin,
           const InputIterator &end,
           SizeType beginToEnd,
           Allocator &&allocator = Allocator{},
           Constructor &&constructor = Constructor{})
        : mAllocator{move (allocator)},
          mConstructor{move (constructor)}
    {
        assign (begin, end, beginToEnd);
    }

    ~Vector()
    {
        deleteAll (mPointer, mSize);
    }

    Vector(Vector &&other)
        : mPointer{other.mPointer},
          mSize{other.mSize},
          mAllocator{move(other.mAllocator)},
          mConstructor{move (other.mConstrutor)}
    {
        other.makeEmpty ();
    }

    Vector &operator = (Vector &&other) {
        // Delete the current data.
        deleteAll (mPointer, mSize);

        // Swap the empty *this with @a other.
        swap (*this, other);

        return *this;
    }

    Vector(const Vector &other)
        : Vector{other.mPointer, other.mPointer+other.mSize, other.mSize,
                 copy(other.mAllocator), copy(other.mConstructor)}
    {
    }

    ZQ_DEFINE_CONST_AND_NON_CONST (const T&, T&, operator[], (SizeType index),
    {
        return *(begin() + index);
    })

    Vector &operator = (const Vector &other) {
        // assign will deleteAll for us.
        assign (other.mPointer, other.mPointer+other.mSize, other.mSize);

        return *this;
    }

    ZQ_DEFINE_CONST_AND_NON_CONST (RawArray<const T>, RawArray<T>, toRawArray, (),
    {
        return {data (), size ()};
    })

    template<class InputIterator>
    void assign(const InputIterator &begin, const InputIterator &end)
    {
        return assign (begin, end, iteratorsRange(begin, end));
    }

    template<class InputIterator>
    void assign(const InputIterator &begin,
                const InputIterator &end,
                SizeType beginToEnd)
    {
       resizeBuffer (beginToEnd);
       insertToUninitilizedBuffer (0, begin, end);
    }

    template<class ...Args>
    void resize(SizeType newSize, Args &&...args) {
        if (newSize == mSize)
            return;

        if (newSize == 0) {
            deleteAll (mPointer, mSize);
            mPointer = nullptr;
            mSize = 0;

            return;
        }

        SizeType oldSize = mSize;
        resizeBuffer (newSize);

        // Construct the new objects (If there are).
        if (newSize > oldSize)
            mConstructor.constructN (mPointer+oldSize, newSize-oldSize, Utils::forward<Args>(args)...);
    }

    void shrinkWithoutFree(SizeType howMuch) {
        ZQ_ASSERT (mSize >= howMuch);

        mSize -= howMuch;
    }

    template<class ...Args>
    void expand (SizeType size, Args&&...args)
    {
        resize (mSize + size, Utils::forward<Args>(args)...);
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
     * @brief Insert a few pairs of [begin,end] or triples of [begin,end,(end-begin)]
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

    void swap(Vector &other) {
        Utils::swap (mPointer, other.mPointer);
        Utils::swap (mSize, other.mSize);
        Utils::swap (mConstructor, other.mConstructor);
        Utils::swap (mAllocator, other.mAllocator);
    }

private:
    Vector(UniquePointer<T[]> &&array)
        : mPointer{array.release ()}, mSize{array.size ()}
    {
        array.reset ();
    }

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

    void makeEmpty () {
        mPointer = nullptr;
        mSize = 0;
    }

    void resizeBuffer (SizeType newSize) {
        if (mSize == newSize) {
            // destruct without free ing.
            mConstructor.destruct (mPointer, mSize);
            return;
        }

        PointerType pointer{Utils::move (mPointer)};
        auto pointerSize = mSize;

        if (newSize != 0) {
            mPointer = mAllocator.allocate (newSize);
            mSize = newSize;
        } else {
            makeEmpty();
        }

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
            mConstructor.construct (&mPointer[currentIndex], *iterator);
        }

        return currentIndex-index;
    }

    void deleteAll (PointerType pointer, SizeType size) {
        if (size == 0)
            return;

        mConstructor.destruct (pointer, size);
        mAllocator.deallocate (pointer);
    }

    PointerType mPointer = nullptr;
    SizeType mSize = 0;

    Allocator mAllocator;
    Constructor mConstructor;
};

} // namespace Utils
ZQ_END_NAMESPACE

#endif // ZIQE_VECTOR_H
