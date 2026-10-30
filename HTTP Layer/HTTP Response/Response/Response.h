#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include "HTTP Headers.h" 

using namespace std;

class HTTPHeader;


class HTTPResponse
{
	public:

		HTTPHeader header;


		enum Status
		{
			StatusOK = 200,
			StatusBadRequest = 400,
			StatusInteralServerError = 500
		};

		string WriteStatusLine(Status status);

		void GetDefaultHeaders(int content_len);

		string WriteHeaders();

		static string Handler(Status status, const string& Body);

};
