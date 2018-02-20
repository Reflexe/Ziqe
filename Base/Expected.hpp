#ifndef ZIQE_BASE_EXPECTED_HPP
#define ZIQE_BASE_EXPECTED_HPP

#include "Base/Macros.hpp"
#include "Base/Checks.hpp"
#include "Base/ConstructableStorage.hpp"
#include "Base/Tuple.hpp"

ZQ_BEGIN_NAMESPACE
namespace Base {

template<class FirstType, class SecondType>
class TwoVariant {

    typedef Internal::ConstructableStorage<max(sizeof(FirstType), sizeof(SecondType)),
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

        mCurrentStatus = CurrentStatus::FirstActive;
        mStorage.template construct<FirstType> (Base::forward<Args> (args)...);
    }

    template<class ...Args>
    void constructSecond (Args&&... args) {
        ZQ_ASSERT (mCurrentStatus == CurrentStatus::NoneActive);

        mCurrentStatus = CurrentStatus::SecondActive;
        mStorage.template construct<SecondType> (Base::forward<Args> (args)...);
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

    void swap (TwoVariant &otherVariant) {
        if (isActive () && otherVariant.isActive ()) {
            Base::swap (otherVariant.mStorage, mStorage);
            Base::swap (otherVariant.mCurrentStatus, mCurrentStatus);
        } else if (isActive ()) {
            otherVariant.mStorage = mStorage;
            otherVariant.mCurrentStatus = mCurrentStatus;

            // Set the current variant as inactive.
            mCurrentStatus = CurrentStatus::NoneActive;
        } else if (otherVariant.isActive ()) {
            mStorage = otherVariant.mStorage;
            mCurrentStatus = otherVariant.mCurrentStatus;

            // Set the other variant as inactive.
            otherVariant.mCurrentStatus = CurrentStatus::NoneActive;
        }

        // If no variant is active, do nothing.
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

template<class...Args>
struct ErrorTypeWrapper
{
    explicit ErrorTypeWrapper(Args&&...args)
        : mArgs{Base::forward<Args>(args)...}
	{
	}

    Base::Tuple<Args...> mArgs;
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
        mValueOrError.constructFirst (Base::forward<Args>(valueArgs)...);
    }

    /**
       @brief Construct the error from to an error wrapper.
       @param error The error.
     */
    template<class...ConvertableErrorTypes>
    Expected(ErrorTypeWrapper<ConvertableErrorTypes...> &&error)
    {
//        mValueOrError.constructSecond (Base::move(error.value));
        Base::apply([this](ConvertableErrorTypes&&...args){
            mValueOrError.constructSecond(Base::forward<ConvertableErrorTypes>(args)...);
        }, Base::move (error.mArgs)
        );

    }

    ZQ_DISALLOW_COPY (Expected)
    ZQ_ALLOW_MOVE (Expected)

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


/**
 * An helper for Expected:
 *  use:
 *  return Error(val)
 *
 * To make sure we know where it is an error, we use
 * this wrapper.
 */
template<class...Args>
auto Error (Args&&...args) -> ErrorTypeWrapper<Args...>
{
    static_assert(Base::IsSame<ParameterPack<Args...>, ParameterPack<Base::RemoveReference<Args>...>>::value,
                  "Please use Base::move for arguments Base::Error");

	return ErrorTypeWrapper<Args...>{Base::forward<Args>(args)...};
}

} // namespace Base
ZQ_END_NAMESPACE

#endif // ZIQE_BASE_EXPECTED_HPP
