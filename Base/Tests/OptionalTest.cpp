#include "Base/Optional.hpp"
#include "ZiqeAPI/EntryPoints.h"

void ZqAPIInit() {
    using Ziqe::Base::Optional;

    Optional<int> integer;
    DEBUG_CHECK (!integer);
    DEBUG_CHECK (!integer.isValid ());

    // Construct from xvalue.
    integer.construct (1);
    DEBUG_CHECK (integer);
    DEBUG_CHECK (integer.isValid ());
    DEBUG_CHECK (*integer == 1);
}


void ZqAPIExit ()
{
}
