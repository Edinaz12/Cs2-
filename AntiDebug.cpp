#include "AntiDebug.hpp"
#include "XOR.hpp"
#include <Windows.h>
#include <chrono>
#include <string>
#include <iostream>

bool IsPatched = false;

void AntiDebug::PatchIsDebuggerPresent() {
    if (IsPatched) return;

    void* addr = (void*)GetProcAddress(GetModuleHandleA(XOR("kernel32.dll").c_str()), XOR("IsDebuggerPresent").c_str());
    if (addr) {
        DWORD oldProtect;
        VirtualProtect(addr, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
        *(BYTE*)addr = 0x31; // xor eax, eax
        VirtualProtect(addr, 1, oldProtect, &oldProtect);
        IsPatched = true;
    }
}

void AntiDebug::HideFromDebugger() {
    using pNtSetInformationThread = NTSTATUS(WINAPI*)(HANDLE, ULONG, PVOID, ULONG);
    auto NtSetInformationThread = (pNtSetInformationThread)GetProcAddress(
        GetModuleHandleA(XOR("ntdll.dll").c_str()), XOR("NtSetInformationThread").c_str());

    if (NtSetInformationThread)
        NtSetInformationThread(GetCurrentThread(), 0x11, nullptr, 0);
}

void AntiDebug::RunChecks() {
    // CheckRemoteDebuggerPresent
    BOOL isRemoteDebuggerPresent = FALSE;
    CheckRemoteDebuggerPresent(GetCurrentProcess(), &isRemoteDebuggerPresent);
    if (isRemoteDebuggerPresent)
        ExitProcess(0);

    // NtQueryInformationProcess
    typedef NTSTATUS(WINAPI* pNtQueryInformationProcess)(HANDLE, UINT, PVOID, ULONG, PULONG);
    auto NtQIP = (pNtQueryInformationProcess)GetProcAddress(
        GetModuleHandleA(XOR("ntdll.dll").c_str()), XOR("NtQueryInformationProcess").c_str());

    DWORD debuggerPresent = 0;
    if (NtQIP) {
        NtQIP(GetCurrentProcess(), 0x07, &debuggerPresent, sizeof(debuggerPresent), nullptr);
        if (debuggerPresent)
            ExitProcess(0);
    }

    // OutputDebugString Trap
    SetLastError(0);
    OutputDebugStringA(XOR("test").c_str());
    if (GetLastError() != 0)
        ExitProcess(0);

    // Timing Check (Stepping)
    auto t1 = std::chrono::high_resolution_clock::now();
    for (volatile int i = 0; i < 100000; ++i); // Dummy Loop
    auto t2 = std::chrono::high_resolution_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() > 100)
        ExitProcess(0);
}

void AntiDebug::Init() {
    PatchIsDebuggerPresent();
    HideFromDebugger();
    RunChecks();
}
