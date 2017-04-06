/**
 * @file UniquePointer.hpp
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
#ifndef UNIQUEPOINTER_HPP
#define UNIQUEPOINTER_HPP

#include "Utils/Macros.hpp"
#include "Utils/Memory.hpp"

ZQ_BEGIN_NAMESPACE
namespace Utils {

template <class T, class Deleter>
class UniquePointerBase
{
public:
    template<class OtherT, class OtherDeleter>
    friend class UniquePointerBase;

    UniquePointerBase() = default;

    template<class Tp>
    explicit UniquePointerBase(Tp *pointer)
        : mPointer{pointer}
    {
    }

    template<class OtherT, class OtherDeleter>
    UniquePointerBase(UniquePointerBase<OtherT, OtherDeleter> &&other)
        : mPointer{other.mPointer}
    {
        other.mPointer = nullptr;
    }

    template<class OtherT, class OtherDeleter>
    UniquePointerBase &operator= (UniquePointerBase<OtherT, OtherDeleter> &&other) {
        reset (other.mPointer);
        other.mPointer = nullptr;
    }

    UniquePointerBase(UniquePointerBase &&other)
        : mPointer{other.mPointer}
    {
        other.mPointer = nullptr;
    }

    UniquePointerBase &operator= (UniquePointerBase &&other) {
        reset (other.mPointer);
        other.mPointer = nullptr;

        return *this;
    }

    template<class Tp>
    UniquePointerBase &operator= (Tp *pointer)
    {
        reset (pointer);
    }

    ZQ_DISALLOW_COPY (UniquePointerBase)

    template<class Tp>
    void reset (Tp *pointer = nullptr) {
        if (*this)
            Deleter{}(mPointer);

        mPointer = pointer;
    }

    T *release () {
        auto pointer = mPointer;
        mPointer = nullptr;

        return mPointer;
    }

    operator bool () const
    {
        return mPointer != nullptr;
    }

    T *get()
    {
        return mPointer;
    }

    const T *get() const
    {
        return mPointer;
    }

    T &operator *()
    {
        return *mPointer;
    }

    const T &operator *() const
    {
        return *mPointer;
    }

    T *operator ->()
    {
        return mPointer;
    }

    const T *operator ->() const
    {
        return mPointer;
    }

    operator T& ()
    {
        return *(this);
    }

    operator const T& ()
    {
        return *(this);
    }

    ZQ_DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (UniquePointerBase, mPointer)

protected:
    T *mPointer = nullptr;
};

template<class T, class Deleter=DefaultDeleter<T>>
class UniquePointer : public UniquePointerBase<T, Deleter>
{
public:
    typedef UniquePointerBase<T, Deleter> _UniquePointerBase;

    using _UniquePointerBase::operator =;
    using _UniquePointerBase::operator ==;
    using _UniquePointerBase::operator *;
    using _UniquePointerBase::operator ->;
    using _UniquePointerBase::operator bool;
    using _UniquePointerBase::operator T&;
    using _UniquePointerBase::operator const T&;

    using _UniquePointerBase::UniquePointerBase;
};

template<class T, class Deleter>
class UniquePointer<T[], Deleter> : public UniquePointerBase<T, Deleter>
{
public:
    typedef UniquePointerBase<T, Deleter> _UniquePointerBase;

    using _UniquePointerBase::operator ==;
    using _UniquePointerBase::operator *;
    using _UniquePointerBase::operator bool;
    using _UniquePointerBase::operator ->;
    using _UniquePointerBase::operator T&;
    using _UniquePointerBase::operator const T&;

    // Don't allow converting of array.
    UniquePointer() = default;
    explicit UniquePointer(T *ptr)
        : _UniquePointerBase{ptr}
    {
    }

    ZQ_ALLOW_COPY_AND_MOVE (UniquePointer)

    T &operator [](SizeType index)
    {
        return this->mPointer[index];
    }

    const T &operator [](SizeType index) const
    {
        return this->mPointer[index];
    }
};

template<class T, class ...Args>
UniquePointer<T> makeUnique(Args&&... args)
{
    return UniquePointer<T>{new T{Utils::forward<Args>(args)...}};
}

} // namespace Utils
ZQ_END_NAMESPACE

#endif // UNIQUEPOINTER_HPP
