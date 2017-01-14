#include "Base/Optional.hpp"
#include "Base/Checks.hpp"
#include "ZqAPI/EntryPoints.h"

void ZqOnLoad () {
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


void ZqOnUnload ()
{
}
