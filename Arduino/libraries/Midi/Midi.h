/*      Midi
 *
 *  http://www.indiana.edu/~emusic/etext/MIDI/chapter3_MIDI4.shtml
 *
 *  Status   | Data 1   | Data 2   | Message           
 * ----------------------------------------------------
 *  1000nnnn | 0kkkkkkk | 0vvvvvvv | Note Off          
 *  1001nnnn | 0kkkkkkk | 0vvvvvvv | Note On           
 *  1010nnnn | 0kkkkkkk | 0ppppppp | Poly Key Pressure 
 *  1011nnnn | 0ccccccc | 0cvcvcvc | Controller Change 
 *  1100nnnn | 0preprep |  <none>  | Program Change    
 *  1101nnnn | 0ppppppp |  <none>  | Channel Pressure  
 *  1110nnnn | 0fffffff | 0cocococ | Pitch Bend         
 *
 *  Legend:
 *      n   Channel             0-15
 *      k   Key #               0-127 (60 = middle C)
 *      v   Velocity            0-127
 *      p   Pressure            0-127
 *      c   Controller          See MidiChannel::Controller below
 *      cv  Controller value    0-127
 *      pre Preset              0-127
 *      f   Fine                co+f = 14 bit resolution
 *      co  Coarse              co+f = 14 bit resolution
 */

#ifndef DJ_GLOVE_ARDUINO_LIBRARIES_MIDI_H_
#define DJ_GLOVE_ARDUINO_LIBRARIES_MIDI_H_

class Midi {
public:
    typedef unsigned char byte_t;

    static byte_t Limit(const byte_t value,
                        const byte_t upper_bound,
                        const byte_t lower_bound);

    enum class Limits : byte_t {
        ChannelMin  = 0,
        ChannelMax  = 15,
        Min         = 0,
        Max         = 127
    };

    enum class Status : byte_t {
        NoteOff             = 0x80, // 1000 0000
        NoteOn              = 0x90, // 1001 0000
        PolyKeyPressure     = 0xA0, // 1010 0000
        ControllerChange    = 0xB0, // 1011 0000
        ProgramChange       = 0xC0, // 1100 0000
        ChannelPressure     = 0xD0, // 1101 0000
        PitchBend           = 0xE0  // 1110 0000
    };

    // http://www.indiana.edu/~emusic/etext/MIDI/chapter3_MIDI6.shtml
    enum class Controller : byte_t {
        BankSelect          = 0x00,
        ModulationWheel     = 0x01,
        Breath              = 0x02,
        // Undefined        = 0x03,
        Foot                = 0x04,
        PortamentoTime      = 0x05,
        DataEntryMsb        = 0x06,
        MainVolume          = 0x07,
        Balance             = 0x08,
        // Undefined        = 0x09,
        Pan                 = 0x0A,
        Ch0                 = 0x0B,
        Effect1             = 0x0C,
        Effect2,
        // Undefined        = 0x0E..0x0F
        GeneralPurpose1     = 0x10,
        GeneralPurpose2,
        GeneralPurpose3,
        GeneralPurpose4,
        // Undefined        = 0x14..0x1F
        // Lsb0..31         = 0x20..0x3F    // Rarely implemented
        DamperPedalSustain  = 0x40,         // Data 1: 0..64 = Off, 64..127 = On
        Portamento          = 0x41,
        Sostenuto           = 0x42,
        SoftPedal           = 0x43,
        LegatoFootswitch    = 0x44,
        Hold2               = 0x45,
        SoundController1    = 0x46,         // Default: Sound Variation
        SoundController2,                   // Default: Timbre/Harmonic Content
        SoundController3,                   // Default: Release Time
        SoundController4,                   // Default: Attack Time
        SoundController5,                   // Default: Brightness
        SoundController6,
        SoundController7,
        SoundController8,
        SoundController9,
        SoundController10,
        GeneralPurpose5     = 0x50,
        GeneralPurpose6,
        GeneralPurpose7,
        GeneralPurpose8,
        PortamentoControl   = 0x54,
        // Undefined        = 0x55..0x5A
        EffectsDepth1       = 0x5B,         // Previously External Effects Depth
        EffectsDepth2,                      // Previously Tremolo Depth
        EffectsDepth3,                      // Previously Chorus Depth
        EffectsDepth4,                      // Previously Detune Depth
        EffectsDepth5,                      // Previously Phaser Depth
        DataIncrement       = 0x60,
        DataDecrement       = 0x61,
        // NonRegistered... = 0x62..0x65
        // Undefined        = 0x66..0x78
        
        //  Channel Mode Messages
        ResetAllControllers = 0x79,
        LocalControl        = 0x7A,
        AllNotesOff         = 0x7B,
        OmniOff             = 0x7C,
        OmniOn              = 0x7D,
        MonoOnPolyOff       = 0x7E,
        PolyOnMonoOff       = 0x7F
    };
};

#endif // DJ_GLOVE_ARDUINO_LIBRARIES_MIDI_H_

