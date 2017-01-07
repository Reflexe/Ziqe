#include "Base/Optional.hpp"
#include "Base/Checks.hpp"
#include "OS/EntryPoints.h"

void ZqAPIInit() {
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


void ZqAPIExit ()
{
}
