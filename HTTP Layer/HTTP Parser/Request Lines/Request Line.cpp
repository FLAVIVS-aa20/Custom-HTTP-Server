#include "Request Line.h"
#include "Transition.h"

vector<string> HTTPRequestLine::splitBytes(vector<char>& buffer, char delim)
{
	vector<string> vec;
	string word = "";

	for (char b : buffer)
	{
		if (b == delim)
		{
			if (!word.empty())
			{
				vec.push_back(word);
				word.clear();
			}
		}
		else
		{
			word += b;
		}
	}

	if (!word.empty())
	{
		vec.push_back(word);
	}

	return vec;
}

optional<HTTPRequestLine::requestAttributes> HTTPRequestLine::ParseLineRequest(vector<char>& Line_req)
{
	requestAttributes request;

	vector<string> parts = splitBytes(Line_req, ' ');

	if (parts.size() != 3)
	{

		cout << HTTP_Format_Error << endl;
		return nullopt;
	}

	request.method = parts[0];
	request.target = parts[1];
	request.version = parts[2];

	if (!ValidateHTTP(request))
	{
		cout << HTTP_Format_Error << endl;
		return nullopt;
	}

	return request;
}


bool HTTPRequestLine::ValidateHTTP(const requestAttributes& request)
{
	
	if (request.method != "GET" && request.method != "POST" &&
		request.method != "PUT" && request.method != "DELETE" &&
		request.method != "HEAD") {
		return false;
	}

	if (request.target.empty() || request.target[0] != '/') {
		return false;
	}

	if (request.version != "HTTP/1.1" && request.version != "HTTP/1.0") {
		return false;
	}

	return true;
}
