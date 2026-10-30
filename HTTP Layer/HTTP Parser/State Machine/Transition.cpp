#include "Transition.h"
#include "Request Line.h"
#include "HTTP Headers.h"
#include "HTTP Body.h"
optional<Transition::States> Transition::TransitionParsing(char biteLine)
{
	switch (state)
	{
	case(StateInit):
	{
		internal_buffer.push_back(biteLine);

		size_t len = internal_buffer.size();

		if (len >= 2 && internal_buffer[len - 2] == '\r' && internal_buffer[len - 1] == '\n')
		{

			if (len == 2)
			{
				internal_buffer.clear();
				return state;
			}
			vector<char> streamLine(internal_buffer.begin(), internal_buffer.end() - 2);
			internal_buffer.clear();
			auto ParsedData = HTTPRequestLine::ParseLineRequest(streamLine);

			if (!ParsedData.has_value())
			{
				state = StateError;
				return state;
			}
			

			requestline->parsedRequest = ParsedData.value();

			state = StateHeader;
			return state;
		}

		return state;
	}

	case(StateHeader):
	{
		internal_buffer.push_back(biteLine);
		size_t len = internal_buffer.size();
		if (len >= 2 && internal_buffer[len - 2] == '\r' && internal_buffer[len - 1] == '\n')
		{
			if (len == 2)
			{
				internal_buffer.clear();
				int length = header->getLen("Content-Length", 0);
				if (length > 0)
				{
					if (body != nullptr)
					{
						body->Clear();
						body->Reserve(length);
					}
					state = StateBody;
				}
				else
				{
					state = StateDone;
				}
				return state;
			}
			vector<char> line_bytes(internal_buffer.begin(), internal_buffer.end() - 2);
			internal_buffer.clear();
			auto parsedData = header->HeaderParser(line_bytes);

			if (!parsedData)
			{
				state = StateError;
				return state;
			}
		}
		return state;
	}

	case(StateBody):
	{
		
		if (body != nullptr)
		{
			body->Append(biteLine);

		}
		size_t expectedLength = header->getLen("Content-Length", 0);
		if (body != nullptr && body->GetSize() >= expectedLength)
		{
			state = StateDone;
		}
		return state;

	}

	case(StateDone):
	{
		return state;
	}

	case(StateError):
	{
		internal_buffer.clear();
		return state;
	}
	}

	return nullopt;
}