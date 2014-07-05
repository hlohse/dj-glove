/*      MidiPlayer
 *
 *  Provides a set of channels to be configured with MIDI signals that can be
 *  send via Bluetooth to be played.
 *
 *  Example, play middle C (60) on channel 5;
 *
 *      MidiPlayer player;
 *
 *      player(5).SetStatus(Midi::NoteOn);
 *      player(5).SetKey(60);
 *      player(5).SetVelocity(64);
 *
 *      player.Play(5);
 */

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

