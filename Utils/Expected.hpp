#ifndef ZIQE_BASE_EXPECTED_HPP
#define ZIQE_BASE_EXPECTED_HPP

#include "Macros.hpp"
#include "Checks.hpp"

ZQ_BEGIN_NAMESPACE
namespace Utils {

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
        : mValue{Utils::forward<Args>(valueArgs)...}, mIsError{false}
    {
    }

    ~Expected ()
    {
        destruct ();
    }

    /**
       @brief Construct the error from to an error rvalue (error).
       @param error The error.
     */
    Expected(ErrorType &&error)
        : mError{Utils::move (error)}, mIsError{true}
    {
    }

    ZQ_DISALLOW_COPY (Expected)

    Expected(Expected &&expected)
        : mIsError{expected.mIsError}
    {
        if (mIsError)
            mError = Utils::move (expected.mError);
        else
            mValue = Utils::move (expected.mValue);
    }

    Expected &operator = (Expected &&expected) {
        destruct ();

        mIsError = expected.mIsError;

        if (mIsError)
            mError = Utils::move (expected.mError);
        else
            mValue = Utils::move (expected.mValue);

        return *this;
    }

    operator bool () const
    {
        return !mIsError;
    }

    bool isError() const
    {
        return mIsError;
    }

    ZQ_DEFINE_CONST_AND_NON_CONST(const ErrorType&, ErrorType&, getError, (), {
        ZQ_ASSERT (mIsError);
        return mError;
    })

    ZQ_DEFINE_CONST_AND_NON_CONST (const T&, T&, get, (), {
        ZQ_ASSERT (!mIsError);
        return mValue;
    })


    ZQ_DEFINE_CONST_AND_NON_CONST (const T&, T&, operator*, (), {
        ZQ_ASSERT (!mIsError);
        return mValue;
    })

    ZQ_DEFINE_CONST_AND_NON_CONST (const T*, T*, operator ->, (), {
        ZQ_ASSERT (!mIsError);
        return &mValue;
    })

private:
    /**
      @brief According to the current status (error|not error),
             descturct the current active value.
      */
    void destruct() {
        if (mIsError)
            mError.~ErrorType();
        else
            mValue.~T();
    }

    /**
       @brief An union, would hold the value, or the error.
     */
    union {
         T mValue;
         ErrorType mError;
    };

    /**
       @brief mIsError Specify wether we're holding a error
              or T.
     */
    bool mIsError;
};

} // namespace Utils
ZQ_END_NAMESPACE

#endif // ZIQE_BASE_EXPECTED_HPP
