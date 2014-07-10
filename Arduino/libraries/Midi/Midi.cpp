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
                              Midi::Status& status,
                              Midi::byte_t& channel)
{
    status  = static_cast<Midi::Status>(status_channel & 0xF0); // 1111 0000
    channel = status_channel & 0x0F; // 0000 1111
}

Midi::byte_t Midi::CombineStatusChannel(const Midi::Status status, const Midi::byte_t channel)
{
    return static_cast<byte_t>(status) | (channel & 0x0F); // ssss cccc
}

void Midi::SplitPitchBend(const Midi::word_t pitch_bend,
                          Midi::byte_t& fine,
                          Midi::byte_t& coarse)
{
    const byte_t fine_7bit   = (byte_t) (pitch_bend & 0x007F); // 0000 0000 0111 1111
    const byte_t coarse_7bit = (byte_t) ((pitch_bend >> 7) & 0x007F);

    fine   = Limit(fine_7bit,   Midi::Value);
    coarse = Limit(coarse_7bit, Midi::Value);
}

Midi::word_t Midi::CombinePitchBend(const Midi::byte_t fine, const Midi::byte_t coarse)
{
    const word_t fine_word   = ((word_t) fine) & 0x007F;          // 0000 0000 0111 1111
    const word_t coarse_word = (((word_t) coarse) << 7) & 0x3F80; // 0011 1111 1000 0000
    
    return coarse_word | fine_word; // 00cc cccc cfff ffff
}

