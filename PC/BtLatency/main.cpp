#include "BtLatency.h"
#include "Platform.h"
#include "Bluetooth.h"
#include "BluetoothDevice.h"
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <float.h>
#include <cmath>
using namespace std;

typedef pair<struct timeval, struct timeval> time_tuple;
static const int timeout_s = 5;

typedef struct {
    double min;
    double max;
    double mean;
    double median;
    double stddev;
    double var;
} Result;

ISerial* GetSerial()
{
    BluetoothDevice arduino("Arduino",
                            Platform::bluetooth_mac,
                            Platform::bluetooth_channel);
    Bluetooth* bluetooth = new Bluetooth(arduino, string(BtLatency::message).length());
    
    bluetooth->Connect(timeout_s);
    
    return bluetooth;
}

void SendStartSignal(ISerial* serial)
{
    string start_signal(&BtLatency::start_signal,
                        sizeof(BtLatency::start_signal));
    serial->Write(start_signal);
}

void DetermineTimes(ISerial* serial, vector<time_tuple>& times)
{
    struct timeval start, stop;
    string expected_message(BtLatency::message);
    string message;

    times.reserve(BtLatency::num_messages);
    SendStartSignal(serial);

    for (int i = 0; i < BtLatency::num_messages; ++i) {
        gettimeofday(&start, NULL);
        serial->Write(string(expected_message));
        message = serial->ReadNextAvailable(expected_message.length());
        gettimeofday(&stop, NULL);

        if (!serial->IsReady()) {
            cout << "Serial error: " << serial->GetLastError() << endl;
            break;
        }

        if (message.compare(expected_message) != 0) {
            cout << "ERROR: Received \"" << message
                 << "\", expected \"" << expected_message << "\"" << endl;
            break;
        }

        times.push_back(time_tuple(start, stop));
    }
}

void DetermineTimesMs(const vector<time_tuple>& times, vector<double>& times_ms)
{
    vector<time_tuple>::const_iterator it;

    for (it = times.begin(); it != times.end(); ++it) {
        double start_time_us = it->first.tv_sec  * 1e6 + it->first.tv_usec;
        double stop_time_us  = it->second.tv_sec * 1e6 + it->second.tv_usec;

        double total_time_ms = (stop_time_us - start_time_us) / 1e3;
        
        times_ms.push_back(total_time_ms);
    }
}

Result GetResult(vector<double>& times_ms)
{
    vector<double>::const_iterator it;
    Result result;
    
    sort(times_ms.begin(), times_ms.end());

    result.min    = DBL_MAX;
    result.max    = DBL_MIN;
    result.mean   = 0;
    result.median = times_ms[times_ms.size() / 2];
    result.var    = 0;

    for (it = times_ms.begin(); it != times_ms.end(); ++it) {
        const double time = *it;

        if (time < result.min) {
            result.min = time;
        }
        if (time > result.max) {
            result.max = time;
        }

        result.mean += time;
    }

    result.mean /= times_ms.size();
    
    for (it = times_ms.begin(); it != times_ms.end(); ++it) {
        const double time = *it;
        result.var += (time - result.mean) * (time - result.mean);
    }

    result.var   /= times_ms.size();
    result.stddev = sqrt(result.var);

    return result;
}

void ShowResult(const Result& result, const vector<double>& times_ms)
{
    vector<double>::const_iterator it;

    cout << setprecision(2) << fixed;
    
    for (it = times_ms.begin(); it != times_ms.end(); ++it) {
        cout << it - times_ms.begin() << " " << *it << " ms" << endl;
    }

    cout << "min\tmax\tmean\tmedian\tstddev\tvar\t[ms]" << endl;
    cout << result.min    << "\t"
         << result.max    << "\t"
         << result.mean   << "\t"
         << result.median << "\t"
         << result.stddev << "\t"
         << result.var    << endl;
}

int main()
{
    ISerial* serial = GetSerial();
    vector<time_tuple> times;
    vector<double> times_ms;
   
    DetermineTimes(serial, times);

    if (times.size() == BtLatency::num_messages) {
        Result result;

        DetermineTimesMs(times, times_ms);
        result = GetResult(times_ms);
        ShowResult(result, times_ms);
    }

    delete serial;
    return 0;
}

