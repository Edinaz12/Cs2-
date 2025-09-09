#include <Windows.h>
#include "Injector.hpp"
#include "Utils.hpp"

const char* targetProcess = "cs2.exe";     // Zielprozess
const char* dllToInject   = "cs2_cheat.dll"; // DLL-Name (im gleichen Ordner)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND hStatus;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "EdinazLoader";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, "Edinaz Solutions - Loader",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, NULL, NULL, hInstance, NULL);

    CreateWindow("BUTTON", "Inject", WS_VISIBLE | WS_CHILD, 150, 80, 100, 30, hwnd, (HMENU)1, hInstance, NULL);
    hStatus = CreateWindow("STATIC", "Warte auf Inject...", WS_VISIBLE | WS_CHILD, 100, 130, 200, 20, hwnd, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
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
            SetWindowText(hStatus, "Injecting...");
            if (Injector::InjectDLL(targetProcess, dllToInject)) {
                SetWindowText(hStatus, "Injection erfolgreich!");
            }
            else {
                SetWindowText(hStatus, "Injection fehlgeschlagen.");
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
