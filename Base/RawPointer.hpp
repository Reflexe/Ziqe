/**
 * @file RawPointer.hpp
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
#ifndef RAWPOINTER_HPP
#define RAWPOINTER_HPP

#include "Base/Macros.hpp"

ZQ_BEGIN_NAMESPACE
namespace Base {

template<class T>
class RawPointerBase
{
public:
    template<class OtherT>
    friend class RawPointerBase;

    RawPointerBase() = default;

    RawPointerBase(T *pointer)
        : mPointer{pointer}
    {
    }

    operator T* () const
    {
        return mPointer;
    }

    ZQ_ALLOW_COPY_AND_MOVE (RawPointerBase)

    void reset (T *pointer = nullptr)
    {
        mPointer = pointer;
    }

    operator bool ()
    {
        return mPointer != nullptr;
    }

    T *get() const
    {
        return mPointer;
    }

    T &operator *() const
    {
        return *mPointer;
    }

    T *operator ->() const
    {
        return mPointer;
    }

    operator T& () const
    {
        return *(this);
    }

    ZQ_DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (RawPointerBase, mPointer)

protected:
    T *mPointer = nullptr;
};

template<class T>
class RawPointer : public RawPointerBase<T>
{
public:
    typedef RawPointerBase<T> _RawPointerBase;

    using _RawPointerBase::operator =;
    using _RawPointerBase::operator ==;
    using _RawPointerBase::operator *;
    using _RawPointerBase::operator ->;
    using _RawPointerBase::operator bool;
    using _RawPointerBase::operator T&;

    using _RawPointerBase::RawPointerBase;
};


template<class T>
class RawPointer<T[]> : public RawPointerBase<T>
{
public:
    typedef RawPointerBase<T> _RawPointerBase;

    using _RawPointerBase::operator ==;
    using _RawPointerBase::operator *;
    using _RawPointerBase::operator ->;
    using _RawPointerBase::operator bool;
    using _RawPointerBase::operator T&;

    // Don't allow converting of array.
    RawPointer() = default;
    RawPointer(T *ptr)
        : _RawPointerBase{ptr}
    {
    }

    ZQ_ALLOW_COPY_AND_MOVE (RawPointer)

    T &operator [](SizeType index) const
    {
        return this->mPointer[index];
    }
};

template<class T>
class RawArray : public RawPointer<T[]>
{
public:
    typedef RawPointer<T[]> _PointerType;

    explicit RawArray () = default;
    RawArray(T *ptr, SizeType size)
        : _PointerType{ptr}, mSize{size}
    {
    }

    // prefer copy consturctor on this one (that's why the class = int)
    template<class = int>
    RawArray(const RawArray<typename RemoveConst<T>::type> &nonconstArray)
        : _PointerType{nonconstArray.get ()}, mSize{nonconstArray.size ()}
    {
    }

    ZQ_ALLOW_COPY_AND_MOVE (RawArray)

    bool operator ==(const RawArray &other)
    {
        return mSize == other.mSize && this->operator == (static_cast<const _PointerType&>(other));
    }

    T &operator [](SizeType index) const
    {
        return this->mPointer[index];
    }

    SizeType size() const
    {
        return mSize;
    }

    typedef T* Iterator;
    typedef const T* ConstIterator;

    Iterator begin() const
    {
        return this->get();
    }

    Iterator end() const
    {
        return this->get() + mSize;
    }

    ConstIterator cbegin () const
    {
        return begin ();
    }

    ConstIterator cend () const
    {
        return end ();
    }
private:
    SizeType mSize;
};

/**
  @brief Convert every object to a RawArray of bytes (uint8_t).
 */
template<class T>
RawArray<uint8_t> makeBytesArray (T &object, SizeType size=sizeof (T))
{
    return RawArray<uint8_t>{reinterpret_cast<uint8_t*>(&object), size};
}

template<class T>
RawArray<const uint8_t> makeBytesArray (const T &object, SizeType size=sizeof (T))
{
    return RawArray<const uint8_t>{reinterpret_cast<const uint8_t*>(&object), size};
}

} // namespace Base
ZQ_END_NAMESPACE

#endif // RAWPOINTER_HPP
