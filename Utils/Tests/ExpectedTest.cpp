#include "Utils/Expected.hpp"

#include "ZqAPI/EntryPoints.h"

enum class Error {
    FirstError,
    SecondError
};

void ZQ_SYMBOL(ZqOnLoad) (void*) {
    using namespace Ziqe;
    using Ziqe::Utils::Logger;

    Ziqe::Utils::Expected<int, Error> error{Error::FirstError};
    Ziqe::Utils::Expected<int, Error> value{123};

    ZQ_LOG("HELLOW!!");

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

void ZQ_SYMBOL(ZqOnUnload) (void*)
{
    using Ziqe::Utils::Logger;
    ZQ_LOG("GUDBAY!");
}
