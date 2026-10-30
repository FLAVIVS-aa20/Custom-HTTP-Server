#pragma once
#include <vector>
#include <optional>

using namespace std;

class HTTPRequestLine;
class HTTPHeader;
class HTTPBody;
class StreamReader; 


class Transition
{
private:
	vector<char> internal_buffer;

public:
	enum States
	{
		StateInit,
		StateHeader,
		StateBody,
		StateDone,
		StateError
	};

	States state = StateInit;

	
	HTTPRequestLine* requestline = nullptr;
	HTTPHeader* header = nullptr;
	HTTPBody* body = nullptr;
	StreamReader* reader = nullptr; 

	optional<States> TransitionParsing(char biteLine);

	
	bool Satisfied_RequestLine() const
	{
		return state == StateHeader;
	}

	bool Satisfied_Header() const
	{
		return state == StateDone;
	}

	bool isDone() const
	{
		return state == StateDone;
	}

	bool isError() const
	{
		return state == StateError;
	}
};