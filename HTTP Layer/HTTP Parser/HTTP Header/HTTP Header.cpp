#include "HTTP Headers.h"
#include "Transition.h"



string HTTPHeader::Trim_Left(const string& S1)
{
	for (size_t i = 0; i < S1.length(); i++)
	{
		if (S1[i] != ' ' && S1[i] != '\t')
			return S1.substr(i);
	}

	return "";
}

string HTTPHeader::Trim_Right(const string& S1)
{
	for (int i = S1.length() - 1; i >= 0; i--)
	{
		if (S1[i] != ' ' && S1[i] != '\t')
			return S1.substr(0, i + 1);
	}

	return "";
}

string HTTPHeader::Trim(const string& S1)
{
	return Trim_Left(Trim_Right(S1));
}

string HTTPHeader::toLower(string S1)
{
	for (char& c : S1)
	{
		c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
	}

	return S1;
}


bool HTTPHeader::isToken(string str)
{
	for (char ch : str)
	{
		bool found = false;

		switch (ch)
		{
		case '#':
		case '$':
		case '%':
		case '&':
		case '\'':
		case '*':
		case '+':
		case '-':
		case '.':
		case '^':
		case '_':
		case '`':
		case '|':
		case '~':
			found = true;
			break;

		default:
			if ((ch >= 'A' && ch <= 'Z') ||
				(ch >= 'a' && ch <= 'z') ||
				(ch >= '0' && ch <= '9'))
			{
				found = true;
			}
			break;
		}

		if (!found)
			return false;
	}

	return true;
}

bool HTTPHeader::HeaderParser(vector<char>& cbytes)
{
	string line(cbytes.begin(), cbytes.end());

	size_t colon_pos = line.find(':');

	if (colon_pos == string::npos)
	{
		cout << "Unable HTTP Format Header!\n";
		return false;
	}

	string key_delim = line.substr(0, colon_pos);
	if (!key_delim.empty() && key_delim == " " || key_delim == "\t")
	{
		cout << "Unable HTTP Format Header!\n";
		return false;
	}

	string key = Trim(key_delim);
	string value = Trim(line.substr(colon_pos + 1));

	if (key.empty())
		return false;

	if (!isToken(key))
	{
		cout << "Invalid HTTP Token name: " << key << endl;
		return false;
	}

	Set(key, value);

	return true;
}

void HTTPHeader::Reset()
{
	header_map.clear();
}


string HTTPHeader::Get(const string& name) const
{
	auto it = header_map.find(toLower(name));

	if (it != header_map.end())
	{
		return it->second;
	}

	return "";
}

void HTTPHeader::Set(const string& name, const string& value)
{

	header_map[toLower(name)] = value;
}

int HTTPHeader::getLen(const string& name, int default_value)
{
	string str = Get(name);
	if (str.empty())
	{
		return default_value;
	}
	int value = stoi(str);
	return value;
}