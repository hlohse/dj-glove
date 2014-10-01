#include "DjGloveServerArguments.h"
#include "Formatter.h"
#include <sstream>
#include <stdexcept>
using namespace std;

const string DjGloveServerArguments::default_arduino_bluetooth_mac = "98:d3:31:b3:0a:25";
const string DjGloveServerArguments::default_virtual_midi_port_name = "Virtual MIDI Port";
const int    DjGloveServerArguments::default_bluetooth_timeout = 10;

DjGloveServerArguments::DjGloveServerArguments()
:   m_arduino_bluetooth_mac(default_arduino_bluetooth_mac),
    m_virtual_midi_port_name(default_virtual_midi_port_name),
    m_bluetooth_timeout(default_bluetooth_timeout)
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
    
        m_arduino_bluetooth_mac  = argv[1];
        m_virtual_midi_port_name = argv[2];
        m_bluetooth_timeout      = atoi(argv[3]);
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
    return m_arduino_bluetooth_mac;
}

string DjGloveServerArguments::VirtualMidiPortName() const
{
    return m_virtual_midi_port_name;
}

int DjGloveServerArguments::BluetoothTimeout() const
{
    return m_bluetooth_timeout;
}

