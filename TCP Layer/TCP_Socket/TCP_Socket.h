#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <memory>
#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

class TCPSocket {
private:
    SOCKET m_sockfd;

public:
    TCPSocket();
    explicit TCPSocket(SOCKET fd);
    ~TCPSocket();

    TCPSocket(const TCPSocket&) = delete;
    TCPSocket& operator=(const TCPSocket&) = delete;

    TCPSocket(TCPSocket&& other) noexcept : m_sockfd(other.m_sockfd) {
        other.m_sockfd = INVALID_SOCKET;
    }
    TCPSocket& operator=(TCPSocket&& other) noexcept {
        if (this != &other) {
            closeSocket();
            m_sockfd = other.m_sockfd;
            other.m_sockfd = INVALID_SOCKET;
        }
        return *this;
    }

    bool createSocket();
    bool bindSocket(const std::string& ip, unsigned short port);
    bool listenSocket(int backlog = SOMAXCONN);

    std::unique_ptr<TCPSocket> acceptConnection();
    void closeSocket();

    SOCKET getHandle() const { return m_sockfd; }
    bool isValid() const { return m_sockfd != INVALID_SOCKET; }
};