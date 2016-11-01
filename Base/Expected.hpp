#ifndef ZIQE_BASE_EXPECTED_HPP
#define ZIQE_BASE_EXPECTED_HPP

#include "Macros.hpp"
#include "Checks.hpp"

namespace Ziqe {
namespace Base {

template<class T, class ErrorType>
class Expected
{
public:
    template<class... Args>
    Expected(Args&&...valueArgs)
        : mValue{Base::forward<Args>(valueArgs)...}, mIsError{false}
    {
    }

    ~Expected ()
    {
        destruct ();
    }

    Expected(ErrorType &&error)
        : mError{std::move (error)}, mIsError{true}
    {
    }

    Expected(Expected &&expected)
        : mIsError{expected.mIsError}
    {
        if (mIsError)
            mError = Base::move (expected.mError);
        else
            mValue = Base::move (expected.mValue);
    }

    Expected(const Expected &expected)
        : mIsError{expected.mIsError}
    {
        if (mIsError)
            mError = expected.mError;
        else
            mValue = expected.mValue;
    }

    Expected &operator = (Expected &&expected) {
        destruct ();

        if (mIsError)
            mError = Base::move (expected.mError);
        else
            mValue = Base::move (expected.mValue);

        return *this;
    }

    Expected &operator = (const Expected &expected) {
        destruct ();

        if (mIsError)
            mError = expected.mError;
        else
            mValue = expected.mValue;

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
        DEBUG_CHECK (mIsError);
        return mError;
    })

    ZQ_DEFINE_CONST_AND_NON_CONST (const T&, T&, get, (), {
        DEBUG_CHECK (!mIsError);
        return mValue;
    })


    ZQ_DEFINE_CONST_AND_NON_CONST (const T&, T&, operator*, (), {
        DEBUG_CHECK (!mIsError);
        return mValue;
    })

    ZQ_DEFINE_CONST_AND_NON_CONST (const T*, T*, operator ->, (), {
        DEBUG_CHECK (!mIsError);
        return &mValue;
    })

private:
    void destruct() {
        if (mIsError)
            mError.~ErrorType();
        else
            mValue.~T();
    }

    union {
         T mValue;
         ErrorType mError;
    };

    bool mIsError;
};

} // namespace Base
} // namespace Ziqe

#endif // ZIQE_BASE_EXPECTED_HPP
