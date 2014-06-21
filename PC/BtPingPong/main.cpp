#include "Bluetooth.h"
#include "BluetoothDevice.h"
#include <iostream>
using namespace std;

int main(const int argc, const char* argv[])
{
    BluetoothDevice arduino("Arduino", "00:00:00:00:00:00");

    if (!arduino.IsValid()) {
        cout << "Invalid " << arduino.ToString() << endl;
        return -1;
    }
    
    Bluetooth bluetooth = Bluetooth(arduino);

    if (bluetooth.HasAvailable()) {
        cout << "Available!" << endl;
    }
    else {
        cout << "Not available!" << endl;
    }

    return 0;
}

