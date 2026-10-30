#include "TCP_Connection.h"

TCPConnection::TCPConnection(TCPSocket* socket) {
    m_socket = socket;
}

TCPConnection::~TCPConnection() {
    closeConnection();
}

bool TCPConnection::sendData(const std::string& data) {
    return sendData(data.c_str(), data.length());
}

bool TCPConnection::sendData(const char* buffer, size_t length) {
    if (!isConnected()) return false;

    int totalSent = 0;
    int bytesLeft = (int)length;

    // Loop to handle partial sends from send()
    while (totalSent < (int)length) {
        int bytesSent = send(m_socket->getHandle(), buffer + totalSent, bytesLeft, 0);
        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
            return false;
        }

        totalSent += bytesSent;
        bytesLeft -= bytesSent;
    }

    return true;
}

int TCPConnection::receiveData(char* buffer, size_t length) {
    if (!isConnected()) return -1;

    int bytesReceived = recv(m_socket->getHandle(), buffer, (int)length, 0);

    if (bytesReceived == SOCKET_ERROR) {
        return -1;
    }
    else if (bytesReceived == 0) {
   
        closeConnection();
        return 0;
    }

    return bytesReceived;
}

// Receive raw bytes dynamically sized for HTTP Parsing
std::vector<char> TCPConnection::receiveBytes(size_t bufferSize) {
    std::vector<char> tempBuffer(bufferSize);
    int bytesRead = receiveData(tempBuffer.data(), bufferSize);

    if (bytesRead > 0) {
        // Resize vector to match exact number of read bytes
        tempBuffer.resize(bytesRead);
        return tempBuffer;
    }

    return std::vector<char>();
}

std::string TCPConnection::receiveString(size_t bufferSize) {
    std::vector<char> buffer = receiveBytes(bufferSize);

    if (!buffer.empty()) {
        return std::string(buffer.begin(), buffer.end());
    }

    return "";
}

bool TCPConnection::isConnected() const {
    return m_socket != nullptr && m_socket->isValid();
}

void TCPConnection::closeConnection() {
    if (m_socket != nullptr && m_socket->isValid()) {
        shutdown(m_socket->getHandle(), SD_SEND);
    }
}
