#ifndef DJ_GLOVE_PC_COMMON_BLUETOOTH_H_
#define DJ_GLOVE_PC_COMMON_BLUETOOTH_H_

#include "CommonSerial.h"
#include "BluetoothDevice.h"
#include <sys/time.h>

class Bluetooth : public CommonSerial {
public:
    Bluetooth(const BluetoothDevice& device);
    virtual ~Bluetooth();

    void Connect(const int timeout_s);

    // ISerial
    virtual bool IsReady() const;
    virtual void WaitUntilAvailable(const int length);
    virtual int Available();
    virtual std::string Read(const int length);
    virtual int Write(const std::string& output);

private:
    static const int read_socket_buffer_bytes = 64;

    BluetoothDevice device_;
    int socket_;
    bool is_ready_;
    std::string buffer_;
    char* read_socket_buffer_;

    void ShutdownSocket();
    void ConnectSocket(struct timeval timeout, fd_set sockets);
    void ReadSocket();

    void SetSocketBlocking() const;
    void SetSocketNonBlocking() const;
    int GetSocketError() const;
};

#endif // DJ_GLOVE_PC_COMMON_BLUETOOTH_H_

