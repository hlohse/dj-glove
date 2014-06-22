#include "Bluetooth.h"
#include <sys/socket.h>
#include <fcntl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
using namespace std;

Bluetooth::Bluetooth(const BluetoothDevice& device)
:   device_(device),
    socket_(socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)),
    is_ready_(false)
{
}

Bluetooth::~Bluetooth()
{
    shutdown(socket_, SHUT_RDWR);
}

void Bluetooth::Connect(const int timeout_s)
{
    struct timeval timeout;
    fd_set sockets;
    
    last_error_.clear();

    if (!device_.IsValid()) {
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

    SetSocketNonBlocking();
    connect(socket_, (struct sockaddr*) &address, sizeof(address));

    if (select(socket_ + 1, NULL, &sockets, NULL, &timeout) == 1) {
        int error = GetSocketError();
    
        if (error == 0) {
            is_ready_ = true;
        }
        else {
            last_error_ << "Error " << error << " on socket";
        }
    }
    else {
        last_error_ << "Error while connecting socket";
    }

    SetSocketBlocking();
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

bool Bluetooth::IsReady() const
{
    return is_ready_;
}

int Bluetooth::Available() const
{
    return 0;
}

void Bluetooth::WaitUntilAvailable()
{
}

string Bluetooth::Read(const int length)
{
    return "";
}

int Bluetooth::Write(const string& output, const int length)
{
    return 0;
}

