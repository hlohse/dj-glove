#include "DjGloveServerArguments.h"
#include "Formatter.h"
#include <sstream>
#include <stdexcept>
using namespace std;

const string DjGloveServerArguments::default_arduino_bluetooth_mac = "98:d3:31:b3:0a:25";
const string DjGloveServerArguments::default_virtual_midi_port_name = "Virtual MIDI Port";
const int    DjGloveServerArguments::default_bluetooth_timeout = 10;

DjGloveServerArguments::DjGloveServerArguments()
:   arduino_bluetooth_mac_(default_arduino_bluetooth_mac),
    virtual_midi_port_name_(default_virtual_midi_port_name),
    bluetooth_timeout_(default_bluetooth_timeout)
{
}

DjGloveServerArguments::~DjGloveServerArguments()
{
}

void DjGloveServerArguments::Apply(int argc, char* argv[])
{
    if (argc > 1) {
        if (argc != 4) {
            throw runtime_error(Formatter()
                << argc - 1 << " arguments passed, expected 3!");
        }
    
        arduino_bluetooth_mac_  = argv[1];
        virtual_midi_port_name_ = argv[2];
        bluetooth_timeout_      = atoi(argv[3]);
    }
}

string DjGloveServerArguments::Usage() const
{
    ostringstream usage;

    usage << "  DJ Glove Server Usage" << endl
          << endl
          << "DjGloveServer arduino_mac midi_port timeout" << endl
          << "    arduino_mac Arduino Bluetooth MAC address (default: "
              << default_arduino_bluetooth_mac << ")" << endl
          << "    midi_port   Virtual MIDI Port name (default: "
              << default_virtual_midi_port_name << ")" << endl
          << "    timeout     Bluetooth connection timeout [s] (default: "
              << default_bluetooth_timeout << ")";

    return usage.str();
}

string DjGloveServerArguments::ArduinoBluetoothMac() const
{
    return arduino_bluetooth_mac_;
}

string DjGloveServerArguments::VirtualMidiPortName() const
{
    return virtual_midi_port_name_;
}

int DjGloveServerArguments::BluetoothTimeout() const
{
    return bluetooth_timeout_;
}

