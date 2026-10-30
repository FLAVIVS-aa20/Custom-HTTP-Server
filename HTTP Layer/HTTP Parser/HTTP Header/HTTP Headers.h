#pragma once
#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <cctype>

using namespace std;

class Transition;

class HTTPHeader
{
	private:
		

	public:

		unordered_map<string, string> header_map;


		static string Trim_Left(const string& S1);

		static string Trim_Right(const string& S1);

		static string Trim(const string& S1);

		static string toLower(string S1);

		bool HeaderParser(vector<char>& cbytes);

		static bool isToken(string str);

		void Reset();

		string Get(const string& name) const;

		void Set(const string& name, const string& value);

		int getLen(const string& name, int default_value);

		Transition* transition;
};

