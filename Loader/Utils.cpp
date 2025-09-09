#include "Utils.hpp"
#include <TlHelp32.h>
#include <fstream>

DWORD Utils::FindProcessId(const std::wstring& name) {
    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(PROCESSENTRY32W);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32FirstW(snapshot, &entry)) {
        do {
            if (!_wcsicmp(entry.szExeFile, name.c_str())) {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return 0;
}

bool Utils::ReadFileBytes(const std::string& path, std::vector<BYTE>& out) {
    std::ifstream file(path, std::ios::binary);
    if (!file.good()) return false;

    file.unsetf(std::ios::skipws);
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    out.reserve(size);
    out.insert(out.begin(), std::istream_iterator<BYTE>(file), {});
    return true;
}
