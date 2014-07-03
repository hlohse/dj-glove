#include "BtLatency.h"
#include "Bluetooth.h"
#include "BluetoothDevice.h"
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cfloat>
#include <cmath>
using namespace std;

#ifdef __linux__
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
using Time = struct timeval;
#elif _WIN32
#include <windows.h>
using Time = SYSTEMTIME;
#endif

using TimeTuple = pair<Time, Time>;
static const int timeout_s = 10;

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
    Bluetooth* bluetooth = new Bluetooth(
        BluetoothDevice::Arduino,
        string(BtLatency::message).length());
    bluetooth->Connect(timeout_s);
    
    return bluetooth;
}

void SyncStart(ISerial* serial)
{
    string start_signal(&BtLatency::start_signal,
                        sizeof(BtLatency::start_signal));
    
    serial->Write(start_signal);
}

void DetermineTimes(ISerial* serial, vector<TimeTuple>& times)
{
    Time start, stop;
    const string expected_message(BtLatency::message);
    string message;

    times.reserve(BtLatency::num_messages);
    SyncStart(serial);

    for (unsigned int i = 0; i < BtLatency::num_messages; ++i)
	{
#ifdef __linux__
        gettimeofday(&start, NULL);
#elif _WIN32
		GetSystemTime(&start);
#endif

		message = serial->ReadNextAvailable(expected_message.length());

#ifdef __linux__
		gettimeofday(&stop, NULL);
#elif _WIN32
		GetSystemTime(&stop);
#endif

        if (!serial->IsReady()) {
            cout << "Serial error: " << serial->GetLastError() << endl;
            break;
        }

        if (message.compare(expected_message) != 0) {
            cout << "ERROR: Received \"" << message
                 << "\", expected \"" << expected_message << "\"" << endl;
            break;
        }

		times.push_back(TimeTuple(start, stop));
    }
}

void DetermineTimesMs(const vector<TimeTuple>& times, vector<double>& times_ms)
{
    for (auto it = times.cbegin(); it != times.cend(); ++it)
	{
		double total_time_ms;

#ifdef __linux__
        double start_time_us = it->first.tv_sec  * 1e6 + it->first.tv_usec;
        double stop_time_us  = it->second.tv_sec * 1e6 + it->second.tv_usec;
        total_time_ms = (stop_time_us - start_time_us) / 1e3;
#elif _WIN32
		double start_time_ms = it->first.wSecond  * 1e3 + it->first.wMilliseconds;
		double stop_time_ms  = it->second.wSecond * 1e3 + it->second.wMilliseconds;
		total_time_ms = stop_time_ms - start_time_ms;
#endif
       
        times_ms.push_back(total_time_ms);
    }
}

Result GetResult(vector<double>& times_ms)
{
    vector<double> times_ms_sorted(times_ms);
    Result result;
    
    sort(times_ms_sorted.begin(), times_ms_sorted.end());

    result.min    = DBL_MAX;
    result.max    = DBL_MIN;
    result.mean   = 0;
    result.median = times_ms[times_ms_sorted.size() / 2];
    result.var    = 0;

    for (auto it = times_ms.cbegin(); it != times_ms.cend(); ++it) {
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
    
    for (auto it = times_ms.cbegin(); it != times_ms.cend(); ++it) {
        const double time = *it;
        result.var += (time - result.mean) * (time - result.mean);
    }

    result.var   /= times_ms.size();
    result.stddev = sqrt(result.var);

    return result;
}

void ShowResult(const Result& result, const vector<double>& times_ms)
{
    cout << setprecision(2) << fixed;
    
    for (auto it = times_ms.cbegin(); it != times_ms.cend(); ++it) {
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
	if (!Bluetooth::SetUp()) {
		cout << "Failed to set up Bluetooth!" << endl;
		return -1;
	}

    ISerial* serial = GetSerial();
    vector<TimeTuple> times;
    vector<double> times_ms;

    if (!serial->IsReady()) {
		cout << "Serial error: " << serial->GetLastError() << endl;
		Bluetooth::TearDown();
        return -1;
    }

    DetermineTimes(serial, times);

    if (times.size() == BtLatency::num_messages) {
        Result result;

        DetermineTimesMs(times, times_ms);
        result = GetResult(times_ms);
        ShowResult(result, times_ms);
    }

	Bluetooth::TearDown();
    delete serial;
    return 0;
}

