#include "Utils/Expected.hpp"
#include "PerDriver/EntryPoints.h"

enum class Error {
    FirstError,
    SecondError
};

void ZQ_PER_DRIVER_UNIQUE_SYMBOL(ZqOnLoad) (void*) {
    using namespace Ziqe;

    Ziqe::Utils::Expected<int, Error> error{Error::FirstError};
    Ziqe::Utils::Expected<int, Error> value{123};

    ZQ_ASSERT (value.get () == 123);
    ZQ_ASSERT (error.isError ());
    ZQ_ASSERT (error.getError () == Error::FirstError);
    ZQ_ASSERT (value);
    ZQ_ASSERT (! error);

    auto newError{Utils::move(error)};
    ZQ_ASSERT (newError.isError ());
    ZQ_ASSERT (newError.getError() == Error::FirstError);

    auto newValue{Utils::move (value)};
    ZQ_ASSERT (!! newValue);
    ZQ_ASSERT (value.get () == 123);
}

void ZQ_PER_DRIVER_UNIQUE_SYMBOL(ZqOnUnload) (void*)
{
}
