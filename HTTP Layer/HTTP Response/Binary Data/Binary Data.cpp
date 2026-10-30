#include <iostream>
#include "Binary Data.h"

using namespace std;

string BinaryBody::Binary_Data(const char* buffer, size_t length)
{
    if (length == 0 || buffer == nullptr)
    {
        return "";
    }

    return string(buffer, length);
}