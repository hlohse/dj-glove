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

#include "Export.h"
#include <memory>
#include <string>
#include <cstdint>

#ifdef __linux__
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#elif _WIN32
#include <WinSock2.h>
#include <ws2bth.h>
#include <bthsdpdef.h>
#include <bluetoothapis.h>
#endif

class EXPORT BluetoothDevice {
public:
    static std::shared_ptr<BluetoothDevice> Arduino();

#ifdef __linux__
    using SocketAddress = struct sockaddr_rc;
#elif _WIN32
    using SocketAddress = SOCKADDR_BTH;
#endif

    BluetoothDevice(const std::string& name,
                    const std::string& address,
                    const int channel);
    ~BluetoothDevice();

    bool IsValid() const;
    std::string ToString() const;

    SocketAddress GetSocketAddress() const;

private:
    static const BluetoothDevice ArduinoDevice;

    std::string m_name;
    std::string m_address;
    int m_channel;
    bool m_is_valid;
    SocketAddress m_socket_address;

#ifdef _WIN32
    int StringToAddress(IN const std::string& m_addressstring,
                        OUT BTH_ADDR& address);
#endif
};

#endif // DJ_GLOVE_PC_BLUETOOTH_DEVICE_H_

