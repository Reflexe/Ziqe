#ifndef ZIQE_BASE_EXPECTED_HPP
#define ZIQE_BASE_EXPECTED_HPP

#include "Utils/Macros.hpp"
#include "Utils/Checks.hpp"
#include "Utils/Constructor.hpp"

ZQ_BEGIN_NAMESPACE
namespace Utils {

/**
 * A piece of memory you can consturct objects on.
 */
template<SizeType size, SizeType alignment>
class ConstructableStorage {
public:
    template<class T, class ...Args>
    T *construct(Args&&... args) {
        CustomStorageConstructor<T,char> consturctor;

        return consturctor.construct(mStorageData, Utils::forward<Args>(args)...);
    }

    template<class T>
    void destruct () {
        CustomStorageConstructor<T,char> consturctor;
        consturctor.destruct(getAs<T>());
    }

    // Does not violate the strict aliasing rule:
    // mStorageData is no longer a array of chars but
    // a T.
    template<class T>
    T *getAs()
    {
        return static_cast<T*>(static_cast<void*>(mStorageData));
    }

    template<class T>
    T *getAs() const
    {
        return static_cast<const T*>(static_cast<void*>(mStorageData));
    }

private:
    /**
       @brief  Storage for the optional data.
     */
    alignas(alignment) char mStorageData[size];
};

using namespace Ziqe::Utils;
template<class FirstType, class SecondType>
class TwoVariant {
    typedef ConstructableStorage<max(sizeof(FirstType), sizeof(SecondType)),
                                 max(alignof(FirstType), alignof(SecondType))>
            StorageType;

public:
    TwoVariant()
    {
    }

    TwoVariant(TwoVariant &&variant)
    {
        swap (variant);
    }

    TwoVariant(const TwoVariant &variant)
        : mCurrentStatus{variant.mCurrentStatus}
    {
        constructFromOtherVariant (variant);
    }

    ~TwoVariant()
    {
        destruct ();
    }

    // Copy operator.
    TwoVariant &operator = (const TwoVariant &variant) {
        if (mCurrentStatus != variant.mCurrentStatus) {
            destruct();

            constructFromOtherVariant(variant);
            mCurrentStatus = variant.mCurrentStatus;
        } else {
            // We have the same type as the copied variant.
            // no need to destruct and reconstruct.

            switch (mCurrentStatus) {
            case CurrentStatus::FirstActive:
                getFirst() = variant.getFirst();
                break;
            case CurrentStatus::SecondActive:
                getSecond() = variant.getSecond();
                break;
            case CurrentStatus::NoneActive:
                break;
            }
        }

        return *this;
    }

    template<class ...Args>
    void constructFirst (Args&&... args) {
        ZQ_ASSERT (mCurrentStatus == CurrentStatus::NoneActive);

        mStorage.template construct<FirstType> (Utils::forward<Args> (args)...);
    }

    template<class ...Args>
    void constructSecond (Args&&... args) {
        ZQ_ASSERT (mCurrentStatus == CurrentStatus::NoneActive);

        mStorage.template construct<SecondType> (Utils::forward<Args> (args)...);
    }

    ZQ_DEFINE_CONST_AND_NON_CONST (const FirstType&, FirstType&, getFirst, (), {
        ZQ_ASSERT (mCurrentStatus == CurrentStatus::FirstActive);
        return *mStorage.template getAs<FirstType>();
    })

    ZQ_DEFINE_CONST_AND_NON_CONST (const SecondType&, SecondType&, getSecond, (), {
        ZQ_ASSERT (mCurrentStatus == CurrentStatus::SecondActive);
        return *mStorage.template getAs<SecondType>();
    })

    bool isFirst() const
    {
        return mCurrentStatus == CurrentStatus::FirstActive;
    }

    bool isSecond() const
    {
        return mCurrentStatus == CurrentStatus::SecondActive;
    }

    bool isActive() const
    {
        return mCurrentStatus == CurrentStatus::NoneActive;
    }

    /**
     * @brief Destruct the current active value.
     */
    void destruct() {
        switch (mCurrentStatus) {
        case CurrentStatus::FirstActive:
            mStorage.template destruct<FirstType> ();
            break;
        case CurrentStatus::NoneActive:
            // Do nothing.
            break;
        case CurrentStatus::SecondActive:
            mStorage.template destruct<SecondType> ();
            break;
        }
    }

    void swap (TwoVariant &variant) {
        Utils::swap (variant.mStorage, mStorage);
        Utils::swap (variant.mCurrentStatus, mCurrentStatus);
    }

private:
    void constructFromOtherVariant (const TwoVariant &variant) {
        switch(variant.mCurrentStatus) {
        case CurrentStatus::FirstActive:
            mStorage.template construct<FirstType> (variant.getFirst());
            break;
        case CurrentStatus::SecondActive:
            mStorage.template construct<SecondType> (variant.getSecond());
        case CurrentStatus::NoneActive:
            // Do nothing
            break;
        }
    }

    StorageType mStorage;

    enum class CurrentStatus {
        NoneActive,
        FirstActive,
        SecondActive,
    };

    CurrentStatus mCurrentStatus = CurrentStatus::NoneActive;
};

/**
  @brief A dynamic variant container for a value, or an error value.


 */
template<class T, class ErrorType>
class Expected
{
public:
    /**
      @brief Forward arguments to T's constructor (not error).
     */
    template<class... Args>
    Expected(Args&&...valueArgs)
    {
        mValueOrError.constructFirst (Utils::forward<Args>(valueArgs)...);
    }

    /**
       @brief Construct the error from to an error rvalue (error).
       @param error The error.
     */
    Expected(ErrorType &&error)
    {
        mValueOrError.constructSecond (Utils::move(error));
    }

    ZQ_DISALLOW_COPY (Expected)

    operator bool () const
    {
        return !isError();
    }

    bool isError() const
    {
        return mValueOrError.isSecond();
    }

    ZQ_DEFINE_CONST_AND_NON_CONST(const ErrorType&, ErrorType&, getError, (), {
        ZQ_ASSERT (isError());
        return mValueOrError.getSecond();
    })

    ZQ_DEFINE_CONST_AND_NON_CONST (const T&, T&, get, (), {
        ZQ_ASSERT (!isError());
        return mValueOrError.getFirst();
    })


    ZQ_DEFINE_CONST_AND_NON_CONST (const T&, T&, operator*, (), {
        ZQ_ASSERT (!isError());
        return mValueOrError.getFirst();
    })

    ZQ_DEFINE_CONST_AND_NON_CONST (const T*, T*, operator ->, (), {
        ZQ_ASSERT (!isError());
        return &mValueOrError.getFirst();
    })

private:
    TwoVariant<T, ErrorType> mValueOrError;

};

} // namespace Utils
ZQ_END_NAMESPACE

#endif // ZIQE_BASE_EXPECTED_HPP
