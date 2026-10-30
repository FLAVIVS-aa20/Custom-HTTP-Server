#pragma once

#include "TCP_Socket.h"
#include <string>
#include <vector>
#include "Response.h"
#include "Request Line.h"
class TCPConnection {
private:
    TCPSocket* m_socket; // Raw pointer
    HTTPResponse response;
    HTTPRequestLine request;
public:
    TCPConnection(TCPSocket* socket);
    ~TCPConnection();

    // Send Operations
    bool sendData(const std::string& data);
    bool sendData(const char* buffer, size_t length);

    // Receive Operations
    int receiveData(char* buffer, size_t length);
    std::vector<char> receiveBytes(size_t bufferSize = 1024); // Clean vector format for HTTP Parsing
    std::string receiveString(size_t bufferSize = 1024);

    // Helpers
    bool isConnected() const;
    void closeConnection();

    TCPSocket* getSocket() const { return m_socket; }
};