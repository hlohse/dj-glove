#include "Bluetooth.h"
#include "BluetoothDevice.h"
#include "Platform.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
using namespace std;

#define MSG_SIZE 4
#define MSG_SEND "PONG"
#define MSG_RECV "PING"

static const int default_timeout_s = 1;

void Error(const ISerial* serial, const BluetoothDevice& device)
{
    cout << "Bluetooth not ready for "
         << device.ToString() << ": "
         << serial->GetLastError() << endl;
}

int main(const int argc, const char* argv[])
{
    const int timeout_s = argc > 1 ? atoi(argv[1]) : default_timeout_s;
    BluetoothDevice arduino("Arduino",
                            Platform::bluetooth_mac,
                            Platform::bluetooth_channel);
    Bluetooth bluetooth(arduino);
    bluetooth.Connect(timeout_s);
    ISerial* serial = &bluetooth;
    string ping;

    while (serial->IsReady()) {
        ping = serial->ReadNextAvailable(4);
        cout << ping << endl;
    }
    
    Error(serial, arduino);

    return 0;
}

