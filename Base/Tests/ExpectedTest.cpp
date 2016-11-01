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

    DEBUG_CHECK (value.get () == 123);
    DEBUG_CHECK (error.isError ());
    DEBUG_CHECK (error.getError () == Error::FirstError);
    DEBUG_CHECK (value);
    DEBUG_CHECK (! error);

    auto newError = error;
    DEBUG_CHECK (newError.isError ());
    DEBUG_CHECK (newError.getError () == error.getError ());

    auto newValue = value;
    DEBUG_CHECK (newValue);
    DEBUG_CHECK (newError.get () == error.get ());
}

void ZqAPIExit ()
{
}
