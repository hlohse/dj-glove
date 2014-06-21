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

#ifndef DJ_GLOVE_PC_COMMON_BLUETOOTH_DEVICE_H_
#define DJ_GLOVE_PC_COMMON_BLUETOOTH_DEVICE_H_

#include <string>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

class BluetoothDevice {
public:
    BluetoothDevice(const std::string& name,
                    const std::string& address);
    ~BluetoothDevice();
    
    struct sockaddr_rc GetSocketAddress() const;

    bool IsValid() const;
    std::string ToString() const;

private:
    std::string name_;
    std::string address_;
    struct sockaddr_rc socket_address_;
    bool is_valid_;
};

#endif // DJ_GLOVE_PC_COMMON_BLUETOOTH_DEVICE_H_

