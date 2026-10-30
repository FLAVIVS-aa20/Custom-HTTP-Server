#include "Chunked Encoding.h"
#include "HTTP Headers.h"

using namespace std;

void ChunkedEncoding::SetChunkHeaders()
{
	header.header_map.erase("Content-Length");
	header.Set("transfer-encoding", "chunked");

}

string ChunkedEncoding::ChunkFormat(const char *buffer, size_t length)
{

	stringstream ss;
	if (length == 0)
	{
		return "";
	}

	string chunk = "";
	ss << hex << length << "\r\n";

	chunk = ss.str();
	chunk.append(buffer, length);
	chunk += "\r\n";

	return chunk;
}

string ChunkedEncoding::FinalChunk()
{
	return "0\r\n\r\n";
}
