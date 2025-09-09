#include "Injector.hpp"
#include "ManualMap.hpp"

bool Injector::ManualMap(DWORD pid, const std::string& dllPath) {
    return ManualMap::Inject(pid, dllPath.c_str());
}
