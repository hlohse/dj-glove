#include "BtLatency.h"
#include "Platform.h"
#include "Bluetooth.h"
#include "BluetoothDevice.h"
#include <string>
#include <iostream>
#include <unistd.h>
using namespace std;

static const int timeout_s = 5;

ISerial* GetSerial()
{
    BluetoothDevice arduino("Arduino",
                            Platform::bluetooth_mac,
                            Platform::bluetooth_channel);
    Bluetooth* bluetooth = new Bluetooth(arduino);
    
    bluetooth->Connect(timeout_s);
    
    return bluetooth;
}

int main()
{
    ISerial* serial = GetSerial();
    struct timeval start, stop;
    double start_time_us, stop_time_us, total_time_ms;
    string expected_message(BtLatency::message);
    string message;

    gettimeofday(&start, NULL);

    for (int i = 0; i < BtLatency::num_messages; ++i) {
        message = serial->ReadNextAvailable(expected_message.length());

        if (!serial->IsReady()) {
            cout << "Serial error: " << serial->GetLastError() << endl;
            break;
        }

        if (message.compare(expected_message) != 0) {
            cout << "ERROR: Received \"" << message
                 << "\", expected \"" << expected_message << "\"" << endl;
        }

        if (i % (BtLatency::num_messages / 10) == 0) {
            cout << "Received " << i << " of "
                 << BtLatency::num_messages << " messages" << endl;
        }
    }

    gettimeofday(&stop, NULL);

    start_time_us = start.tv_sec * 1e6 + start.tv_usec;
    stop_time_us  = stop.tv_sec  * 1e6 + stop.tv_usec;
    total_time_ms = (stop_time_us - start_time_us) / 1e3;

    cout << "Total time [ms]:       " << total_time_ms << endl;
    cout << "Time per message [ms]: " << total_time_ms / BtLatency::num_messages << endl;

    delete serial;
    return 0;
}

