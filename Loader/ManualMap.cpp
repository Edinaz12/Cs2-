#include "ManualMap.hpp"
#include <fstream>
#include <vector>
#include <windows.h>
#include <winnt.h>

bool ManualMap::MapRemoteModule(HANDLE hProcess, const char* dllPath) {
    std::ifstream file(dllPath, std::ios::binary);
    if (!file.good()) return false;

    std::vector<char> dllBuffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    auto dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(dllBuffer.data());
    auto ntHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(dllBuffer.data() + dosHeader->e_lfanew);

    SIZE_T imageSize = ntHeaders->OptionalHeader.SizeOfImage;
    LPVOID remoteImage = VirtualAllocEx(hProcess, nullptr, imageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!remoteImage) return false;

    // Write Headers
    WriteProcessMemory(hProcess, remoteImage, dllBuffer.data(), ntHeaders->OptionalHeader.SizeOfHeaders, nullptr);

    // Write Sections
    auto section = IMAGE_FIRST_SECTION(ntHeaders);
    for (int i = 0; i < ntHeaders->FileHeader.NumberOfSections; ++i) {
        LPVOID remoteSectionAddr = (LPBYTE)remoteImage + section[i].VirtualAddress;
        LPVOID localSectionAddr = dllBuffer.data() + section[i].PointerToRawData;
        WriteProcessMemory(hProcess, remoteSectionAddr, localSectionAddr, section[i].SizeOfRawData, nullptr);
    }

    // Call DllMain remotely
    LPTHREAD_START_ROUTINE remoteEntry = (LPTHREAD_START_ROUTINE)((LPBYTE)remoteImage + ntHeaders->OptionalHeader.AddressOfEntryPoint);
    HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, remoteEntry, remoteImage, 0, nullptr);
    if (!hThread) return false;

    CloseHandle(hThread);
    return true;
}
