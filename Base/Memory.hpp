/**
 * @file Memory.hpp
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
#ifndef ZIQE_MEMORY_H
#define ZIQE_MEMORY_H

#include "Types.hpp"
#include "Macros.hpp"
#include "Checks.hpp"

#include "ZiqeAPI/Memory.h"

void* operator new  (std::size_t count);
void operator delete (void *pointer) noexcept;

// Placement
inline void *operator new(std::size_t, void *p) noexcept{ return p; }
inline void *operator new[](std::size_t, void *p)  noexcept{ return p; }
inline void  operator delete  (void *, void *) noexcept { }
inline void  operator delete[](void *, void *) noexcept { }

namespace Ziqe {
namespace Base {

template<class T>
struct Allocator {
    T* allocate(SizeType n) {
        n *= sizeof(T);
        return static_cast<T*>(::operator new(n));
    }

    void deallocate(T *p)
    {
        ::operator delete(p);
    }
};

/**
  @brief A constructor that can construct a @tparam T on element from
         StorageType that atleast in sizeof(T).
 */
template<class T, class StorageType>
struct CustomStorageConstructor {
    template<class ...Args>
    T *constructN(StorageType *pointer, SizeType n, Args ...args)
    {
        return ::new(static_cast<void*>(pointer)) T[n]{Base::forward<Args>(args)...};
    }

    template<class ...Args>
    T *construct(StorageType *pointer, Args&&...args)
    {
        return ::new(static_cast<void*>(pointer)) T{Base::forward<Args>(args)...};
    }

    /// These function receives T* and not StorageType* because of
    /// strict aliasing rule.
    void destruct(T *pointer, SizeType n) {
        // Destruct in reverse
        while (n--)
            pointer[n].~T();
    }

    void destruct(T *pointer)
    {
        pointer->~T();
    }
};

/**
  @brief A default constructor that uses its own type
         as StorageType.

  In the most case, it should work.
 */
template<class T>
using Constructor=CustomStorageConstructor<T, T>;


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

// TODO:  make CountType atomic: write an atomic API.
template<class DeleterType>
struct _SharedPointerReferenceType
{
    typedef SizeType CountType;

    _SharedPointerReferenceType() = default;
    explicit _SharedPointerReferenceType(CountType count)
        : mReferenceCount{count}
    {}

    DeleterType mDeleter;

    // The number of count-1
    CountType mReferenceCount = 0;
};

template<class T, class Deleter>
class SharedPointerBase
{
public:
    template<class OtherT, class OtherDeleter>
    friend class SharedPointerBase;

    typedef _SharedPointerReferenceType<Deleter> _ReferenceType;
    typedef typename _ReferenceType::CountType CountType;

    SharedPointerBase()
        : mPointer{nullptr}, mReference{nullptr}
    {
    }

    explicit SharedPointerBase(T *pointer)
        : mPointer{pointer}, mReference{nullptr}
    {
    }

    template<class OtherT, class OtherDeleter>
    SharedPointerBase(const SharedPointerBase<OtherT, OtherDeleter> &other)
        : mPointer{other.mPointer}, mReference{other.mReference}
    {
        increaseCount ();
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
        increaseCount ();
    }

    ~SharedPointerBase()
    {
        decreaseCount ();
    }

    template<class OtherT, class OtherDeleter>
    SharedPointerBase &operator= (SharedPointerBase<OtherT, OtherDeleter> &&other) {
        decreaseCount ();
        deleteReference ();

        mPointer = other.mPointer;
        mReference = other.mReference;

        other.mPointer = nullptr;
        other.mReference = nullptr;

        return *this;
    }

    template<class OtherT, class OtherDeleter>
    SharedPointerBase &operator= (const SharedPointerBase<OtherT, OtherDeleter> &other) {
        decreaseCount ();
        deleteReference ();

        mPointer = other.mPointer;
        mReference = other.mReference;

        increaseCount ();

        return *this;
    }

    SharedPointerBase &operator= (SharedPointerBase &&other) {
        decreaseCount ();
        deleteReference ();

        mPointer = other.mPointer;
        mReference = other.mReference;

        other.mPointer = nullptr;
        other.mReference = nullptr;

        return *this;
    }

    SharedPointerBase &operator= (const SharedPointerBase &other) {
        decreaseCount ();
        deleteReference ();

        mPointer = other.mPointer;
        mReference = other.mReference;

        increaseCount ();

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

    ZQ_DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (SharedPointerBase, mPointer)

    CountType getReferenceCount () const{
        if (mPointer == nullptr)
            return 0;
        else if (mReference == nullptr)
            return 1;
        else
            return mReference->mReferenceCount;
    }

    operator bool () const
    {
        return mPointer != nullptr;
    }

    operator T& ()
    {
        return operator *();
    }

    operator const T& ()
    {
        return operator *();
    }

protected:
    void setPointer (T *pointer) {
        decreaseCount ();

        // leave mReference as it is: nullptr, or 0: one anyway.
        mPointer = pointer;
    }

    void increaseCount ()  {
        // if we have no object.
        if (mPointer == nullptr)
            return;

        // 1 -> 2
        if (mReference == nullptr)
            mReference = new _ReferenceType{1};
        else
            ++(mReference->mReferenceCount);
    }

    void decreaseCount() {
        // if count==0
        if (mPointer == nullptr)
            return;

        // if --count == 0
        if (mReference == nullptr) {
            deleteObject ();
        } else {
            if (mReference->mReferenceCount == 0)
                deleteObject ();
        }

        mReference = nullptr;
        mPointer = nullptr;
    }

    void deleteObject () {
        ZQ_ASSERT (mPointer != nullptr);

        mReference->mDeleter (mPointer);
    }

    void deleteReference () {
        if (mReference != nullptr)
            delete mReference;
    }

    T *mPointer;

    /**
     * @brief mReference
     *
     * If nullptr: count=1; else, count=ref.count+1.
     */
    _ReferenceType *mReference;
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
    using _SharedPointerBase::operator bool;
    using _SharedPointerBase::operator T&;
    using _SharedPointerBase::operator const T&;

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
    using _SharedPointerBase::operator bool;
    using _SharedPointerBase::operator T&;
    using _SharedPointerBase::operator const T&;

    // Don't allow cenvertion of an array.
    SharedPointer() = default;
    explicit SharedPointer(T *ptr)
        : _SharedPointerBase{ptr}
    {
    }

    ZQ_ALLOW_COPY_AND_MOVE (SharedPointer)

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
class RawPointerBase
{
public:
    template<class OtherT, class OtherDeleter>
    friend class RawPointerBase;

    RawPointerBase() = default;

    RawPointerBase(T *pointer)
        : mPointer{pointer}
    {
    }

    operator T* ()
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

    operator const T& () const
    {
        return *(this);
    }

    ZQ_DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (RawPointerBase, mPointer)

protected:
    T *mPointer = nullptr;
};

template<class T, class Deleter=DefaultDeleter<T>>
class RawPointer : public RawPointerBase<T, Deleter>
{
public:
    typedef RawPointerBase<T, Deleter> _RawPointerBase;

    using _RawPointerBase::operator =;
    using _RawPointerBase::operator ==;
    using _RawPointerBase::operator *;
    using _RawPointerBase::operator ->;
    using _RawPointerBase::operator bool;
    using _RawPointerBase::operator T&;

    using _RawPointerBase::RawPointerBase;
};


template<class T, class Deleter>
class RawPointer<T[], Deleter> : public RawPointerBase<T, Deleter>
{
public:
    typedef RawPointerBase<T, Deleter> _RawPointerBase;

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

    typedef T* Iterator;
    typedef const T* ConstIterator;

    ZQ_DEFINE_CONST_AND_NON_CONST (ConstIterator, Iterator, begin, (), { return this->get (); })

    ZQ_DEFINE_CONST_AND_NON_CONST (ConstIterator, Iterator, end, (), { return this->get () + mSize; })

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
 * Class that act like a pointer but it is a regular value.
 */
template<class T>
class DummyPointer
{
public:
    template<class ...Args>
    DummyPointer(Args...args)
        : mValue{Base::forward<Args>(args)...}
    {
    }

    operator T* ()
    {
        return &mValue;
    }

    operator bool ()
    {
        return true;
    }

    T *get()
    {
        return &mValue;
    }

    const T *get() const
    {
        return &mValue;
    }

    T &operator *()
    {
        return mValue;
    }

    const T &operator *() const
    {
        return mValue;
    }

    T *operator ->()
    {
        return &mValue;
    }

    const T *operator ->() const
    {
        return &mValue;
    }

    operator T& ()
    {
        return *(this);
    }

    operator const T& () const
    {
        return *(this);
    }

private:
    T mValue;
};

template<class T, class ...Args>
UniquePointer<T> makeUnique(Args&&... args)
{
    return UniquePointer<T>{new T{Base::forward<Args>(args)...}};
}

template<class T, class ...Args>
SharedPointer<T> makeShared(Args&&... args)
{
    return SharedPointer<T>{new T{Base::forward<Args>(args)...}};
}

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
} // namespace Ziqe

#endif // ZIQE_MEMORY_H
