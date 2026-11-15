#include "Router.h"
#include <fstream>

using namespace std;

void Router::Routing(const string& path, TCPConnection& connection)
{
    if (path == "/mp4")
    {
        ifstream file("sample.mp4", ios::binary);
        if (!file.is_open())
        {
            string err = HTTPResponse::Handler(HTTPResponse::StatusInteralServerError, "<h1>Video Not Found</h1>");
            connection.sendData(err);
            return;
        }

        response.header.Set("Transfer-Encoding", "chunked");
        response.header.Set("Content-Type", "video/mp4");
        string headers = response.WriteStatusLine(response.StatusOK) + response.WriteHeaders();
        connection.sendData(headers);

        const size_t buffer_size = 4096;
        vector<char> buffer(buffer_size);

        while (file.read(buffer.data(), buffer_size))
        {
            string chunk = chunker.ChunkFormat(buffer.data(), file.gcount());
            connection.sendData(chunk);
        }
        if (file.gcount() > 0)
        {
            string chunk = chunker.ChunkFormat(buffer.data(), file.gcount());
            connection.sendData(chunk);
        }

        connection.sendData(chunker.FinalChunk());
    }
    else if (path == "/png")
    {
        ifstream file("image.png", ios::binary);
        if (!file.is_open())
        {
            string err = HTTPResponse::Handler(HTTPResponse::StatusInteralServerError, "<h1>Image Not Found</h1>");
            connection.sendData(err);
            return;
        }

        binary.Clear();
        const size_t buffer_size = 4096;
        vector<char> buffer(buffer_size);
        while (file.read(buffer.data(), buffer_size) || file.gcount() > 0)
        {
            binary.Append(buffer.data(), file.gcount());
        }

        response.header.Set("Content-Type", "image/png");
        response.header.Set("Content-Length", to_string(binary.GetSize()));
        string headers = response.WriteStatusLine(response.StatusOK) + response.WriteHeaders();
        connection.sendData(headers);

        const auto& raw_data = binary.GetData();
        connection.sendData(string(raw_data.begin(), raw_data.end()));
    }
    else
    {
        string body = "<h1>Welcome to Ziad's HTTP Server!</h1>";
        string resp = HTTPResponse::Handler(HTTPResponse::StatusOK, body);
        connection.sendData(resp);
    }
}