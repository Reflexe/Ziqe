/**
 * @file Memory.h
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
#ifndef ZIQE_MEMORY_H
#define ZIQE_MEMORY_H

#include "Types.h"
#include "Macros.h"

#include "ZiqeAPI/Memory.h"

#include <utility>

void* operator new  (std::size_t count) {
    static_assert (sizeof (ZqRegisterType) >= sizeof (void *),
                   "Invalid ZqRegisterType");
    return reinterpret_cast<void*>(ZqAllocateVirtual (static_cast<ZqRegisterType>(count)));
}

void operator delete (void *pointer) noexcept
{
    ZqDeallocateVirtual (reinterpret_cast<ZqRegisterType>(pointer));
}

// Placement
inline void *operator new(std::size_t, void *p) noexcept{ return p; }
inline void *operator new[](std::size_t, void *p)  noexcept{ return p; }
inline void  operator delete  (void *, void *) noexcept { }
inline void  operator delete[](void *, void *) noexcept { }

namespace Ziqe {

template<class T>
struct Allocator {
    T* allocate(SizeType n) {
        n *= sizeof(T);
        return static_cast<T*>(::operator new(n));
    }

    void deallocate(T* p, SizeType)
    {
        ::operator delete(p);
    }
};

template<class T>
struct Constructor {
    template<class ...Args>
    void constructN(T *pointer, SizeType n, Args ...args)
    {
        ::new(static_cast<void*>(pointer)) T[n]{std::forward<Args>(args)...};
    }

    template<class ...Args>
    void construct(T *pointer, Args&&...args)
    {
        ::new(static_cast<void*>(pointer)) T{std::forward<Args>(args)...};
    }

    void destruct(T *pointer, SizeType n) {
        for (SizeType i = 0; i < n; ++i)
            pointer[i].~T();
    }

    void destruct(T *pointer)
    {
        pointer->~T();
    }
};

template<class T>
struct DefaultDeleter {
    void operator() (T *pointer)
    {
        delete pointer;
    }
};

template<class T>
struct DefaultDeleter<T[]> {
    void operator() (T *pointer)
    {
        delete[] pointer;
    }
};

template<class T>
struct NoDeleter {
    void operator() (T *)
    {
    }
};

template <class T, class Deleter>
class UniquePointerBase
{
    template<class OtherT, class OtherDeleter>
    friend class UniquePointer;

    UniquePointerBase() = default;

    template<class Tp>
    UniquePointerBase(Tp *pointer)
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
    }

    template<class Tp>
    UniquePointerBase &operator= (Tp *pointer)
    {
        reset (pointer);
    }

    DISALLOW_COPY (UniquePointerBase)

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

    operator bool ()
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

    DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (UniquePointerBase, mPointer)

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

    using _UniquePointerBase::UniquePointerBase;
};

template<class T, class Deleter>
class UniquePointer<T[], Deleter> : public UniquePointerBase<T, Deleter>
{
public:
    typedef UniquePointerBase<T, Deleter> _UniquePointerBase;

    using _UniquePointerBase::operator ==;
    using _UniquePointerBase::operator *;
    using _UniquePointerBase::operator ->;

    // Don't allow converting of array.
    UniquePointer() = default;
    UniquePointer(T *ptr)
        : _UniquePointerBase{ptr}
    {
    }

    ALLOW_COPY_AND_MOVE (UniquePointer)

    T &operator [](SizeType index)
    {
        return this->mPointer[index];
    }

    const T &operator [](SizeType index) const
    {
        return this->mPointer[index];
    }
};

struct _ReferenceType
{
    typedef SizeType CountType;

    _ReferenceType() = default;
    _ReferenceType(CountType count)
        : mReferenceCount{count}
    {}

    CountType mReferenceCount = 0;
};

template<class T, class Deleter>
class SharedPointerBase
{
public:
    template<class OtherT, class OtherDeleter>
    friend class SharedPointerBase;

    typedef _ReferenceType::CountType CountType;

    SharedPointerBase() = default;

    SharedPointerBase(T *pointer)
    {
        setPointer (pointer);
    }

    template<class OtherT, class OtherDeleter>
    SharedPointerBase(const SharedPointerBase<OtherT, OtherDeleter> &other)
        : mPointer{other.mPointer}, mReference{other.mReference}
    {
        ++(mReference->mReferenceCount);
    }

    template<class OtherT, class OtherDeleter>
    SharedPointerBase(SharedPointerBase<OtherT, OtherDeleter> &&other)
        : mPointer{other.mPointer}, mReference{other.mReference}
    {
        other.mPointer = nullptr;
        other.mReference = nullptr;
    }

    SharedPointerBase(SharedPointerBase &&other)
        : mPointer{other.mPointer}, mReference{other.mReference} {
        other.mPointer = nullptr;
        other.mReference = nullptr;
    }

    SharedPointerBase(const SharedPointerBase &other)
        : mPointer{other.mPointer}, mReference{other.mReference}
    {
        ++(mReference->mReferenceCount);
    }

    ~SharedPointerBase()
    {
        decreaseCount ();
    }

    template<class OtherT, class OtherDeleter>
    SharedPointerBase &operator= (SharedPointerBase<OtherT, OtherDeleter> &&other) {
        decreaseCount ();

        mPointer = other.mPointer;
        mReference = other.mReference;

        other.mPointer = nullptr;
        other.mReference = nullptr;

        return *this;
    }

    template<class OtherT, class OtherDeleter>
    SharedPointerBase &operator= (const SharedPointerBase<OtherT, OtherDeleter> &other) {
        decreaseCount ();

        mPointer = other.mPointer;
        mReference = other.mReference;

        ++(mReference->mReferenceCount);

        return *this;
    }

    SharedPointerBase &operator= (SharedPointerBase &&other) {
        decreaseCount ();

        mPointer = other.mPointer;
        mReference = other.mReference;

        other.mPointer = nullptr;
        other.mReference = nullptr;

        return *this;
    }

    SharedPointerBase &operator= (const SharedPointerBase &other) {
        decreaseCount ();

        mPointer = other.mPointer;
        mReference = other.mReference;

        ++(mReference->mReferenceCount);

        return *this;
    }

    void reset (T *pointer = nullptr) {
        decreaseCount ();

        setPointer (pointer);
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

    DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (SharedPointerBase, mPointer)

    CountType getReferenceCount () const
    {
        return (! mReference) ? 0 : mReference->mReferenceCount;
    }

    operator bool () const
    {
        return mPointer != nullptr;
    }

protected:
    void setPointer (T *pointer) {
        // Don't waste memory on reference this is a nullptr.
        mReference = (pointer != nullptr) ? new _ReferenceType{1} : nullptr;
        mPointer = pointer;
    }

    void decreaseCount() {
        if (mReference->mReferenceCount == 0)
            return;

        --(mReference->mReferenceCount);

        if (mReference->mReferenceCount == 0) {
            Deleter{}(mPointer);
            delete mReference;
        }

        mPointer = nullptr;
    }

    T *mPointer;
    _ReferenceType *mReference = nullptr;
};

template<class T, class Deleter=DefaultDeleter<T>>
class SharedPointer : public SharedPointerBase<T, Deleter>
{
public:
    typedef SharedPointerBase<T, Deleter> _SharedPointerBase;

    using _SharedPointerBase::operator =;
    using _SharedPointerBase::operator ==;
    using _SharedPointerBase::operator *;
    using _SharedPointerBase::operator ->;

    using _SharedPointerBase::SharedPointerBase;
};

template<class T, class Deleter>
class SharedPointer<T[], Deleter> : public SharedPointerBase<T, Deleter>
{
public:
    typedef SharedPointerBase<T, Deleter> _SharedPointerBase;

    using _SharedPointerBase::operator ==;
    using _SharedPointerBase::operator *;
    using _SharedPointerBase::operator ->;

    // Don't allow converting of array.
    SharedPointer() = default;
    SharedPointer(T *ptr)
        : _SharedPointerBase{ptr}
    {
    }

    ALLOW_COPY_AND_MOVE (SharedPointer)

    T &operator [](SizeType index)
    {
        return this->mPointer[index];
    }

    const T &operator [](SizeType index) const
    {
        return this->mPointer[index];
    }
};

template<class T, class Deleter>
class UglyPointerBase
{
public:
    template<class OtherT, class OtherDeleter>
    friend class UglyPointerBase;

    UglyPointerBase() = default;

    UglyPointerBase(T *pointer)
        : mPointer{pointer}
    {
    }

    operator T* ()
    {
        return mPointer;
    }

    ALLOW_COPY_AND_MOVE (UglyPointerBase)

    void reset (T *pointer = nullptr)
    {
        mPointer = pointer;
    }

    operator bool ()
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

    DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (UglyPointerBase, mPointer)

protected:
    T *mPointer = nullptr;
};

template<class T, class Deleter=DefaultDeleter<T>>
class UglyPointer : public UglyPointerBase<T, Deleter>
{
public:
    typedef UglyPointerBase<T, Deleter> _UglyPointerBase;

    using _UglyPointerBase::operator =;
    using _UglyPointerBase::operator ==;
    using _UglyPointerBase::operator *;
    using _UglyPointerBase::operator ->;

    using _UglyPointerBase::UglyPointerBase;
};


template<class T, class Deleter>
class UglyPointer<T[], Deleter> : public UglyPointerBase<T, Deleter>
{
public:
    typedef UglyPointerBase<T, Deleter> _UglyPointerBase;

    using _UglyPointerBase::operator ==;
    using _UglyPointerBase::operator *;
    using _UglyPointerBase::operator ->;

    // Don't allow converting of array.
    UglyPointer() = default;
    UglyPointer(T *ptr)
        : _UglyPointerBase{ptr}
    {
    }

    ALLOW_COPY_AND_MOVE (UglyPointer)

    T &operator [](SizeType index)
    {
        return this->mPointer[index];
    }

    const T &operator [](SizeType index) const
    {
        return this->mPointer[index];
    }
};

template<class T>
class UglyArray : public UglyPointer<T[]>
{
public:
    typedef UglyPointer<T> _PointerType;

    UglyArray () = default;
    UglyArray(T *ptr, SizeType size)
        : _PointerType{ptr}, mSize{size}
    {
    }

    ALLOW_COPY_AND_MOVE (UglyArray)

    bool operator ==(const UglyArray &other)
    {
        return mSize == other.mSize && this->_PointerType == other._PointerType;
    }

    T &operator [](SizeType index)
    {
        return this->mPointer[index];
    }

    const T &operator [](SizeType index) const
    {
        return this->mPointer[index];
    }

    SizeType size() const
    {
        return mSize;
    }

private:
    SizeType mSize;
};

template<class T, class ...Args>
UniquePointer<T> makeUnique(Args&&... args)
{
    return UniquePointer<T>{new T{std::forward<Args>(args)...}};
}

} // namespace Ziqe

#endif // ZIQE_MEMORY_H
