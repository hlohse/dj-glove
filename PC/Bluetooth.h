#ifndef DJ_GLOVE_PC_BLUETOOTH_H_
#define DJ_GLOVE_PC_BLUETOOTH_H_

#include "Export.h"
#include "BluetoothDevice.h"
#include <memory>

#ifdef __linux__
#include <sys/time.h>
#endif

class EXPORT Bluetooth {
public:
#ifdef __linux__
	using socket_t = int;
#elif _WIN32
	using socket_t = SOCKET;
#endif

	// Call once first; throws exception on failure
    // Note: Call TearDown afterwards in case of failure!
	static void SetUp();

	// Call on termination; throw exception on failure
	static void TearDown();

    Bluetooth(const int read_socket_buffer_bytes);
    ~Bluetooth();

    // Throws exception in case of failure
    void Connect(std::shared_ptr<BluetoothDevice> device,
                 const int timeout_s);

    int Available() const;
    // Throws exception in case of failure
    void WaitUntilAvailable(const int length);
    
    char PeekFirst() const;
    // Throw expcetion in case of failure
    std::string ReadNextAvailable(const int length);
    // Throw expcetion in case of failure
    std::string Read(const int length);

    // Throw expcetion in case of failure
    int Write(const std::string& output);

private:
    std::shared_ptr<BluetoothDevice> device_;
    std::string buffer_;
	socket_t    socket_;
    int         read_socket_buffer_bytes_;
	char*       read_socket_buffer_;

    // Throws exception in case of failure
    void ConnectSocket(struct timeval timeout, fd_set sockets);
    void ShutdownSocket();
    
    // Throws exception in case of failure
    void ReadSocket();

    void SetSocketBlocking() const;
    void SetSocketNonBlocking() const;
    int  GetSocketError() const;
};

#endif // DJ_GLOVE_PC_BLUETOOTH_H_

