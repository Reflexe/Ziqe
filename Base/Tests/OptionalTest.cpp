#include "Base/Optional.hpp"
#include "Base/Checks.hpp"
#include "PerDriver/EntryPoints.hpp"

ZQ_BEGIN_C_DECL
void ZQ_PER_DRIVER_UNIQUE_SYMBOL (ZqOnLoad) (Ziqe::Base::RawPointer<Ziqe::OS::DriverContext>) {
    using Ziqe::Base::Optional;

    Optional<int> integer;
    ZQ_ASSERT (!integer);
    ZQ_ASSERT (!integer.isValid ());

    // Construct from xvalue.
    integer.construct (1);
    ZQ_ASSERT (integer);
    ZQ_ASSERT (integer.isValid ());
    ZQ_ASSERT (*integer == 1);
}


void ZQ_PER_DRIVER_UNIQUE_SYMBOL (ZqOnUnload) (Ziqe::Base::RawPointer<Ziqe::OS::DriverContext>)
{
}
ZQ_END_C_DECL
