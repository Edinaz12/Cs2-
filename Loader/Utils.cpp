#include "Utils.hpp"
#include <Windows.h>
#include <TlHelp32.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

DWORD Utils::FindProcessId(const std::string& name) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(entry);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    DWORD pid = 0;
    if (snapshot == INVALID_HANDLE_VALUE) return 0;

    while (Process32Next(snapshot, &entry)) {
        if (name == entry.szExeFile) {
            pid = entry.th32ProcessID;
            break;
        }
    }

    CloseHandle(snapshot);
    return pid;
}

bool Utils::VerifyAuth(const std::string& url) {
    HINTERNET hInternet = InternetOpenA("EdinazAuth", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) return false;

    HINTERNET hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return false;
    }

    char buffer[1024];
    DWORD bytesRead;
    bool success = false;

    if (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = 0;
        std::string response = buffer;
        if (response.find("success") != std::string::npos)
            success = true;
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    return success;
}
