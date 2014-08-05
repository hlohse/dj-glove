#include "LibDjGlove.h"
#include <iostream>
#include <stdexcept>
using namespace std;

int exit()
{
    cout << "Press any key to shut down program." << endl;
    cin.ignore();
    Bluetooth::TearDown();
    return -1;
}

int main(int argc, char* argv[])
{
    if (!Bluetooth::SetUp()) {
        cout << "Failed to set up Bluetooth!" << endl;
        return exit();
    }

    Bluetooth bluetooth(BluetoothDevice::Arduino(), 1024);
    MidiPort  midi_port;

    bluetooth.Connect(10);

    if (!bluetooth.IsReady()) {
        cout << "Failed to connect Bluetooth: " << bluetooth.GetLastError() << endl;
        return exit();
    }

    try {
        midi_port.Open("Virtual MIDI Port");
    }
    catch (exception e) {
        cout << "Failed to open MIDI port: " << e.what() << endl;
        return exit();
    }

    // Play...

    return 0;
}

