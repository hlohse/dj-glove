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
};

#endif // DJ_GLOVE_PC_DJ_GLOVE_SIGNAL_H_

