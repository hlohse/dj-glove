#include "Bluetooth.h"
#include "BluetoothDevice.h"
#include <iostream>
#include <cstdlib>
using namespace std;

static const int default_timeout_s = 1;

int main(const int argc, const char* argv[])
{
    BluetoothDevice arduino("Arduino", "98:D3:31:B3:0A:25", 1);
    Bluetooth bluetooth(arduino);
    const int timeout_s = argc > 1 ? atoi(argv[1]) : default_timeout_s;

    bluetooth.Connect(timeout_s);

    if (bluetooth.IsReady()) {
        cout << "Bluetooth ready!" << endl;
    }
    else {
        cout << "Bluetooth not ready for "
             << arduino.ToString() << ": "
             << bluetooth.GetLastError() << endl;
    }

    return 0;
}

