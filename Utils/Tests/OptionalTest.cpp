#include "Utils/Optional.hpp"
#include "Utils/Checks.hpp"
#include "CppCore/EntryPoints.h"

void ZQ_PER_DRIVER_UNIQUE_SYMBOL (ZqOnLoad) (void*) {
    using Ziqe::Utils::Optional;

    Optional<int> integer;
    ZQ_ASSERT (!integer);
    ZQ_ASSERT (!integer.isValid ());

    // Construct from xvalue.
    integer.construct (1);
    ZQ_ASSERT (integer);
    ZQ_ASSERT (integer.isValid ());
    ZQ_ASSERT (*integer == 1);
}


void ZQ_PER_DRIVER_UNIQUE_SYMBOL (ZqOnUnload) (void*)
{
}
