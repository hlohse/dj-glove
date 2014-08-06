#ifndef DJ_GLOVE_PC_DJ_GLOVE_SIGNAL_H_
#define DJ_GLOVE_PC_DJ_GLOVE_SIGNAL_H_

#include "Export.h"
#include "MidiSignal.h"
#include <vector>

class EXPORT DjGloveSignal {
public:
    DjGloveSignal();
    virtual ~DjGloveSignal();

    static bool SignalsPunch(const char value);
    static bool SignalsSensors(const char value);

    virtual int NumValues() const = 0;
    virtual void FromNext(std::vector<char>& values) = 0;

    std::vector<MidiSignal>& MidiSignals();

protected:
    std::vector<MidiSignal> midi_signals_;
};

class EXPORT PunchSignal : public DjGloveSignal {
public:
    PunchSignal();
    virtual ~PunchSignal();
    
    virtual int NumValues() const;
    virtual void FromNext(std::vector<char>& values);

private:
    bool punched_;
};

class EXPORT SensorsSignal : public DjGloveSignal {
public:
    SensorsSignal();
    virtual ~SensorsSignal();
    
    virtual int NumValues() const;
    virtual void FromNext(std::vector<char>& values);

private:
    // TODO: Adjust accordingly
    static const int num_midi_signals = 10;

    bool button_push_0_;    // 1 Bit: pressed/not pressed
    bool button_push_1_;
    bool button_push_2_;
    bool button_push_3_;
    bool button_push_4_;
    bool button_touch_0_;
    bool button_touch_1_;
    bool button_touch_2_;
    bool button_touch_3_;
    bool button_flip_;
    int  poti_0_;           // 7 Bit: 0..127
    int  poti_1_;
    int  poti_2_;
    int  distance_;
    int  orientation_x_;
    int  orientation_y_;
    int  orientation_z_;
    int  flex_;             // 6 Bit: 0..63
    int  channel_;          // 3 Bit: 0..7 (LED)
    int  program_;
    // 71 Bit = 9 Byte total
};

#endif // DJ_GLOVE_PC_DJ_GLOVE_SIGNAL_H_

