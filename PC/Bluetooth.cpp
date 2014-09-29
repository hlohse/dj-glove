#include "Bluetooth.h"
#include "Formatter.h"
#include <stdexcept>
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

#ifdef __linux__
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#endif

void Bluetooth::SetUp()
{
#ifdef _WIN32
    // Use winsock version 2.2
    const int version_major = 2;
    const int version_minor = 2;
	const WORD required_version = MAKEWORD(version_major, version_minor);
	WSADATA winsock_data;

	const int status = WSAStartup(required_version, &winsock_data);

    if (status != 0) {
		throw runtime_error(Formatter()
            << "WSAStartup failed with return code " << status);
	}

    const int actual_version_major = HIBYTE(winsock_data.wVersion);
    const int actual_version_minor = LOBYTE(winsock_data.wVersion);

	if (actual_version_major != version_major
	||  actual_version_minor != version_minor)
	{
		throw runtime_error(Formatter()
            << "Used winsock version "
            << actual_version_major << "." << actual_version_minor
            << " it not desired " << version_major << "." << version_minor);
	}
#endif
}

void Bluetooth::TearDown()
{
#ifdef _WIN32
	const int status = WSACleanup();

    if (status != 0) {
        throw runtime_error(Formatter()
            << "WSACleanup failed with return code " << status);
    }
#endif
}

Bluetooth::Bluetooth(const int read_socket_buffer_bytes)
:   device_(nullptr),
	buffer_(""),
    read_socket_buffer_bytes_(read_socket_buffer_bytes),
	read_socket_buffer_(NULL)
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
}

void Bluetooth::Connect(std::shared_ptr<BluetoothDevice> device,
                        const int timeout_s)
{
    struct timeval timeout;
	fd_set sockets;

#ifdef __linux__
	socket_ = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
#elif _WIN32
	socket_ = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
#endif

    if (!device->IsValid()) {
        throw runtime_error(Formatter() << "Invalid " << device_->ToString());
    }

    device_ = device;

    timeout.tv_sec  = timeout_s;
    timeout.tv_usec = 0;

    FD_ZERO(&sockets);
    FD_SET(socket_, &sockets);

    try {
        ConnectSocket(timeout, sockets);
    } catch (...) { throw; }
}

void Bluetooth::ConnectSocket(struct timeval timeout, fd_set sockets)
{
	BluetoothDevice::SocketAddress address = device_->GetSocketAddress();

    SetSocketNonBlocking();
    connect(socket_, (sockaddr*) &address, sizeof(address));
	
	switch (select(socket_ + 1, NULL, &sockets, NULL, &timeout)) {
	case 1: {
		int error = GetSocketError();

		if (error != 0) {
            throw runtime_error(Formatter()
                << "Error " << error << " on socket");
		}
		break;
	}
	case 0:
        throw runtime_error(Formatter()
            << "Timeout while connecting socket");
		break;
	default:
        throw runtime_error(Formatter()
#ifdef __linux__
		    << "Error " << errno << " while connecting socket");
#elif _WIN32
		    << "Error " << WSAGetLastError() << " while connecting socket");
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

int Bluetooth::Available() const
{
    return buffer_.length();
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
                ShutdownSocket();
                throw runtime_error(Formatter()
                    << "Error " << error << " on socket while waiting for data");
            }
            
            try {
                ReadSocket();
            } catch(...) { throw; }
        }
        else {
            ShutdownSocket();
            throw runtime_error(Formatter() <<
                "Error while waiting for socket data");
        }
    }
}

void Bluetooth::ReadSocket()
{
    int bytes_read;

    if (read_socket_buffer_ == NULL) {
        ShutdownSocket();
        throw runtime_error(Formatter()
            << "No read socket buffer available");
    }

    memset(read_socket_buffer_, 0, read_socket_buffer_bytes_ + 1);
    
#ifdef __linux__
    bytes_read = read(socket_,
                      read_socket_buffer_,
                      read_socket_buffer_bytes_);
#elif _WIN32
	bytes_read = recv(socket_,
					  read_socket_buffer_,
					  read_socket_buffer_bytes_,
					  0);
#endif
    
    if (bytes_read < 0) {
        ShutdownSocket();
        throw runtime_error(Formatter()
            << "Error " << errno << " while reading socket data");
    }

    if (bytes_read > 0) {
        buffer_ += read_socket_buffer_;
    }
}

string Bluetooth::ReadNextAvailable(const int length)
{
    try {
        WaitUntilAvailable(length);
    } catch (...) { throw; }

    return Read(length);
}

unsigned char Bluetooth::ReadNextAvailable()
{
	return ReadNextAvailable(1)[0];
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

unsigned char Bluetooth::Read()
{
	return Read(1)[0];
}

int Bluetooth::Write(const string& output)
{
#ifdef __linux__
    int bytes_written = write(socket_,
                              output.c_str(),
							  output.length());
#elif _WIN32
	int bytes_written = send(socket_,
							 output.c_str(),
							 output.length(),
							 0);
#endif

    if (bytes_written < 0) {
        ShutdownSocket();
        throw runtime_error(Formatter()
            << "Error " << errno << " while writing socket data");
    }

    return bytes_written;
}

int Bluetooth::Write(const unsigned char output)
{
	return Write(string(output, 1));
}


