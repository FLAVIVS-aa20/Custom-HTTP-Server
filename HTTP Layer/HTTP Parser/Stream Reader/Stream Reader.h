#pragma once
#include <iostream>
#include <optional>
#include <memory>
#include "Transition.h"
#include "Request Line.h"
#include "HTTP Headers.h"
#include "HTTP Body.h"

using namespace std;

class StreamReader
{

public:

    HTTPRequestLine requestline;
    HTTPHeader header;
    HTTPBody body;
    unique_ptr<Transition> transition;


    StreamReader();
    ~StreamReader();


    bool ProcessChunk(const char* bytes, size_t length);


    const HTTPRequestLine& getRequestLine() const
    {
        return requestline;
    }

    const HTTPHeader& getHeaders() const
    {
        return header;
    }

    const HTTPBody& getBody() const { return body; }
};