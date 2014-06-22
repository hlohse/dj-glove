#ifndef DJ_GLOVE_PC_COMMON_BLUETOOTH_H_
#define DJ_GLOVE_PC_COMMON_BLUETOOTH_H_

#include "CommonSerial.h"
#include "BluetoothDevice.h"
#include <sys/time.h>

class Bluetooth : public CommonSerial {
public:
    Bluetooth(const BluetoothDevice& device);
    ~Bluetooth();

    void Connect(const int timeout_s);

    // ISerial
    virtual bool IsReady() const;
    virtual int Available();
    virtual void WaitUntilAvailable();
    virtual std::string Read(const int length);
    virtual int Write(const std::string& output, const int length);

private:
    BluetoothDevice device_;
    int socket_;
    bool is_ready_;

    void ShutdownSocket();
    void ConnectSocket(struct timeval timeout, fd_set sockets);

    void SetSocketBlocking() const;
    void SetSocketNonBlocking() const;
    int GetSocketError() const;
};

#endif // DJ_GLOVE_PC_COMMON_BLUETOOTH_H_

