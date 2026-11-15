#include "Response.h"
#include "HTTP Headers.h"
#include <sstream>

using namespace std;
string HTTPResponse::WriteStatusLine(Status status)
{
	switch (status)
	{
	case StatusOK:
		return "HTTP/1.1 200 OK\r\n";
	case StatusBadRequest:
		return "HTTP/1.1 400 Bad Request\r\n";
	case StatusInteralServerError:
	default:
		return "HTTP/1.1 500 Internal Server Error\r\n";
	}
}

void HTTPResponse::GetDefaultHeaders(int content_len)
{
	header.Set("Content-Length", to_string(content_len));
	header.Set("Content-Type", "text/html");
	header.Set("Connection", "close");
	
}

string HTTPResponse::WriteHeaders()
{
	string result;
	for (const auto& [key, value] : header.header_map)
	{
		result += key + ": " + value + "\r\n";
	}
	result += "\r\n";

	return result;
}

string HTTPResponse::Handler(Status status, const string& body)
{
	HTTPResponse response;

	string status_line = response.WriteStatusLine(status);
	response.GetDefaultHeaders(body.length());

	string headers = response.WriteHeaders();

	return status_line + headers + body;

	
}
