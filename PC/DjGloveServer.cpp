#include "LibDjGlove.h"
#include "DjGloveServerArguments.h"
#include <iostream>
#include <string>
#include <memory>
using namespace std;

int main(int argc, char* argv[])
{
    DjGloveServerArguments arguments;
    Bluetooth bluetooth(1024);
    shared_ptr<BluetoothDevice> arduino;
    MidiPort midi_port;
    DjGlove glove;
    
    try {
        arguments.Apply(argc, argv);
    }
    catch (runtime_error e) {
        cout << "Arguments error: " << e.what() << endl;
        cout << arguments.Usage() << endl;
        goto fail;
    }
    
    try {
        Bluetooth::SetUp();
    }
    catch (runtime_error e) {
        cout << "Failed to set up Bluetooth:" << endl;
        cout << e.what();
        goto fail;
    }
    
    arduino = make_shared<BluetoothDevice>("Arduino",
                                           arguments.ArduinoBluetoothMac(),
                                           1);
    
    try {
        midi_port.Open(arguments.VirtualMidiPortName());
    }
    catch (runtime_error e) {
        cout << "Failed to open virtual MIDI port \""
             << arguments.VirtualMidiPortName() << "\":" << endl;
        cout << e.what() << endl;
        goto fail;
    }

    cout << "Virtual MIDI port \"" << arguments.VirtualMidiPortName()
         << "\" opened successfully!" << endl;

    try {
        bluetooth.Connect(arduino, arguments.BluetoothTimeout());
    }
    catch (runtime_error e) {
        cout << "Failed to connect via Bluetooth:" << endl;
        cout << e.what() << endl;
        goto fail;
    }

    cout << "Bluetooth connection to " << arduino->ToString()
         << " established successfully!" << endl;
   
    cout << "Server up and running!" << endl;

    while (true) {
        bluetooth.WaitUntilAvailable(1);
        
        glove.Process(*bluetooth.Read(1).c_str());

        if (glove.HasMidiSignal()) {
            MidiSignal signal = glove.NextMidiSignal();
            
            try {
                midi_port.Play(signal);
            }
            catch (runtime_error e) {
                cout << "Failed to play " << signal.ToString() << ":" << endl;
                cout << e.what() << endl;
                goto fail;
            }
        }
    }

    return 0;

fail:
    Bluetooth::TearDown();
    return -1;
}

