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

void Midi::SplitStatusChannel(const Midi::byte_t status_channel,
                              Midi::byte_t& status,
                              Midi::byte_t& channel)
{
    status  = status_channel & 0xF0;
    channel = status_channel & 0x0F;
}

Midi::byte_t CombineStatusChannel(const Midi::byte_t status, const Midi::byte_t channel)
{
    return (status << 4) | channel;
}

void Midi::SplitPitchBend(const Midi::word_t pitch_bend,
                          Midi::byte_t& fine,
                          Midi::byte_t& coarse)
{
    coarse = Limit((byte_t) (pitch_bend >> 8), Midi::Value);
    fine   = Limit((byte_t) pitch_bend, Midi::Value);
}

Midi::word_t Midi::CombinePitchBend(const Midi::byte_t fine, const Midi::byte_t coarse)
{
    return ((coarse << 7) | fine) & 0x3FFF; // 14 bit resolution
}

