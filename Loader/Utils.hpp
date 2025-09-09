#pragma once
#include <string>

namespace Utils {
    DWORD FindProcessId(const std::string& processName);
    bool VerifyAuth(const std::string& url);
}
