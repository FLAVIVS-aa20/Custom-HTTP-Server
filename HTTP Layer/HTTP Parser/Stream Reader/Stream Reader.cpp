#include "Stream Reader.h"
#include "Transition.h"
#include "Request Line.h"
#include "HTTP Headers.h"
#include "HTTP Body.h"


StreamReader::StreamReader() : transition(std::make_unique<Transition>())
{
	transition->requestline = &requestline;
	transition->header = &header;
	transition->body = &body;
	transition->reader = this;
}

StreamReader::~StreamReader() = default;

bool StreamReader::ProcessChunk(const char* bytes, size_t length)
{
	for (int i = 0; i < length; i++)
	{
		auto current_data = transition->TransitionParsing(bytes[i]);

		if (!current_data.has_value() || current_data.value() == Transition::StateError)
		{
			return false;
		}

		if (current_data.value() == Transition::StateDone)
		{
			return true;
		}
	}
	return true;
}
