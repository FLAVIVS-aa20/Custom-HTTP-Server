#pragma once
#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <sstream>

using namespace std;

class Transition;

class HTTPRequestLine
{
private:
	
	static constexpr const char* HTTP_Format_Error = "Bad Request 400";

	string path;
public:
	struct requestAttributes
	{
		string method;
		string target;
		string version;
	};

	requestAttributes parsedRequest;

	static vector<string> splitBytes(vector<char>& buffer, char delim);

	static optional<requestAttributes> ParseLineRequest(vector<char>& Line_req);


	static bool ValidateHTTP(const requestAttributes& request);

	string GetPath()const
	{
		return path;
	}

	Transition* transition;

};