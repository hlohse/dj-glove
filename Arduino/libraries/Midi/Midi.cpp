#include "Midi.h"

Midi::byte_t Midi::Limit(const Midi::byte_t value,
                         const Midi::ValueType type)
{
    const byte_t min = Midi::Min;
    const byte_t max = type == Midi::Channel ? Midi::ChannelMax : Midi::Max;

    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

void Midi::SplitPitchBend(const Midi::word_t pitch_bend,
                          Midi::byte_t& coarse,
                          Midi::byte_t& fine)
{
    coarse = Limit((byte_t) (pitch_bend >> 8), Midi::Value);
    fine   = Limit((byte_t) pitch_bend, Midi::Value);
}

