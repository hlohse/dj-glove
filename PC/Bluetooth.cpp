#include "Bluetooth.h"
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

#ifdef __linux__
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#elif _WIN32
#include <io.h>
#endif

bool Bluetooth::SetUp()
{
#ifdef __linux__
	return true;	// Nothing to do on Linux
#elif _WIN32
	const int version[2] = { 2, 2 };	// Use Winsock 2.2
	const WORD required_version = MAKEWORD(version[0], version[1]);
	WSADATA winsock_data;

	if (WSAStartup(required_version, &winsock_data) != 0) {
		return false;
	}

	if (LOBYTE(winsock_data.wVersion) != version[1]
	||  HIBYTE(winsock_data.wVersion) != version[0])
	{
		Bluetooth::TearDown();
		return false;
	}

	return true;
#endif
}

void Bluetooth::TearDown()
{
#ifdef __linux__
	return;	// Nothing to do on Linux
#elif _WIN32
	WSACleanup();
#endif
}

Bluetooth::Bluetooth(const BluetoothDevice& device,
                     const int read_socket_buffer_bytes)
:   device_(device),
    is_ready_(false),
    buffer_(""),
    read_socket_buffer_bytes_(read_socket_buffer_bytes),
	read_socket_buffer_(NULL),
#ifdef __linux__
	socket_(socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM))
#elif _WIN32
	socket_(socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM))
#endif
{
    if (read_socket_buffer_bytes_ > 0) {
        read_socket_buffer_ = new char[read_socket_buffer_bytes_ + 1];
    }
}

Bluetooth::~Bluetooth()
{
    ShutdownSocket();

    if (read_socket_buffer_ != NULL) {
        delete read_socket_buffer_;
    }   
}

void Bluetooth::ShutdownSocket()
{
#ifdef __linux__
    shutdown(socket_, SHUT_RDWR);
#elif _WIN32
	shutdown(socket_, SD_BOTH);
#endif

    is_ready_ = false;
}

void Bluetooth::Connect(const int timeout_s)
{
    struct timeval timeout;
    fd_set sockets;

    if (!device_.IsValid()) {
        last_error_.clear();
        last_error_ << "Invalid " << device_.ToString();
        return;
    }

    timeout.tv_sec  = timeout_s;
    timeout.tv_usec = 0;

    FD_ZERO(&sockets);
    FD_SET(socket_, &sockets);

    ConnectSocket(timeout, sockets);
}

void Bluetooth::ConnectSocket(struct timeval timeout, fd_set sockets)
{
    BluetoothDevice::SocketAddress address = device_.GetSocketAddress();

    SetSocketNonBlocking();
    connect(socket_, (struct sockaddr*) &address, sizeof(address));
	
	switch (select(socket_ + 1, NULL, &sockets, NULL, &timeout)) {
	case 1: {
		int error = GetSocketError();

		if (error == 0) {
			is_ready_ = true;
		}
		else {
			last_error_.clear();
			last_error_ << "Error " << error << " on socket";
		}
		break;
	}
	case 0:
		last_error_.clear();
		last_error_ << "Timeout while connecting socket";
		break;
	default:
		last_error_.clear();
#ifdef __linux__
		last_error_ << "Error " << errno << " while connecting socket";
#elif _WIN32
		last_error_ << "Error " << WSAGetLastError() << " while connecting socket";
#endif
		break;
	}

    SetSocketBlocking();
}

void Bluetooth::SetSocketBlocking() const
{
#ifdef __linux__
    int socket_flags = fcntl(socket_, F_GETFL, 0);
    socket_flags ^= O_NONBLOCK;
    fcntl(socket_, F_SETFL, socket_flags);
#elif _WIN32
	ULONG non_blocking = 0;
	ioctlsocket(socket_, FIONBIO, &non_blocking);
#endif
}

void Bluetooth::SetSocketNonBlocking() const
{
#ifdef __linux__
    int socket_flags = fcntl(socket_, F_GETFL, 0);
    socket_flags |= O_NONBLOCK;
    fcntl(socket_, F_SETFL, socket_flags);
#elif _WIN32
	ULONG non_blocking = 1;
	ioctlsocket(socket_, FIONBIO, &non_blocking);
#endif
}

int Bluetooth::GetSocketError() const
{
    int error;

#ifdef __linux__
    socklen_t
#elif _WIN32
	int
#endif
	length = sizeof(error);

    getsockopt(socket_, SOL_SOCKET, SO_ERROR, (char*) &error, &length);
    return error;
}

//  ISerial

bool Bluetooth::IsReady() const
{
    return is_ready_;
}

void Bluetooth::WaitUntilAvailable(const int length)
{
    fd_set sockets;
    FD_ZERO(&sockets);
    FD_SET(socket_, &sockets);

    while ((int) buffer_.length() < length) {
        if (select(socket_ + 1, NULL, &sockets, NULL, NULL) == 1) {
            int error = GetSocketError();

            if (error != 0) {
                last_error_.clear();
                last_error_ << "Error " << error << " on socket while waiting for data";
                ShutdownSocket();
                return;
            }

            ReadSocket();
        }
        else {
            last_error_.clear();
            last_error_ << "Error while waiting for socket data";
            ShutdownSocket();
            return;
        }
    }
}

void Bluetooth::ReadSocket()
{
    int bytes_read;

    if (read_socket_buffer_ == NULL) {
        last_error_ << "No read socket buffer available";
        ShutdownSocket();
        return;
    }

    memset(read_socket_buffer_, 0, read_socket_buffer_bytes_ + 1);
    
#ifdef __linux__
    bytes_read = read(socket_,
                      read_socket_buffer_,
                      read_socket_buffer_bytes_);
#elif _WIN32
	bytes_read = _read(socket_,
					   read_socket_buffer_,
					   read_socket_buffer_bytes_);
#endif
    
    if (bytes_read < 0) {
        last_error_ << "Error while reading socket data";
        ShutdownSocket();
        return;
    }

    if (bytes_read > 0) {
        buffer_ += read_socket_buffer_;
    }
}

int Bluetooth::Available()
{
    return buffer_.length();
}

string Bluetooth::Read(const int length)
{
    string input;
    
    if (buffer_.length() == 0 || length <= 0) {
        return "";
    }

    input = string(buffer_, 0, length);
    buffer_.erase(0, length);
    
    return input;
}

int Bluetooth::Write(const string& output)
{
#ifdef __linux__
    int bytes_written = write(socket_,
                              output.c_str(),
							  output.length());
#elif _WIN32
	int bytes_written = _write(socket_,
							   output.c_str(),
							   output.length());
#endif

    if (bytes_written < 0) {
        last_error_ << "Error while writing socket data";
        ShutdownSocket();
        return 0;
    }

    return bytes_written;
}

