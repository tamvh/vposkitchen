#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iomanip>
#include <sstream>
#include <vector>

class Utils
{
public:
    Utils();

    static std::string string_to_hex(const std::string& input);

    static void print_hex_vector(const std::vector<uint8_t>& input);
    static void print_hex_vector2(float val);

    template<class data>
    static std::string int_to_hex(data i)
    {
      std::stringstream stream;
      stream << std::setfill ('0') << std::setw(2*sizeof(i))
             << std::hex << i;
      return stream.str();
    }
};

#endif // UTILS_H
