#pragma once
#include <Windows.h>

namespace ManualMap {
    bool Inject(DWORD pid, const char* dllPath);
}
