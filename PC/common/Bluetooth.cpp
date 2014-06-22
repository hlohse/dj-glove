#include "Bluetooth.h"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
using namespace std;

Bluetooth::Bluetooth(const BluetoothDevice& device)
:   device_(device),
    socket_(socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)),
    is_ready_(false)
{
    SetSocketNonBlocking();
}

Bluetooth::~Bluetooth()
{
    ShutdownSocket();
}

void Bluetooth::ShutdownSocket()
{
    shutdown(socket_, SHUT_RDWR);
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
    struct sockaddr_rc address = device_.GetSocketAddress();

    connect(socket_, (struct sockaddr*) &address, sizeof(address));

    if (select(socket_ + 1, NULL, &sockets, NULL, &timeout) == 1) {
        int error = GetSocketError();
    
        if (error == 0) {
            is_ready_ = true;
        }
        else {
            last_error_.clear();
            last_error_ << "Error " << error << " on socket";
        }
    }
    else {
        last_error_.clear();
        last_error_ << "Timeout while connecting socket";
    }
}

void Bluetooth::SetSocketBlocking() const
{
    int socket_flags = fcntl(socket_, F_GETFL, 0);
    socket_flags ^= O_NONBLOCK;
    fcntl(socket_, F_SETFL, socket_flags);
}

void Bluetooth::SetSocketNonBlocking() const
{
    int socket_flags = fcntl(socket_, F_GETFL, 0);
    socket_flags |= O_NONBLOCK;
    fcntl(socket_, F_SETFL, socket_flags);
}

int Bluetooth::GetSocketError() const
{
    int error;
    socklen_t length = sizeof(error);
    getsockopt(socket_, SOL_SOCKET, SO_ERROR, &error, &length);
    return error;
}

//  ISerial

bool Bluetooth::IsReady() const
{
    return is_ready_;
}

int Bluetooth::Available()
{
    int available;
    int error;

    error = ioctl(socket_, FIONREAD, &available);

    if (error != 0) {
        last_error_.clear();
        last_error_ << "Error " << error << " while counting socket data";
        ShutdownSocket();
        return 0;
    }

    return available;
}

void Bluetooth::WaitUntilAvailable()
{
    fd_set sockets;
    FD_ZERO(&sockets);
    FD_SET(socket_, &sockets);

    if (select(socket_ + 1, NULL, &sockets, NULL, NULL) == 1) {
        int error = GetSocketError();

        if (error != 0) {
            last_error_.clear();
            last_error_ << "Error " << error << " on socket while waiting for data";
            ShutdownSocket();
        }
    }
    else {
        last_error_.clear();
        last_error_ << "Error while waiting for socket data";
        ShutdownSocket();
    }
}

string Bluetooth::Read(const int length)
{
    string result;
    int status;

    if (length <= 0) {
        return "";
    }

    result.resize(length);
    status = read(socket_, (void*) result.c_str(), length);
    
    if (status < 0) {
        last_error_ << "Error while reading socket data";
        ShutdownSocket();
        return "";
    }

    return result;
}

int Bluetooth::Write(const string& output, const int length)
{
    int status;
    int output_length = (int) output.length();
    int max_length = output_length < length ? output_length : length;

    if (length <= 0) {
        return 0;
    }

    status = write(socket_, output.c_str(), max_length);

    if (status < 0) {
        last_error_ << "Error while writing socket data";
        ShutdownSocket();
        return 0;
    }

    return status;
}

