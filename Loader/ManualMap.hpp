#pragma once
#include <Windows.h>

namespace ManualMap {
    bool MapRemoteModule(HANDLE hProcess, const char* dllPath);
}
