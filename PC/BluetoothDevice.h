/*
 *  This class represents a convenient way to deal with Bluetooth devices, low-
 *  level data structures and validity checks.
 *
 *  Example, retrieve socket address:
 *
 *      BluetoothDevice bt_device("Example", "01:23:45:67:89:10");
 *
 *      if (bt_device.IsValid()) {
 *          // Do stuff with
 *          bt_device.GetSocketAddress();
 *      }
 *      else {
 *          cout << "Invalid " << bt_device.ToString() << endl;
 *      }
 */

#ifndef DJ_GLOVE_PC_BLUETOOTH_DEVICE_H_
#define DJ_GLOVE_PC_BLUETOOTH_DEVICE_H_

#include <string>
#include <cstdint>

#ifdef __linux__
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#elif _WIN32
//#include <Windows.h>
#include <WinSock2.h>
#include <ws2bth.h>
#include <bthsdpdef.h>
#include <bluetoothapis.h>
#endif

class BluetoothDevice {
public:
    static const BluetoothDevice Arduino;

    BluetoothDevice(const std::string& name,
                    const std::string& address,
                    const int channel);
    ~BluetoothDevice();

    bool IsValid() const;
	std::string ToString() const;

#ifdef __linux__
	struct sockaddr_rc
#elif _WIN32
	SOCKADDR_BTH
#endif
	GetSocketAddress() const;

private:
    std::string name_;
    std::string address_;
    int channel_;
	bool is_valid_;

#ifdef __linux__
	struct sockaddr_rc
#elif _WIN32
	SOCKADDR_BTH
#endif
	socket_address_;

#ifdef _WIN32
	int StringToAddress(IN const std::string& address_string,
						OUT BTH_ADDR& address);
#endif
};

#endif // DJ_GLOVE_PC_BLUETOOTH_DEVICE_H_

