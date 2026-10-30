#pragma once
#include <iostream>
#include <vector>
#include "HTTP Headers.h"
#include "TCP_Connection.h"
#include "Response.h"
#include "Chunked Encoding.h"
#include "Binary Data.h"
#include <sstream>
class Router
{
	public:
		void Routing(const string& path, TCPConnection& connection);
		HTTPHeader header;
		HTTPResponse response;
		ChunkedEncoding chunker;
		BinaryBody binary;
};
