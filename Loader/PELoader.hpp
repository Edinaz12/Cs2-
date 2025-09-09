#pragma once
#include <Windows.h>
#include <vector>
#include <string>

class PELoader {
public:
    bool ManualMap(HANDLE hProc, const std::vector<BYTE>& rawData, LPVOID& remoteBase, std::string& error);
};
