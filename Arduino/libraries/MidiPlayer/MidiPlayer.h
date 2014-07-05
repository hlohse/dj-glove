#ifndef DJ_GLOVE_ARDUINO_LIBRARY_MIDI_PLAYER_H_
#define DJ_GLOVE_ARDUINO_LIBRARY_MIDI_PLAYER_H_

#include <MidiChannel.h>

class MidiPlayer {
public:
    static const int num_channels = 10;

    MidiPlayer();
    ~MidiPlayer();

    MidiChannel& operator()(const int channel);
    MidiChannel& Channel(const int channel);

    void Play(const int channel);
    void Play(MidiChannel& midi_channel);

private:
    MidiChannel channels_[num_channels];
};

#endif // DJ_GLOVE_ARDUINO_LIBRARY_MIDI_PLAYER_H_

