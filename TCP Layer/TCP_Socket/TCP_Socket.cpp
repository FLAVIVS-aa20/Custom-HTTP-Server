#include "TCP_Socket.h"

using namespace std;

// Function to start Winsock once
static bool initWinsock() {
    static bool isInitialized = false;
    if (!isInitialized) {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            cerr << "WSAStartup failed: " << result << endl;
            return false;
        }
        isInitialized = true;
    }
    return true;
}

TCPSocket::TCPSocket() {
    m_sockfd = INVALID_SOCKET;
    initWinsock();
}

TCPSocket::TCPSocket(SOCKET fd) {
    m_sockfd = fd;
    initWinsock();
}

TCPSocket::~TCPSocket() {
    closeSocket();
}

bool TCPSocket::createSocket() {
    closeSocket(); // Close if open before creating a new one

    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockfd == INVALID_SOCKET) {
        cerr << "Failed to create socket: " << WSAGetLastError() << endl;
        return false;
    }
    return true;
}

bool TCPSocket::bindSocket(const string& ip, unsigned short port) {
    if (!isValid()) {
        cerr << "Cannot bind: Socket is invalid." << endl;
        return false;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    
    if (ip.empty() || ip == "0.0.0.0") {
        address.sin_addr.s_addr = INADDR_ANY;
    }
    else {
        inet_pton(AF_INET, ip.c_str(), &address.sin_addr);
    }

    int result = bind(m_sockfd, (SOCKADDR*)&address, sizeof(address));
    if (result == SOCKET_ERROR) {
        cerr << "Bind failed: " << WSAGetLastError() << endl;
        return false;
    }

    return true;
}

bool TCPSocket::listenSocket(int backlog) {
    if (!isValid()) return false;

    if (listen(m_sockfd, backlog) == SOCKET_ERROR) {
        cerr << "Listen failed: " << WSAGetLastError() << endl;
        return false;
    }
    return true;
}

unique_ptr<TCPSocket> TCPSocket::acceptConnection() {
    if (!isValid()) return nullptr;

    sockaddr_in clientAddr;
    int clientLen = sizeof(clientAddr);

    SOCKET clientFD = accept(m_sockfd, (sockaddr*)&clientAddr, &clientLen);
    if (clientFD == INVALID_SOCKET) {
    
        return nullptr;
    }

    return std::make_unique<TCPSocket>(clientFD);
}

void TCPSocket::closeSocket() {
    if (m_sockfd != INVALID_SOCKET) {
        closesocket(m_sockfd);
        m_sockfd = INVALID_SOCKET;
    }
}