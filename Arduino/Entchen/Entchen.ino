#include <Midi.h>
#include <MidiChannel.h>
#include <MidiPlayer.h>
#include <Bluetooth.h>
#include <SoftwareSerial.h>

const char start_signal = '!';
const int  channel_nr   = 0;
const int  velocity     = 64;
const int  key_delay    = 100;
const int  num_keys     = 27;

// http://computermusicresource.com/midikeys.html
const int keys[num_keys] = {
    60, 62, 64, 65, 67, 67,  // C D E F G G
    69, 69, 69, 69, 67,      // A A A A G
    69, 69, 69, 69, 67,      // A A A A G
    65, 65, 65, 65, 64, 64,  // F F F F E E
    62, 62, 62, 62, 60       // D D D D C
};

void setup() {
    BluetoothSetup();
}

void loop() {
    MidiPlayer player;
    MidiChannel& channel = player(channel_nr);
    channel.Status(Midi::NoteOn);
    channel.Velocity(velocity);
    
    while (!Bluetooth.available());
    
    if (Bluetooth.read() == start_signal) {
        for (int i = 0; i < num_keys; i++) {
            channel.Key(keys[i]);
            player.Play(channel);
            delay(key_delay);
        }
    }
}
