#include "ManualMap.hpp"
#include <Windows.h>
#include <fstream>
#include <vector>
#include <TlHelp32.h>
#include <Psapi.h>

bool WriteData(HANDLE hProcess, LPVOID lpBaseAddress, const void* data, size_t size) {
    return WriteProcessMemory(hProcess, lpBaseAddress, data, size, nullptr);
}

std::vector<BYTE> ReadDllFile(const char* path) {
    std::ifstream file(path, std::ios::binary);
    return std::vector<BYTE>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

LPVOID AllocRemoteMemory(HANDLE hProcess, SIZE_T size) {
    return VirtualAllocEx(hProcess, nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
}

bool ManualMap::Inject(DWORD pid, const char* dllPath) {
    std::vector<BYTE> dllBytes = ReadDllFile(dllPath);
    if (dllBytes.empty()) return false;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) return false;

    // Allocate memory in target
    LPVOID remoteAddr = AllocRemoteMemory(hProcess, dllBytes.size());
    if (!remoteAddr) {
        CloseHandle(hProcess);
        return false;
    }

    // Write DLL bytes into remote process
    if (!WriteData(hProcess, remoteAddr, dllBytes.data(), dllBytes.size())) {
        VirtualFreeEx(hProcess, remoteAddr, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    // Shellcode to manually map the DLL goes here (real manual mapping requires PE parsing, relocations, etc.)
    // For now we simulate a basic "LoadLibrary" fallback for testing
    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    FARPROC loadLibrary = GetProcAddress(hKernel32, "LoadLibraryA");

    // Write DLL path for LoadLibraryA
    LPVOID pathAddr = VirtualAllocEx(hProcess, nullptr, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    WriteProcessMemory(hProcess, pathAddr, dllPath, strlen(dllPath) + 1, nullptr);

    // Create remote thread (can be replaced by queueing APC or shellcode stub)
    HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0,
        (LPTHREAD_START_ROUTINE)loadLibrary, pathAddr, 0, nullptr);

    if (!hThread) {
        VirtualFreeEx(hProcess, remoteAddr, 0, MEM_RELEASE);
        VirtualFreeEx(hProcess, pathAddr, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    // Wait for completion
    WaitForSingleObject(hThread, INFINITE);

    // Cleanup
    VirtualFreeEx(hProcess, remoteAddr, 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, pathAddr, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);
    return true;
}
