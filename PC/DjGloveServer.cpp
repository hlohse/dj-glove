#include "LibDjGlove.h"
#include <iostream>
#include <string>
#include <memory>
using namespace std;

const int timeout_s         = 10;
const string midi_port_name = "DJ Glove";

int main(int argc, char* argv[])
{
	Bluetooth bt(1024);
	shared_ptr<BluetoothDevice> device = BluetoothDevice::Arduino();
	MidiPort midi_port;
	DjGlove glove;

	try {
		midi_port.Open(midi_port_name);
	}
	catch (runtime_error e) {
		cout << "Failed to open virtual MIDI port " << midi_port_name << endl;
		cout << e.what() << endl;
		cin.ignore();
		return -1;
	}

	cout << "Virtual MIDI port " << midi_port_name << " running" << endl;

	try {
		Bluetooth::SetUp();
	}
	catch (runtime_error e) {
		cout << "Failed to set up Bluetooth" << endl;
		cout << e.what();
		cin.ignore();
		Bluetooth::TearDown();
		return -1;
	}

	try {
		bt.Connect(device, timeout_s);
	}
	catch (runtime_error e) {
		cout << "Failed to connect via Bluetooth" << endl;
		cout << e.what() << endl;
		cin.ignore();
		Bluetooth::TearDown();
		return -1;
	}

	while (true) {
		bt.WaitUntilAvailable(1);

		try {
			glove.Process(*bt.Read(1).c_str());
		}
		catch (runtime_error e) {
			cout << "Failed to receive Bluetooth data" << endl;
			cout << e.what() << endl;
			cin.ignore();
			Bluetooth::TearDown();
			return -1;
		}

		if (glove.HasMidiSignal()) {
			MidiSignal signal = glove.NextMidiSignal();

			try {
				midi_port.Play(signal);
			}
			catch (runtime_error e) {
				cout << "Failed to play " << signal.ToString() << endl;
				cout << e.what() << endl;
				cin.ignore();
				Bluetooth::TearDown();
				return -1;
			}
		}
	}
    
    return 0;
}

