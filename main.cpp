#include <iostream>
#include <string>
#include <memory>
#include "TCP_Socket.h"
#include "TCP_Connection.h"
#include "Stream Reader.h"
#include "Router.h"

using namespace std;

int main() {
	const string HOST = "0.0.0.0";
	const unsigned short PORT = 8080;

	TCPSocket serverSocket;
	if (!serverSocket.createSocket() || !serverSocket.bindSocket(HOST, PORT) || !serverSocket.listenSocket()) {
		cerr << "[ERROR] Server setup failed." << endl;
		return 1;
	}

	cout << "[INFO] Server running on http://" << HOST << ":" << PORT << endl;

	while (true) {
		std::unique_ptr<TCPSocket> clientSocketHandle = serverSocket.acceptConnection();
		if (!clientSocketHandle) continue;

		DWORD timeout = 1000;
		setsockopt(clientSocketHandle->getHandle(), SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

		TCPConnection connection(clientSocketHandle.get());

		cout << "\n------------------------------------------" << endl;
		cout << "[NET] Client Connected!" << endl;

		StreamReader streamReader;
		char chunkBuffer[512];
		bool clientClosedPrematurely = false;

		while (connection.isConnected()) {
			int bytesRead = connection.receiveData(chunkBuffer, sizeof(chunkBuffer));

			if (bytesRead <= 0) {
				clientClosedPrematurely = true;
				break;
			}

			if (!streamReader.ProcessChunk(chunkBuffer, bytesRead)) {
				cerr << "[PARSER ERROR] Invalid HTTP Request Format during streaming." << endl;
				break;
			}

			if (streamReader.transition->isDone()) {
				break;
			}
		}

		if (streamReader.transition->isDone()) {
			const auto& reqLine = streamReader.getRequestLine();
			const auto& reqHeader = streamReader.getHeaders();
			const auto& reqBody = streamReader.getBody();

			cout << "[PARSER SUCCESS] HTTP Request Parsed Cleanly!" << endl;
			cout << " > Method:        " << reqLine.parsedRequest.method << endl;
			cout << " > Target:        " << reqLine.parsedRequest.target << endl;
			cout << " > Version:       " << reqLine.parsedRequest.version << endl;
			cout << " > Host:          " << reqHeader.Get("Host") << endl;
			cout << " > User Agent:    " << reqHeader.Get("User-Agent") << endl;
			cout << " > Content-Type:  " << reqHeader.Get("Content-Type") << endl;
			cout << " > Content-Length:" << reqHeader.Get("Content-Length") << endl;

			
			if (reqBody.GetSize() > 0) {
				cout << " > Body Payload:  " << reqBody.GetAsString() << endl;
			}

			Router router;
			router.Routing(reqLine.parsedRequest.target, connection);
		}
		else {
			cerr << "[PARSER ERROR] Invalid HTTP Request Format." << endl;
			string errorResponse = HTTPResponse::Handler(HTTPResponse::StatusBadRequest, "<h1>400 Bad Request</h1>");
			connection.sendData(errorResponse);
		}

		connection.closeConnection();
		cout << "[NET] Connection closed." << endl;
	}

	serverSocket.closeSocket();
	return 0;
}