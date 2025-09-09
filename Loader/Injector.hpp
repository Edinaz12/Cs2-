#pragma once
#include <Windows.h>
#include <string>

namespace Injector {
    bool InjectDLL(const std::string& processName, const std::string& dllPath);
}
