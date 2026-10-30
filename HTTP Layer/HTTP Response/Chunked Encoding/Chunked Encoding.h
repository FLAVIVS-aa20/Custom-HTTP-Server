#pragma once

#include <iostream>
#include <sstream>
#include "HTTP Headers.h"
#include "Response.h"

class ChunkedEncoding
{
	public :

		HTTPHeader header;

		void SetChunkHeaders();

		string ChunkFormat(const char *buffer, size_t length);

		string FinalChunk();

};
