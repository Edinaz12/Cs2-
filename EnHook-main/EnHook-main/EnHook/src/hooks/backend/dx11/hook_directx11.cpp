#include "../../../backend.hpp"
#include "../../../console/console.hpp"

#ifdef ENABLE_BACKEND_DX11

#include <Windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <memory>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include "../../../dependencies/imgui/imgui_impl_dx11.h"
#include "../../../dependencies/imgui/imgui_impl_win32.h"
#include "../../../dependencies/minhook/MinHook.h"

#include "../../../utils/utils.hpp"
#include "../../hooks.hpp"
#include "../../../menu/menu.hpp"
#include "../../../Aimbot.hpp"
#include "../../../Triggerbot.hpp"
#include "../../../ESP.hpp"
#include "../../../EntityManager.hpp"
#include "../../../memory.hpp"
#include "../../../globals.hpp"
#include "../../../CheatMain.hpp"  // <--- falls du CheatMain.cpp erstellst

// DX11 Globals
static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static ID3D11RenderTargetView* g_pd3dRenderTarget = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;

// Vorwärtsdekl.
static void CleanupDeviceD3D11();
static void CleanupRenderTarget();
static void RenderImGui_DX11(IDXGISwapChain* pSwapChain);

// === DX11 Render Target erstellen ===
static void CreateRenderTarget(IDXGISwapChain* pSwapChain) {
    ID3D11Texture2D* pBackBuffer = NULL;
    pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (pBackBuffer) {
        DXGI_SWAP_CHAIN_DESC sd;
        pSwapChain->GetDesc(&sd);

        D3D11_RENDER_TARGET_VIEW_DESC desc = {};
        desc.Format = static_cast<DXGI_FORMAT>(Utils::GetCorrectDXGIFormat(sd.BufferDesc.Format));
        desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, &desc, &g_pd3dRenderTarget);
        pBackBuffer->Release();
    }
}

// === Cleanup ===
static void CleanupRenderTarget() {
    if (g_pd3dRenderTarget) {
        g_pd3dRenderTarget->Release();
        g_pd3dRenderTarget = NULL;
    }
}

static void CleanupDeviceD3D11() {
    CleanupRenderTarget();

    if (g_pSwapChain) {
        g_pSwapChain->Release();
        g_pSwapChain = NULL;
    }
    if (g_pd3dDevice) {
        g_pd3dDevice->Release();
        g_pd3dDevice = NULL;
    }
    if (g_pd3dDeviceContext) {
        g_pd3dDeviceContext->Release();
        g_pd3dDeviceContext = NULL;
    }
}

// === Main Hook ===
static void RenderImGui_DX11(IDXGISwapChain* pSwapChain) {
    if (!ImGui::GetIO().BackendRendererUserData) {
        if (SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(&g_pd3dDevice)))) {
            g_pd3dDevice->GetImmediateContext(&g_pd3dDeviceContext);
            ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
        }
    }

    if (!H::bShuttingDown) {
        if (!g_pd3dRenderTarget)
            CreateRenderTarget(pSwapChain);

        if (ImGui::GetCurrentContext() && g_pd3dRenderTarget) {
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            Menu::Render();     // GUI
            CheatMain();        // Dein Cheat (Aimbot, ESP, Triggerbot...)

            ImGui::Render();
            g_pd3dDeviceContext->OMSetRenderTargets(1, &g_pd3dRenderTarget, NULL);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }
    }
}

// === Hooked Functions ===
static std::add_pointer_t<HRESULT WINAPI(IDXGISwapChain*, UINT, UINT)> oPresent;
static HRESULT WINAPI hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    RenderImGui_DX11(pSwapChain);
    return oPresent(pSwapChain, SyncInterval, Flags);
}

// === Hook Start ===
namespace DX11 {
    void Hook(HWND hwnd) {
        LOG("[+] Initialisiere DX11 Hook...\n");

        // Dummy Device zum VTable bekommen
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.Windowed = TRUE;
        swapChainDesc.BufferCount = 2;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = hwnd;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
            NULL, 0, D3D11_SDK_VERSION, &swapChainDesc, &g_pSwapChain, &g_pd3dDevice, NULL, &g_pd3dDeviceContext);

        if (FAILED(hr) || !g_pSwapChain) {
            LOG("[!] D3D11 Device creation failed\n");
            return;
        }

        void** pVTable = *reinterpret_cast<void***>(g_pSwapChain);
        void* fnPresent = pVTable[8];

        CleanupDeviceD3D11();

        if (MH_CreateHook(fnPresent, &hkPresent, reinterpret_cast<void**>(&oPresent)) != MH_OK ||
            MH_EnableHook(fnPresent) != MH_OK) {
            LOG("[!] DX11 Hooking Present fehlgeschlagen!\n");
            return;
        }

        LOG("[+] DX11 Present Hook erfolgreich!\n");
    }

    void Unhook() {
        LOG("[~] DX11 unhook\n");

        if (ImGui::GetCurrentContext()) {
            if (ImGui::GetIO().BackendRendererUserData)
                ImGui_ImplDX11_Shutdown();

            if (ImGui::GetIO().BackendPlatformUserData)
                ImGui_ImplWin32_Shutdown();

            ImGui::DestroyContext();
        }

        CleanupDeviceD3D11();
    }
}

#else

// Fallback wenn DX11 nicht aktiviert ist
#include <Windows.h>
namespace DX11 {
    void Hook(HWND hwnd) { LOG("[!] DX11 Backend nicht aktiviert.\n"); }
    void Unhook() {}
}

#endif
