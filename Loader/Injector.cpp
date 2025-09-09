#include "Injector.hpp"
#include "ManualMap.hpp"
#include <tlhelp32.h>
#include <Windows.h>

DWORD GetProcessIdByName(const std::string& name) {
    PROCESSENTRY32 entry = {};
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    DWORD pid = 0;
    if (Process32First(snapshot, &entry)) {
        do {
            if (!_stricmp(entry.szExeFile, name.c_str())) {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return pid;
}

bool Injector::ManualMapInject(const std::string& procName, const std::string& dllPath) {
    DWORD pid = GetProcessIdByName(procName);
    if (!pid) return false;

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProc) return false;

    bool result = ManualMap::MapRemoteModule(hProc, dllPath.c_str());

    CloseHandle(hProc);
    return result;
}
