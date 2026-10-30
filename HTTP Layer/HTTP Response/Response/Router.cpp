#include <iostream>
#include "Router.h"

using namespace std;

void Router::Routing(const string& path, TCPConnection& connection)
{
	
	if (path == "/mp4")
	{
		stringstream ss;
		//chunker.SetChunkHeaders(response);
		response.header.Set("Transfer-Encoding", "chunked");
		response.header.Set("Content-Type", "video/mp4"); 

		string headers = response.WriteStatusLine(response.StatusOK) + response.WriteHeaders();
		connection.sendData(headers);

		vector <char> buffer(1024);
		
		string chunk = chunker.ChunkFormat(buffer.data(), buffer.size());
		connection.sendData(chunk);
		
		connection.sendData(chunker.FinalChunk());
	}

	/*else if (path == "/PNG")
	{
		header.Set("Content-Type", "image/PNG");
		string headers = response.WriteStatusLine(response.StatusOK) + response.WriteHeaders();
		connection.sendData(headers);

		vector <char> buffer(1024, 'A');
		string byte = binary.Binary_Data(buffer.data(), buffer.size());
		connection.sendData(byte);

	}*/

	else
	{
		string body = "<h1>Welcome to Ziad's HTTP Server!</h1>";
		string response = HTTPResponse::Handler(HTTPResponse::StatusOK, body);
		connection.sendData(response);
	}
}