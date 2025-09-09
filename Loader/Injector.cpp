#include "Injector.hpp"
#include <tlhelp32.h>

DWORD GetProcessIdByName(const std::string& name) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;

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

bool Injector::InjectDLL(const std::string& processName, const std::string& dllPath) {
    DWORD pid = GetProcessIdByName(processName);
    if (!pid) return false;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) return false;

    LPVOID alloc = VirtualAllocEx(hProcess, NULL, dllPath.size(), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!alloc) {
        CloseHandle(hProcess);
        return false;
    }

    WriteProcessMemory(hProcess, alloc, dllPath.c_str(), dllPath.size(), NULL);
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, alloc, 0, NULL);

    CloseHandle(hThread);
    CloseHandle(hProcess);
    return true;
}
