#include "BtLatency.h"
#include "LibDjGlove.h"
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cfloat>
#include <cmath>
using namespace std;
using namespace BtLatency;

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

void SyncStart(Bluetooth& bluetooth)
{
	bluetooth.Write((char) start_signal);
}

void DetermineTimes(Bluetooth& bluetooth,
                    vector<TimeTuple>& times,
                    const int message_size)
{
    Time start, stop;

    bluetooth.Clear();
    times.clear();
    times.reserve(tries_per_message);
    
    SyncStart(bluetooth);

    for (int i = 0; i < tries_per_message; ++i)
	{
#ifdef __linux__
        gettimeofday(&start, NULL);
#elif _WIN32
		GetSystemTime(&start);
#endif

        bluetooth.WaitUntilAvailable((i + 1) * message_size);

#ifdef __linux__
		gettimeofday(&stop, NULL);
#elif _WIN32
		GetSystemTime(&stop);
#endif

		times.push_back(TimeTuple(start, stop));
    }
}

void DetermineTimesMs(const vector<TimeTuple>& times, vector<double>& times_ms)
{
    times_ms.clear(); 

    for (auto it = times.cbegin() + 1; it != times.cend(); ++it)
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

void ShowResult(const Result& result,
                const vector<double>& times_ms,
                const int delay_ms,
                const int message_size)
{
    cout << setprecision(2) << fixed;
    
    cout << "delay " << delay_ms << ", message size " << message_size << endl;

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

int exit(const string& message, const runtime_error& error)
{
    cout << message << endl;
    cout << error.what() << endl;

    try {
        Bluetooth::TearDown();
    }
    catch (runtime_error e) {
        cout << "Failed to tear down Bluetooth" << endl;
        cout << e.what() << endl;
    }

    cout << "Press any key to terminate." << endl;
    cin.ignore();
    return -1;
}

int main()
{
    const int buffer_size = message_sizes[num_message_sizes - 1];
    Bluetooth bluetooth(buffer_size);
    vector<TimeTuple> times;
    vector<double> times_ms;

    try {
        Bluetooth::SetUp();
    }
    catch (runtime_error e) {
        return exit("Failed to set up Bluetooth", e);
    }

    try {
        bluetooth.Connect(BluetoothDevice::Arduino(), timeout_s);
    }
    catch (runtime_error e) {
        return exit("Failed to connect Bluetooth", e);
    }

    for (int d = 0; d < num_delays_ms; ++d) {
        for (int m = 0; m < num_message_sizes; ++m) {
            const int delay_ms     = delays_ms[d];
            const int message_size = message_sizes[m];
            Result result;

            DetermineTimes(bluetooth, times, message_size);
            DetermineTimesMs(times, times_ms);
            
            result = GetResult(times_ms);
            ShowResult(result, times_ms, delay_ms, message_size);
        }
    }

    cout << "Press any key to terminate." << endl;
	cin.ignore();

	Bluetooth::TearDown();
    return 0;
}

