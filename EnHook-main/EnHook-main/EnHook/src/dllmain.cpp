#include <Windows.h>
#include <iostream>
#include <thread>

#include "console/console.hpp"

#include "hooks/hooks.hpp"
#include "utils/utils.hpp"

#include "dependencies/minhook/MinHook.h"

#include <tchar.h>

#define ALLOC_CONSOLE 1

CONST TCHAR* DESIRED_PROCESS_NAME = _T("League of Legends.exe");

DWORD WINAPI OnProcessAttach(LPVOID lpParam);
DWORD WINAPI OnProcessDetach(LPVOID lpParam);

/**
 * \brief Checks if the DLL is loaded into the correct process.
 * \return True if the DLL is loaded into the correct process, false otherwise.
 */
BOOL IsLoadedIntoCorrectProcess( ) {
    TCHAR szFileName[MAX_PATH] = {0};
    GetModuleFileName(NULL, szFileName, MAX_PATH);
    TCHAR* szShortName = _tcsrchr(szFileName, '\\');

    if (szShortName) {
        szShortName++; // Move past the '\'
    }

    if (szShortName && _tcsicmp(szShortName, _T(DESIRED_PROCESS_NAME)) != 0) {
        return FALSE;
    }
}

VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    OnProcessAttach((HINSTANCE)idEvent);
    KillTimer(NULL, idEvent); // Kill the timer since it's a one-shot timer
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    DisableThreadLibraryCalls(hinstDLL);

    U::SetRenderingBackend(DIRECTX11);

    if (fdwReason == DLL_PROCESS_ATTACH) {
        SetTimer(NULL, (UINT_PTR)hinstDLL, 8000, TimerProc); // 8-second delay

    } else if (fdwReason == DLL_PROCESS_DETACH && !lpReserved) {
        OnProcessDetach(hinstDLL);
    }

    return TRUE;
}

/**
 * \brief This function is being called upon Process Attach.
 * \param lpParam HInstance of the DLL, passed by DLLMain. 
 * \return 0 if successful, 1 otherwise.
 */
DWORD WINAPI OnProcessAttach(LPVOID lpParam) {
    Beep(200, 200);

#if ALLOC_CONSOLE == 1
    Console::Alloc( );
#endif
    LOG("[+] Rendering backend: %s\n", U::RenderingBackendToStr( ));
    if (U::GetRenderingBackend( ) == NONE) {
        LOG("[!] Looks like you forgot to set a backend. Will unload after pressing enter...");
        std::cin.get( );

        FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(lpParam), 0);
    }

    MH_Initialize( );
    H::Init( );

    return 0;
}

/**
 * \brief This function is being called upon Process Detach and frees all resources.
 * \param lpParam HInstance of the DLL, passed by DLLMain. 
 * \return 0 if successful, 1 otherwise.
 */
DWORD WINAPI OnProcessDetach(LPVOID lpParam) {
    H::Free( );
    MH_Uninitialize( );

    Console::Free( );

    return 0;
}
