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

void DetermineTimes(Bluetooth& bluetooth, vector<TimeTuple>& times)
{
    Time start, stop;
    times.reserve(iterations);
    
    for (int i = 0; i < iterations; ++i)
    {
#ifdef __linux__
        gettimeofday(&start, NULL);
#elif _WIN32
        GetSystemTime(&start);
#endif

        bluetooth.Write(message);
        bluetooth.WaitUntilAvailable(i + 1);

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
    for (auto it = times.cbegin() + 200; it != times.cend(); ++it)
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
    Bluetooth bluetooth(1024);
    vector<TimeTuple> times;
    vector<double> times_ms;
    Result result;

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

    DetermineTimes(bluetooth, times);
    DetermineTimesMs(times, times_ms);
    
    result = GetResult(times_ms);
    ShowResult(result, times_ms);

    Bluetooth::TearDown();
    return 0;
}

