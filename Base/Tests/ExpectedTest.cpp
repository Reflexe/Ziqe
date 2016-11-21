#include "Base/Expected.hpp"

#include "ZiqeAPI/EntryPoints.h"

enum class Error {
    FirstError,
    SecondError
};

void ZqAPIInit () {
    using namespace Ziqe;

    Ziqe::Base::Expected<int, Error> error{Error::FirstError};
    Ziqe::Base::Expected<int, Error> value{123};

    ZQ_ASSERT (value.get () == 123);
    ZQ_ASSERT (error.isError ());
    ZQ_ASSERT (error.getError () == Error::FirstError);
    ZQ_ASSERT (value);
    ZQ_ASSERT (! error);

    auto newError = error;
    ZQ_ASSERT (newError.isError ());
    ZQ_ASSERT (newError.getError () == error.getError ());

    auto newValue = value;
    ZQ_ASSERT (newValue);
    ZQ_ASSERT (newError.get () == error.get ());
}

void ZqAPIExit ()
{
}
