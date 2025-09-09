#pragma once
#include <Windows.h>
#include <vector>
#include <string>

namespace Utils {
    DWORD FindProcessId(const std::wstring& name);
    bool ReadFileBytes(const std::string& path, std::vector<BYTE>& out);
}
