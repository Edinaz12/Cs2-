#pragma once
#include <string>

namespace Injector {
    bool ManualMap(DWORD pid, const std::string& dllPath);
}
