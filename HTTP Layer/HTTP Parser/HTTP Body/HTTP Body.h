#pragma once
#include <vector>
#include <string>

class HTTPBody
{
private:
	std::vector<char> raw_data;

public:
	HTTPBody() = default;

	// Pre-allocate memory using Content-Length to prevent reallocation overhead
	void Reserve(size_t expected_size)
	{
		raw_data.reserve(expected_size);
	}

	// Append a single byte passed from the FSM
	void Append(char byte)
	{
		raw_data.push_back(byte);
	}

	// Append a chunk of bytes directly
	void Append(const char* bytes, size_t length)
	{
		raw_data.insert(raw_data.end(), bytes, bytes + length);
	}

	void Clear()
	{
		raw_data.clear();
	}

	size_t GetSize() const
	{
		return raw_data.size();
	}

	// Returns the body as a std::string (for JSON / plain text parsing)
	std::string GetAsString() const
	{
		return std::string(raw_data.begin(), raw_data.end());
	}

	// Move body payload out cleanly without deep copying
	std::string MoveAsString()
	{
		std::string str(raw_data.begin(), raw_data.end());
		raw_data.clear();
		return str;
	}

	// Immutable reference for raw binary inspection
	const std::vector<char>& GetRawData() const
	{
		return raw_data;
	}
};
