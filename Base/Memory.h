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
    return reinterpret_cast<void*>(ZqAllocate (static_cast<ZqRegisterType>(count)));
}

void operator delete (void *pointer) noexcept
{
    ZqDeallocate (reinterpret_cast<ZqRegisterType>(pointer));
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

template<class T, class Deleter=DefaultDeleter<T>>
class UniquePointer
{
public:
    template<class OtherT, class OtherDeleter>
    friend class SharedPointer;

    UniquePointer() = default;

    template<class Tp>
    UniquePointer(Tp *pointer)
        : mPointer{pointer}
    {
    }

    template<class OtherT, class OtherDeleter>
    UniquePointer(UniquePointer<OtherT, OtherDeleter> &&other)
        : mPointer{other.mPointer}
    {
        other.mPointer = nullptr;
    }

    template<class OtherT, class OtherDeleter>
    UniquePointer &operator= (UniquePointer<OtherT, OtherDeleter> &&other) {
        reset (other.mPointer);
        other.mPointer = nullptr;
    }

    UniquePointer(UniquePointer &&other)
        : mPointer{other.mPointer}
    {
        other.mPointer = nullptr;
    }

    UniquePointer &operator= (UniquePointer &&other) {
        reset (other.mPointer);
        other.mPointer = nullptr;
    }

    template<class Tp>
    UniquePointer &operator= (Tp *pointer)
    {
        reset (pointer);
    }

    DISALLOW_COPY (UniquePointer)

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

    DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (UniquePointer, mPointer)

private:
    T *mPointer = nullptr;
};

template<class T, class Deleter>
class UniquePointer<T[], Deleter>
{
public:
    template<class OtherT, class OtherDeleter>
    friend class UniquePointer;

    UniquePointer() = default;

    ~UniquePointer() {
        if (*this)
            Deleter{}(mPointer);
    }

    template<class Tp>
    UniquePointer(Tp *pointer)
        : mPointer{pointer}
    {
    }

    template<class OtherT, class OtherDeleter>
    UniquePointer(UniquePointer<OtherT, OtherDeleter> &&other)
        : mPointer{other.mPointer}
    {
        other.mPointer = nullptr;
    }

    template<class OtherT, class OtherDeleter>
    UniquePointer &operator= (UniquePointer<OtherT, OtherDeleter> &&other) {
        reset (other.mPointer);
        other.mPointer = nullptr;
    }


    UniquePointer(UniquePointer &&other)
        : mPointer{other.mPointer}
    {
        other.mPointer = nullptr;
    }

    UniquePointer &operator= (UniquePointer &&other) {
        reset (other.mPointer);
        other.mPointer = nullptr;
    }

    template<class Tp>
    UniquePointer &operator= (Tp *pointer)
    {
        reset (pointer);
    }

    DISALLOW_COPY (UniquePointer)

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

    T &operator [](SizeType index)
    {
        return mPointer[index];
    }

    const T &operator [](SizeType index) const
    {
        return mPointer[index];
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

    DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (UniquePointer, mPointer)

private:
    T *mPointer = nullptr;
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

template<class T, class Deleter=DefaultDeleter<T>>
class SharedPointer
{
public:
    template<class OtherT, class OtherDeleter>
    friend class SharedPointer;

    typedef _ReferenceType::CountType CountType;

    SharedPointer() = default;

    SharedPointer(T *pointer)
    {
        setPointer (pointer);
    }

    template<class OtherT, class OtherDeleter>
    SharedPointer(const SharedPointer<OtherT, OtherDeleter> &other)
        : mPointer{other.mPointer}, mReference{other.mReference}
    {
        ++(mReference->mReferenceCount);
    }

    template<class OtherT, class OtherDeleter>
    SharedPointer(SharedPointer<OtherT, OtherDeleter> &&other)
        : mPointer{other.mPointer}, mReference{other.mReference}
    {
        other.mPointer = nullptr;
        other.mReference = nullptr;
    }

    SharedPointer(SharedPointer &&other)
        : mPointer{other.mPointer}, mReference{other.mReference} {
        other.mPointer = nullptr;
        other.mReference = nullptr;
    }

    SharedPointer(const SharedPointer &other)
        : mPointer{other.mPointer}, mReference{other.mReference}
    {
        ++(mReference->mReferenceCount);
    }

    ~SharedPointer()
    {
        decreaseCount ();
    }

    template<class OtherT, class OtherDeleter>
    SharedPointer &operator= (SharedPointer<OtherT, OtherDeleter> &&other) {
        decreaseCount ();

        mPointer = other.mPointer;
        mReference = other.mReference;

        other.mPointer = nullptr;
        other.mReference = nullptr;

        return *this;
    }

    template<class OtherT, class OtherDeleter>
    SharedPointer &operator= (const SharedPointer<OtherT, OtherDeleter> &other) {
        decreaseCount ();

        mPointer = other.mPointer;
        mReference = other.mReference;

        ++(mReference->mReferenceCount);

        return *this;
    }

    SharedPointer &operator= (SharedPointer &&other) {
        decreaseCount ();

        mPointer = other.mPointer;
        mReference = other.mReference;

        other.mPointer = nullptr;
        other.mReference = nullptr;

        return *this;
    }

    SharedPointer &operator= (const SharedPointer &other) {
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

    T &operator [](SizeType index)
    {
        return mPointer[index];
    }

    const T &operator [](SizeType index) const
    {
        return mPointer[index];
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

    CountType getReferenceCount () const
    {
        return (! mReference) ? 0 : mReference->mReferenceCount;
    }

    operator bool () const
    {
        return mPointer != nullptr;
    }

private:
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

//template<class T>
//class WeakPointer
//{
//public:
//    WeakPointer(const SharedPointer<T> &pointer)
//    {
//        (void) pointer;
//    }

//    void isExpired();

//    void lock();
    
//private:
    
//};

template<class T, class Deleter=DefaultDeleter<T>>
class UglyPointer
{
public:
    UglyPointer()
    {
    }

    ~UglyPointer()
    {
    }

    UglyPointer(T *pointer)
        : mPointer{pointer}
    {
    }

    operator T* ()
    {
        return mPointer;
    }

    ALLOW_COPY_AND_MOVE (UglyPointer)

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

    DEFINE_EQUAL_AND_NOT_EQUAL_BY_MEMBER (UglyPointer, mPointer)

private:
    T *mPointer = nullptr;
};


/**
 * @brief A little helper to copy a lvalue and return the copy as a prvalue.
 */
template<class T>
T copy (const T &value)
{
    return value;
}

template<class T, class ...Args>
UniquePointer<T> makeUnique(Args&&... args)
{
    return UniquePointer<T>{new T{std::forward<Args>(args)...}};
}

} // namespace Ziqe

#endif // ZIQE_MEMORY_H
