#include "Injector.hpp"
#include "Utils.hpp"
#include <Windows.h>
#include <string>
#include <thread>
#include <iostream>
#include <tchar.h>

// OPTIONAL: Wenn du eine WinAPI GUI willst
#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")

// Auth Endpoints
const char* AUTH_API_PRIMARY = "https://edinaz.solutions/api/verify";
const char* AUTH_API_SECONDARY = "https://2cbcxta8k1.execute-api.eu-central-1.amazonaws.com/prod/api/verify";

// Globals
HWND hButton, hStatus;
HINSTANCE hInst;
std::string dllPath = "EdinazCS2.dll"; // oder beliebiger DLL-Name

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void UpdateStatus(const std::string& msg) {
    SetWindowTextA(hStatus, msg.c_str());
}

void InjectRoutine() {
    UpdateStatus("Authentifiziere...");
    if (!Utils::VerifyAuth(AUTH_API_PRIMARY) && !Utils::VerifyAuth(AUTH_API_SECONDARY)) {
        UpdateStatus("Auth fehlgeschlagen!");
        return;
    }

    UpdateStatus("Warte auf CS2...");
    DWORD pid = Utils::FindProcessId("cs2.exe");
    if (!pid) {
        UpdateStatus("CS2 nicht gefunden!");
        return;
    }

    UpdateStatus("Injektion läuft...");
    if (Injector::ManualMap(pid, dllPath)) {
        UpdateStatus("✅ Erfolgreich injiziert!");
    } else {
        UpdateStatus("❌ Injektion fehlgeschlagen!");
    }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;

    // Fensterklasse
    WNDCLASS wc = { };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = _T("EdinazLoader");
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, _T("Edinaz Solutions Loader"), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200, NULL, NULL, hInstance, NULL);

    hButton = CreateWindow(TEXT("button"), TEXT("Inject"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        90, 80, 100, 30, hwnd, (HMENU)1, hInstance, NULL);

    hStatus = CreateWindow(TEXT("static"), TEXT("Status: bereit"), WS_VISIBLE | WS_CHILD,
        30, 130, 240, 20, hwnd, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            std::thread(InjectRoutine).detach();
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
