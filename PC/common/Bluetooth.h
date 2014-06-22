#ifndef DJ_GLOVE_PC_COMMON_BLUETOOTH_H_
#define DJ_GLOVE_PC_COMMON_BLUETOOTH_H_

#include "CommonSerial.h"
#include "BluetoothDevice.h"

class Bluetooth : public CommonSerial {
public:
    Bluetooth(const BluetoothDevice& device);
    ~Bluetooth();

    void Connect(const int timeout_s);

    // ISerial
    virtual bool IsReady() const;
    virtual int Available() const;
    virtual void WaitUntilAvailable();
    virtual std::string Read(const int length);
    virtual int Write(const std::string& output, const int length);

private:
    BluetoothDevice device_;
    int socket_;
    bool is_ready_;

    void SetSocketBlocking();
    void SetSocketNonBlocking();
};

#endif // DJ_GLOVE_PC_COMMON_BLUETOOTH_H_

