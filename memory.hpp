#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <cmath>
#include <vector>
#include "globals.hpp" // Für Vector3

// ==== Entity Struct ====
struct Entity {
    bool visible = false;
    int id = 0;
    int health = 0;
    int team = 0;
    Vector3 pos = { 0.f, 0.f, 0.f };

    float DistanceTo(const Vector3& localPos) const {
        float dx = pos.x - localPos.x;
        float dy = pos.y - localPos.y;
        float dz = pos.z - localPos.z;
        return sqrtf(dx * dx + dy * dy + dz * dz);
    }
};

// ==== Read<T> Template ====
template<typename T>
T Read(DWORD_PTR addr) noexcept {
    if (!addr) return T();
    __try {
        return *reinterpret_cast<volatile T*>(addr);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return T();
    }
}

// ==== GetModuleBase ====
inline DWORD_PTR GetModuleBase(const wchar_t* modName) {
    DWORD pid = GetCurrentProcessId();
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (snap == INVALID_HANDLE_VALUE) return 0;

    MODULEENTRY32 me = { sizeof(me) };
    if (Module32First(snap, &me)) {
        do {
            if (_wcsicmp(me.szModule, modName) == 0) {
                DWORD_PTR base = (DWORD_PTR)me.modBaseAddr;
                CloseHandle(snap);
                return base;
            }
        } while (Module32Next(snap, &me));
    }

    CloseHandle(snap);
    return 0;
}

// ==== MemoryManager ====
class MemoryManager {
public:
    DWORD_PTR clientBase = 0;
    DWORD_PTR entityList = 0;
    DWORD_PTR localPawn = 0;
    DWORD_PTR viewMatrix = 0;

    DWORD m_iHealth = 0x34C;
    DWORD m_iTeamNum = 0x3EB;
    DWORD m_vecOrigin = 0x15B0;

    void Init() {
        clientBase = (DWORD_PTR)GetModuleHandleA("client.dll");
        entityList = clientBase + 0x1E019A0;   // Replace with PatternScan
        localPawn = clientBase + 0x1AF4A20;
        viewMatrix = clientBase + 0x1D21800;
    }

    template<typename T>
    T Read(DWORD_PTR addr) {
        __try { return *(T*)addr; } __except (EXCEPTION_EXECUTE_HANDLER) { return T(); }
    }
};

inline MemoryManager g_Mem;
