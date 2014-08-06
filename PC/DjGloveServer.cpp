#include "LibDjGlove.h"
#include <iostream>
#include <string>
#include <memory>
#include <boost/program_options.hpp>
using namespace std;
namespace po = boost::program_options;

void Start(const int timeout_s,
           const string& arduino_mac,
           const string& midi_port_name)
{
    Bluetooth bt(1024);
    shared_ptr<BluetoothDevice> device;
    MidiPort midi_port;
    DjGlove glove;

    if (arduino_mac.empty()) {
        device = BluetoothDevice::Arduino();
    }
    else {
        device = make_shared<BluetoothDevice>("Arduino", arduino_mac, 1);
    }

    try {
        midi_port.Open(midi_port_name);
    }
    catch (runtime_error e) {
        cout << "Failed to open virtual MIDI port " << midi_port_name << endl;
        cout << e.what() << endl;
        return;
    }

    cout << "Virtual MIDI port " << midi_port_name << " running" << endl;

    try {
        Bluetooth::SetUp();
    }
    catch (runtime_error e) {
        cout << "Failed to set up Bluetooth" << endl;
        cout << e.what();
        Bluetooth::TearDown();
        return;
    }

    try {
        bt.Connect(device, timeout_s);
    }
    catch (runtime_error e) {
        cout << "Failed to connect via Bluetooth" << endl;
        cout << e.what() << endl;
        Bluetooth::TearDown();
        return;
    }

    while (true) {
        bt.WaitUntilAvailable(1);
        
        try {
            glove.Process(*bt.Read(1).c_str());
        }
        catch (runtime_error e) {
            cout << "Failed to receive Bluetooth data" << endl;
            cout << e.what() << endl;
            break;
        }

        if (glove.HasMidiSignal()) {
            MidiSignal signal = glove.NextMidiSignal();
            
            try {
                midi_port.Play(signal);
            }
            catch (runtime_error e) {
                cout << "Failed to play " << signal.ToString() << endl;
                cout << e.what() << endl;
                break;
            }
        }
    }
}

int main(int argc, char* argv[])
{
    string arduino_mac;
    string midi_port_name;
    int timeout_s = 0;

    po::options_description options_description("DJ Glove Server Options");
    po::variables_map options;

    options_description.add_options()
        ("help",           "Show options")
        ("arduino-mac",    po::value<std::string>(&arduino_mac),                "Arduino's Bluetooth MAC address")
        ("midi-port-name", po::value<std::string>(&midi_port_name)->required(), "Virtual MIDI port's name")
        ("timeout",        po::value<int>(&timeout_s)->required(),              "Bluetooth connection timeout [s]")
    ;

    po::store(po::parse_command_line(argc, argv, options_description), options);
    po::notify(options);

    if (options.count("help")) {
        cout << options_description << endl;
        return 1;
    }

    Start(timeout_s, arduino_mac, midi_port_name);
    
    return 0;
}

