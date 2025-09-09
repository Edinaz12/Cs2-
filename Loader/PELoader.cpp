#include "PELoader.hpp"
#include <winnt.h>
#include <tlhelp32.h>

bool PELoader::ManualMap(HANDLE hProc, const std::vector<BYTE>& rawData, LPVOID& remoteBase, std::string& error) {
    if (rawData.empty()) {
        error = "Leere DLL-Daten";
        return false;
    }

    auto dosHeader = reinterpret_cast<const IMAGE_DOS_HEADER*>(rawData.data());
    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
        error = "Ungültiges DOS-Header-Signatur";
        return false;
    }

    auto ntHeader = reinterpret_cast<const IMAGE_NT_HEADERS*>(rawData.data() + dosHeader->e_lfanew);
    if (ntHeader->Signature != IMAGE_NT_SIGNATURE) {
        error = "Ungültige NT-Signatur";
        return false;
    }

    SIZE_T imageSize = ntHeader->OptionalHeader.SizeOfImage;
    LPVOID remoteImage = VirtualAllocEx(hProc, (LPVOID)(ntHeader->OptionalHeader.ImageBase), imageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!remoteImage) {
        remoteImage = VirtualAllocEx(hProc, nullptr, imageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (!remoteImage) {
            error = "VirtualAllocEx fehlgeschlagen";
            return false;
        }
    }

    // Copy Headers
    WriteProcessMemory(hProc, remoteImage, rawData.data(), ntHeader->OptionalHeader.SizeOfHeaders, nullptr);

    // Copy Sections
    auto sections = IMAGE_FIRST_SECTION(ntHeader);
    for (int i = 0; i < ntHeader->FileHeader.NumberOfSections; ++i) {
        LPVOID localSectionAddr = (LPVOID)(rawData.data() + sections[i].PointerToRawData);
        LPVOID remoteSectionAddr = (LPBYTE)remoteImage + sections[i].VirtualAddress;
        WriteProcessMemory(hProc, remoteSectionAddr, localSectionAddr, sections[i].SizeOfRawData, nullptr);
    }

    // Optional: Relocations und Imports behandeln
    // === Relocations bearbeiten ===
if (ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size) {
    DWORD delta = (DWORD)((uintptr_t)remoteImage - ntHeader->OptionalHeader.ImageBase);
    if (delta) {
        auto relocDir = (IMAGE_BASE_RELOCATION*)(rawData.data() +
            ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);

        while (relocDir->VirtualAddress) {
            DWORD entries = (relocDir->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
            WORD* data = (WORD*)(relocDir + 1);

            for (DWORD i = 0; i < entries; ++i) {
                if ((data[i] >> 12) == IMAGE_REL_BASED_HIGHLOW || (data[i] >> 12) == IMAGE_REL_BASED_DIR64) {
                    DWORD rva = relocDir->VirtualAddress + (data[i] & 0xFFF);
                    uintptr_t patchAddr = (uintptr_t)(rawData.data() + rva);
                    uintptr_t patchedValue = 0;

                    if ((data[i] >> 12) == IMAGE_REL_BASED_HIGHLOW) {
                        patchedValue = *(DWORD*)patchAddr + delta;
                        *(DWORD*)patchAddr = patchedValue;
                    }
                    else if ((data[i] >> 12) == IMAGE_REL_BASED_DIR64) {
                        patchedValue = *(uintptr_t*)patchAddr + delta;
                        *(uintptr_t*)patchAddr = patchedValue;
                    }
                }
            }

            relocDir = (IMAGE_BASE_RELOCATION*)((BYTE*)relocDir + relocDir->SizeOfBlock);
        }
    }
}


    remoteBase = remoteImage;
    return true;
}
