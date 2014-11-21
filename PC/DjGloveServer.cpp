#include "LibDjGlove.h"
#include "DjGloveServerArguments.h"
#include <iostream>
#include <string>
#include <memory>
#include <iomanip>
using namespace std;

const char* sync_signal = "!";

int main(int argc, char* argv[])
{
    DjGloveServerArguments arguments;
    Bluetooth bluetooth(1024);
    shared_ptr<BluetoothDevice> arduino;
    MidiPort midi_port;
    DjGlove glove;
    int i = 0;

    Orientation::InitFrequency();
    
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

    //cout << glove.DataHeader() << endl;

    bluetooth.Write(sync_signal);
    while (bluetooth.ReadNextAvailable() != sync_signal[0]);

    while (true) {
        const char data = bluetooth.ReadNextAvailable();
        glove.Process(data);

        i++;
        if (i == 100) {
            //cout << glove.DataString() << endl;
            //cout << (int) data << endl;
            i = 0;
        }

        while (glove.HasMidiSignal()) {
            MidiSignal signal = glove.NextMidiSignal();
            //cout << signal.ToString() << endl;
            
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
    cin.ignore();
    Bluetooth::TearDown();
    return -1;
}

