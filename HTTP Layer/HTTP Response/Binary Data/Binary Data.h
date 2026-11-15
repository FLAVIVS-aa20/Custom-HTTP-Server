#pragma once
#include <iostream>
#include <vector>
#include <cstdint>

class BinaryBody
{
private:
    std::vector<char> raw_binary_data;

public:
    BinaryBody() = default;

    void Reserve(size_t size) {
        raw_binary_data.reserve(size);
    }

    void Append(const char* buffer, size_t length) {
        if (buffer != nullptr && length > 0) {
            raw_binary_data.insert(raw_binary_data.end(), buffer, buffer + length);
        }
    }

    const std::vector<char>& GetData() const {
        return raw_binary_data;
    }

    std::vector<char> MoveData() {
        return std::move(raw_binary_data);
    }

    size_t GetSize() const {
        return raw_binary_data.size();
    }

    void Clear() {
        raw_binary_data.clear();
    }
};