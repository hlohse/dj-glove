#include "LibDjGlove.h"
#include <iostream>
using namespace std;

const int buffer_size = 1024;
const int timeout_s = 5;
const string start_signal = "!";
const string midi_port_name = "Entchen";
const int num_keys = 27;

void PlayKeys(Bluetooth& bt, MidiPort& port, const int keys)
{
	for (int i = 0; i < keys; ++i) {
		const int num_bytes = (int) bt.ReadNextAvailable(1).front();

		try {
			const string midi_bytes = bt.ReadNextAvailable(num_bytes);
			MidiSignal signal(midi_bytes);

			cout << "Playing " << signal.ToString() << endl;
			
			port.Play(signal);
		}
		catch (runtime_error e) {
			cout << "Failed to play MIDI signal: " << e.what() << endl;
			return;
		}
	}
}

int main(int argc, char* argv[])
{
	if (!Bluetooth::SetUp()) {
		cout << "Failed to set up Bluetooth!" << endl;
		cin.ignore();
		return -1;
	}

	Bluetooth bt(BluetoothDevice::Arduino(), buffer_size);
	MidiPort port;

	bt.Connect(timeout_s);

	if (!bt.IsReady()) {
		cout << "Failed to connect via Bluetooth: " << bt.GetLastError() << endl;
		cin.ignore();
		return -1;
	}

	try {
		port.Open(midi_port_name);
		cout << "Opened virtual MIDI port " << midi_port_name << endl;
	}
	catch (runtime_error e) {
		cout << "Failed to open MIDI port: " << e.what() << endl;
		cin.ignore();
		return -1;
	}

	cout << "Press any key to start receiving MIDI signals." << endl;
	cin.ignore();

	bt.Write(start_signal);
	PlayKeys(bt, port, num_keys);

	cout << "Done." << endl;
	cin.ignore();
	
	Bluetooth::TearDown();
	return 0;
}