#pragma once
#include <string>

// ==== XOR String Macro ====
#define XOR(s) ([]() -> std::string {                     \
    std::string _x = s;                                   \
    for (size_t _i = 0; _i < _x.size(); ++_i)             \
        _x[_i] ^= 0x5A;                                   \
    return _x;                                            \
}())
