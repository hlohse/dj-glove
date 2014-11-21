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

Bluetooth::Bluetooth(const int m_read_socket_bufferbytes)
:   m_device(nullptr),
    m_buffer(""),
    m_read_socket_buffer_bytes(m_read_socket_bufferbytes),
    m_read_socket_buffer(NULL)
{
    if (m_read_socket_buffer_bytes > 0) {
        m_read_socket_buffer = new char[m_read_socket_buffer_bytes + 1];
    }
}

Bluetooth::~Bluetooth()
{
    ShutdownSocket();

    if (m_read_socket_buffer != NULL) {
        delete m_read_socket_buffer;
    }   
}

void Bluetooth::ShutdownSocket()
{
#ifdef __linux__
    shutdown(m_socket, SHUT_RDWR);
#elif _WIN32
    shutdown(m_socket, SD_BOTH);
#endif
}

void Bluetooth::Connect(std::shared_ptr<BluetoothDevice> device,
                        const int timeout_s)
{
    struct timeval timeout;
    fd_set sockets;

#ifdef __linux__
    m_socket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
#elif _WIN32
    m_socket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
#endif

    if (!device->IsValid()) {
        throw runtime_error(Formatter() << "Invalid " << m_device->ToString());
    }

    m_device = device;

    timeout.tv_sec  = timeout_s;
    timeout.tv_usec = 0;

    FD_ZERO(&sockets);
    FD_SET(m_socket, &sockets);

    try {
        ConnectSocket(timeout, sockets);
    } catch (...) { throw; }
}

void Bluetooth::ConnectSocket(struct timeval timeout, fd_set sockets)
{
    BluetoothDevice::SocketAddress address = m_device->GetSocketAddress();

    SetSocketNonBlocking();
    connect(m_socket, (sockaddr*) &address, sizeof(address));
    
    switch (select(m_socket + 1, NULL, &sockets, NULL, &timeout)) {
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
    int m_socketflags = fcntl(m_socket, F_GETFL, 0);
    m_socketflags ^= O_NONBLOCK;
    fcntl(m_socket, F_SETFL, m_socketflags);
#elif _WIN32
    ULONG non_blocking = 0;
    ioctlsocket(m_socket, FIONBIO, &non_blocking);
#endif
}

void Bluetooth::SetSocketNonBlocking() const
{
#ifdef __linux__
    int m_socketflags = fcntl(m_socket, F_GETFL, 0);
    m_socketflags |= O_NONBLOCK;
    fcntl(m_socket, F_SETFL, m_socketflags);
#elif _WIN32
    ULONG non_blocking = 1;
    ioctlsocket(m_socket, FIONBIO, &non_blocking);
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

    getsockopt(m_socket, SOL_SOCKET, SO_ERROR, (char*) &error, &length);
    return error;
}

int Bluetooth::Available() const
{
    return m_buffer.length();
}

void Bluetooth::WaitUntilAvailable(const int length)
{
    fd_set sockets;
    FD_ZERO(&sockets);
    FD_SET(m_socket, &sockets);

    while ((int) m_buffer.length() < length) {
        if (select(m_socket + 1, NULL, &sockets, NULL, NULL) == 1) {
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

    if (m_read_socket_buffer == NULL) {
        ShutdownSocket();
        throw runtime_error(Formatter()
            << "No read socket buffer available");
    }

    memset(m_read_socket_buffer, 0, m_read_socket_buffer_bytes + 1);
    
#ifdef __linux__
    bytes_read = read(m_socket,
                      m_read_socket_buffer,
                      m_read_socket_buffer_bytes);
#elif _WIN32
    bytes_read = recv(m_socket,
                      m_read_socket_buffer,
                      m_read_socket_buffer_bytes,
                      0);
#endif
    
    if (bytes_read < 0) {
        ShutdownSocket();
        throw runtime_error(Formatter()
            << "Error " << errno << " while reading socket data");
    }

    if (bytes_read > 0) {
        m_buffer += m_read_socket_buffer;
    }
}

string Bluetooth::ReadNextAvailable(const int length)
{
    try {
        WaitUntilAvailable(length);
    } catch (...) { throw; }

    return Read(length);
}

char Bluetooth::ReadNextAvailable()
{
    return ReadNextAvailable(1)[0];
}

string Bluetooth::Read(const int length)
{
    string input;
    
    if (m_buffer.length() == 0 || length <= 0) {
        return "";
    }

    input = string(m_buffer, 0, length);
    m_buffer.erase(0, length);
    
    return input;
}

char Bluetooth::Read()
{
    return Read(1)[0];
}

int Bluetooth::Write(const string& output)
{
#ifdef __linux__
    int bytes_written = write(m_socket,
                              output.c_str(),
                              output.length());
#elif _WIN32
    int bytes_written = send(m_socket,
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

int Bluetooth::Write(const char output)
{
    char buffer[2];
    buffer[0] = output;
    buffer[1] = '\0';
    return Write(string(buffer));
}

void Bluetooth::Clear()
{
    Read(Available());
}

