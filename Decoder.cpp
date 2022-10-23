#include "Decoder.h"
#include <vector>

std::string sjisToUtf8(const std::string& value)
{
    icu::UnicodeString src(value.c_str(), "shift_jis");
    int length = src.extract(0, src.length(), NULL, "utf8");

    std::vector<char> result(length + 1);
    src.extract(0, src.length(), &result[0], "utf8");

    return std::string(result.begin(), result.end() - 1);
}
