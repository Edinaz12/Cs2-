#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "PELoader.hpp"
#include "Utils.hpp"

int main() {
    // Schritt 1: Prozess-ID von CS2 holen
    DWORD pid = Utils::FindProcessId(L"cs2.exe");
    if (!pid) {
        std::cerr << "[Fehler] CS2.exe nicht gefunden.\n";
        system("pause");
        return 1;
    }

    // Schritt 2: Prozess öffnen
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProc || hProc == INVALID_HANDLE_VALUE) {
        std::cerr << "[Fehler] OpenProcess fehlgeschlagen.\n";
        return 1;
    }

    // Schritt 3: DLL lesen
    std::vector<BYTE> dllData;
    if (!Utils::ReadFileBytes("EdinazInternal.dll", dllData)) {
        std::cerr << "[Fehler] DLL konnte nicht gelesen werden.\n";
        return 1;
    }

    // Schritt 4: Manual Map starten
    LPVOID remoteBase = nullptr;
    std::string error;
    if (!PELoader::ManualMap(hProc, dllData, remoteBase, error)) {
        std::cerr << "[ManualMap Fehler] " << error << "\n";
        return 1;
    }

    std::cout << "[+] DLL erfolgreich gemappt bei: 0x" << remoteBase << "\n";
    CloseHandle(hProc);
    system("pause");
    return 0;
}
