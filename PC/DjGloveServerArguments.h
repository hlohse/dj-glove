#ifndef DJ_GLOVE_PC_DJ_GLOVE_SERVER_ARGUMENTS_H_
#define DJ_GLOVE_PC_DJ_GLOVE_SERVER_ARGUMENTS_H_

#include "Export.h"
#include <string>

class EXPORT DjGloveServerArguments {
public:
    DjGloveServerArguments();
    ~DjGloveServerArguments();
    
    void Apply(int argc, char* argv[]);

    std::string ArduinoBluetoothMac() const;
    std::string VirtualMidiPortName() const;
    int         BluetoothTimeout()    const;

    std::string Usage() const;

private:
    static const std::string default_arduino_bluetooth_mac;
    static const std::string default_virtual_midi_port_name;
    static const int         default_bluetooth_timeout;

    std::string m_arduino_bluetooth_mac;
    std::string m_virtual_midi_port_name;
    int         m_bluetooth_timeout;
};

#endif // DJ_GLOVE_PC_DJ_GLOVE_SERVER_ARGUMENTS_H_

