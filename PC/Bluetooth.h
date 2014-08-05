#ifndef DJ_GLOVE_PC_BLUETOOTH_H_
#define DJ_GLOVE_PC_BLUETOOTH_H_

#include "CommonSerial.h"
#include "BluetoothDevice.h"
#include "Export.h"

#ifdef __linux__
#include <sys/time.h>
#endif

class EXPORT Bluetooth : public CommonSerial {
public:
#ifdef __linux__
	using Socket = int;
#elif _WIN32
	using Socket = SOCKET;
#endif

	// Call once first
	static bool SetUp();

	// Call on termination
	static void TearDown();

    Bluetooth(const BluetoothDevice& device,
              const int read_socket_buffer_bytes);
    virtual ~Bluetooth();

    void Connect(const int timeout_s);

    char PeekFirst() const;

    // ISerial
    virtual bool IsReady() const;
    virtual void WaitUntilAvailable(const int length);
    virtual int Available() const;
    virtual std::string Read(const int length);
    virtual int Write(const std::string& output);

private:
    BluetoothDevice device_;
    bool is_ready_;
    std::string buffer_;
    int read_socket_buffer_bytes_;
	char* read_socket_buffer_;
	Socket socket_;

    void ShutdownSocket();
    void ConnectSocket(struct timeval timeout, fd_set sockets);
    void ReadSocket();

    void SetSocketBlocking() const;
    void SetSocketNonBlocking() const;
    int GetSocketError() const;
};

#endif // DJ_GLOVE_PC_BLUETOOTH_H_

