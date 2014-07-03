#include "Midi.h"

Midi::byte_t Limit(const Midi::byte_t value,
                   const Midi::byte_t upper_bound,
                   const Midi::byte_t lower_bound)
{
    if (value > upper_bound) {
        return upper_bound;
    }
    else if (value < lower_bound) {
        return lower_bound;
    }
    else {
        return value;
    }
}

