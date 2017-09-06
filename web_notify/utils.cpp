#include "utils.h"
#include <iostream>

Utils::Utils()
{

}

std::string Utils::string_to_hex(const std::string &input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        output.append("0x");
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
        output.append(" ");
    }
    return output;
}

void Utils::print_hex_vector(const std::vector<uint8_t> &input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.size();
    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        output.append("0x");
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
        output.append(" ");
    }

    std::cout << output << std::endl;
}

void Utils::print_hex_vector2(float val)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = sizeof(val);
    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        output.append("0x");
        const unsigned char c = *(((uint8_t*)&val)+i);
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
        output.append(" ");
    }

    std::cout << output << std::endl;
}

