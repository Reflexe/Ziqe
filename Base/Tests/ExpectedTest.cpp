#include "Base/Expected.hpp"
#include "PerDriver/EntryPoints.hpp"

enum class Error {
    FirstError,
    SecondError
};

ZQ_BEGIN_C_DECL

void ZQ_PER_DRIVER_UNIQUE_SYMBOL(ZqOnLoad) (Ziqe::Base::RawPointer<Ziqe::OS::DriverContext>) {
    using namespace Ziqe;

	Ziqe::Base::Expected<int, Error> error{Base::Error(Error::FirstError)};
    Ziqe::Base::Expected<int, Error> value{123};

    ZQ_ASSERT (value.get () == 123);
    ZQ_ASSERT (error.isError ());
    ZQ_ASSERT (error.getError () == Error::FirstError);
    ZQ_ASSERT (value);
    ZQ_ASSERT (! error);

    Ziqe::Base::Expected<int, Error> newError{Base::move(error)};
    ZQ_ASSERT (newError.isError ());
    ZQ_ASSERT (newError.getError() == Error::FirstError);

    Ziqe::Base::Expected<int, Error> newValue{Base::move (value)};
    ZQ_ASSERT (!! newValue);
    ZQ_ASSERT (value.get () == 123);
}

void ZQ_PER_DRIVER_UNIQUE_SYMBOL(ZqOnUnload) (Ziqe::Base::RawPointer<Ziqe::OS::DriverContext>)
{
}

ZQ_END_C_DECL
