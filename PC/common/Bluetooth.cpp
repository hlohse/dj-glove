#include "Bluetooth.h"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
using namespace std;

Bluetooth::Bluetooth(const BluetoothDevice& device,
                     const int read_socket_buffer_bytes)
:   device_(device),
    socket_(socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)),
    is_ready_(false),
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

    SetSocketNonBlocking();
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
    
    bytes_read = read(socket_,
                      read_socket_buffer_,
                      read_socket_buffer_bytes_);
    
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
    int bytes_written = write(socket_,
                              output.c_str(),
                              output.length());

    if (bytes_written < 0) {
        last_error_ << "Error while writing socket data";
        ShutdownSocket();
        return 0;
    }

    return bytes_written;
}

