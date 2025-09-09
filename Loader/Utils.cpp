#include "Utils.hpp"
#include <Windows.h>

std::string XOR(const std::string& input) {
    std::string out = input;
    for (char& c : out) c ^= 0x5A;
    return out;
}
