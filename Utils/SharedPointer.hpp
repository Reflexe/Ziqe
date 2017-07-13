/**
 * @file SharedPointer.hpp
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
#ifndef SHAREDPOINTER_HPP
#define SHAREDPOINTER_HPP

#include "Utils/Macros.hpp"
#include "Utils/Memory.hpp"
#include "Utils/RawPointer.hpp"

ZQ_BEGIN_NAMESPACE
namespace Utils {

// TODO:  make CountType atomic: write an atomic API.
template<class T, class DeleterType>
struct _SharedPointerReferenceType
{
    typedef SizeType CountType;

    struct WeakContainer {
        WeakContainer(RawPointer<_SharedPointerReferenceType> reference)
            : mReference{reference}
        {
            ++(reference->mWeakCount);
        }

        ~WeakContainer()
        {
            if (--(mReference->mWeakCount) == 0) {
                delete mReference.get ();
            }
        }

        ZQ_DEFINE_CONST_AND_NON_CONST(const _SharedPointerReferenceType&,
                                      _SharedPointerReferenceType,
                                      get,
                                      (),
        {
            return *mReference;
        })

    private:
        RawPointer<_SharedPointerReferenceType> mReference;
    };

    struct SharedContainer {
        SharedContainer(RawPointer<_SharedPointerReferenceType> reference)
            : mWeakContainer{reference}
        {
            ++(mWeakContainer.get().mReferenceCount);
        }

        ~SharedContainer()
        {
            --(mWeakContainer.get().mReferenceCount);
        }

        ZQ_DEFINE_CONST_AND_NON_CONST(const _SharedPointerReferenceType&,
                                      _SharedPointerReferenceType,
                                      get,
                                      (),
        {
            return mWeakContainer.get ();
        })

    private:
        WeakContainer mWeakContainer;
    };

    _SharedPointerReferenceType() = default;
    explicit _SharedPointerReferenceType(CountType count)
        : mReferenceCount{count}
    {}

    T *mPointer;

    DeleterType mDeleter;

    // The number of count-1
    // 0 -> 1
    // 1 -> 2
    CountType mReferenceCount = 0;

    // Reference count for the reference type.
    CountType mWeakCount = 0;
};

template<class T, class Deleter>
class SharedPointerBase
{
public:
    template<class OtherT, class OtherDeleter>
    friend class SharedPointerBase;

    template<class OtherT, class OtherDeleter>
    friend class WeakPointer;

    typedef _SharedPointerReferenceType<T,Deleter> _ReferenceType;
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
        decreaseCountAndDeleteReferece ();
    }

    template<class OtherT, class OtherDeleter>
    SharedPointerBase &operator= (SharedPointerBase<OtherT, OtherDeleter> &&other) {
        decreaseCountAndDeleteReferece ();

        mPointer = other.mPointer;
        mReference = other.mReference;

        other.mPointer = nullptr;
        other.mReference = nullptr;

        return *this;
    }

    template<class OtherT, class OtherDeleter>
    SharedPointerBase &operator= (const SharedPointerBase<OtherT, OtherDeleter> &other) {
        decreaseCountAndDeleteReferece ();

        mPointer = other.mPointer;
        mReference = other.mReference;

        increaseCount ();

        return *this;
    }

    SharedPointerBase &operator= (SharedPointerBase &&other) {
        swap (other);

        return *this;
    }

    SharedPointerBase &operator= (const SharedPointerBase &other) {
        mReference = other.mReference;

        return *this;
    }

    void reset (T *pointer = nullptr)
    {
        swap (SharedPointer(pointer));
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
            return mReference->mReferenceCount+1;
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

    void swap (SharedPointerBase &otherPointer)
    {
        Utils::swap (mReference, otherPointer.mReference);
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

    void decreaseCountAndDeleteReferece ()
    {
        decreaseCount ();
    }

    void decreaseCount(bool shouldDeleteReferece=false) {
        // if count==0
        if (mPointer == nullptr)
            return;

        // if --count == 0
        if (mReference == nullptr) {
            deleteObject ();
        } else if (mReference->mReferenceCount == 0) {
            deleteObject ();

            if (shouldDeleteReferece)
                deleteReference ();
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

template<class T, class Deleter=DefaultDeleter<T>>
class WeakPointer
{
public:
    typedef _SharedPointerReferenceType<T,Deleter> _ReferenceType;
    typedef typename _ReferenceType::CountType CountType;

    WeakPointer(const SharedPointer<T, Deleter> &sharedPointer)
        : mReference{sharedPointer.mReference}
    {
    }

    bool isExpired () const
    {
        return mReference->mReferenceCount == 0;
    }

    SharedPointer<T, Deleter> lock()
    {
        return SharedPointer<T, Deleter>{mReference};
    }

private:
    typename _ReferenceType::WeakContainer mReference;
};

template<class T, class ...Args>
SharedPointer<T> makeShared(Args&&... args)
{
    return SharedPointer<T>{new T{Utils::forward<Args>(args)...}};
}

} // namespace Utils
ZQ_END_NAMESPACE

#endif // SHAREDPOINTER_HPP
