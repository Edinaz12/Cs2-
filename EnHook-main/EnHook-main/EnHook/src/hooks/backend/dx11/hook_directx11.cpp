#include "../../../backend.hpp"
#include "../../../console/console.hpp"

#ifdef ENABLE_BACKEND_DX11

#include <Windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <wrl/client.h>
#include <memory>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include "../../../dependencies/imgui/imgui_impl_dx11.h"
#include "../../../dependencies/imgui/imgui_impl_win32.h"
#include "../../../dependencies/minhook/MinHook.h"

#include "../../../utils/utils.hpp"
#include "../../hooks.hpp"
#include "../../../menu/menu.hpp"
#include "../../../CheatMain.hpp"

using Microsoft::WRL::ComPtr;

// DX11 State
static ComPtr<ID3D11Device>            g_pd3dDevice;
static ComPtr<ID3D11DeviceContext>     g_pd3dDeviceContext;
static ComPtr<IDXGISwapChain>          g_pSwapChain;
static ComPtr<ID3D11RenderTargetView>  g_pRenderTarget;

static bool g_ImGuiInitialized = false;

// Forward decl
static void CleanupDeviceD3D11();
static void CleanupRenderTarget();
static void CreateRenderTarget(IDXGISwapChain* pSwapChain);
static void RenderImGui_DX11(IDXGISwapChain* pSwapChain);

// === Render Target erstellen ===
static void CreateRenderTarget(IDXGISwapChain* pSwapChain) {
    ComPtr<ID3D11Texture2D> pBackBuffer;
    if (SUCCEEDED(pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)))) {
        DXGI_SWAP_CHAIN_DESC sd;
        pSwapChain->GetDesc(&sd);

        D3D11_RENDER_TARGET_VIEW_DESC desc = {};
        desc.Format = static_cast<DXGI_FORMAT>(Utils::GetCorrectDXGIFormat(sd.BufferDesc.Format));
        desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

        if (FAILED(g_pd3dDevice->CreateRenderTargetView(pBackBuffer.Get(), &desc, &g_pRenderTarget))) {
            LOG("[!] Failed to create render target view.\n");
        }
    }
}

static void CleanupRenderTarget() {
    g_pRenderTarget.Reset();
}

static void CleanupDeviceD3D11() {
    CleanupRenderTarget();
    g_pSwapChain.Reset();
    g_pd3dDevice.Reset();
    g_pd3dDeviceContext.Reset();
    g_ImGuiInitialized = false;
}

// === Main ImGui Render ===
static void RenderImGui_DX11(IDXGISwapChain* pSwapChain) {
    if (!g_ImGuiInitialized) {
        if (SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(&g_pd3dDevice)))) {
            g_pd3dDevice->GetImmediateContext(&g_pd3dDeviceContext);
            ImGui::CreateContext();
            ImGui_ImplDX11_Init(g_pd3dDevice.Get(), g_pd3dDeviceContext.Get());
            g_ImGuiInitialized = true;
        }
    }

    if (!H::bShuttingDown) {
        if (!g_pRenderTarget) {
            CreateRenderTarget(pSwapChain);
        }

        if (ImGui::GetCurrentContext() && g_pRenderTarget) {
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            // Dein UI + Cheat-Logik
            Menu::Render();
            CheatMain();

            ImGui::Render();
            g_pd3dDeviceContext->OMSetRenderTargets(1, g_pRenderTarget.GetAddressOf(), nullptr);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }
    }
}

// === Hooked Present ===
static std::add_pointer_t<HRESULT WINAPI(IDXGISwapChain*, UINT, UINT)> oPresent;
static HRESULT WINAPI hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    RenderImGui_DX11(pSwapChain);
    return oPresent(pSwapChain, SyncInterval, Flags);
}

// === Hook Start ===
namespace DX11 {
    void Hook(HWND hwnd) {
        LOG("[+] Initialisiere DX11 Hook...\n");

        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 2;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hwnd;
        sd.SampleDesc.Count = 1;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        ComPtr<ID3D11Device> pDevice;
        ComPtr<ID3D11DeviceContext> pContext;
        ComPtr<IDXGISwapChain> pSwap;

        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
            nullptr, 0, D3D11_SDK_VERSION,
            &sd, &pSwap, &pDevice, nullptr, &pContext
        );

        if (FAILED(hr) || !pSwap) {
            LOG("[!] D3D11 device creation failed.\n");
            return;
        }

        void** pVTable = *reinterpret_cast<void***>(pSwap.Get());
        void* fnPresent = pVTable[8];

        if (MH_CreateHook(fnPresent, &hkPresent, reinterpret_cast<void**>(&oPresent)) != MH_OK ||
            MH_EnableHook(fnPresent) != MH_OK) {
            LOG("[!] Failed to hook DX11 Present.\n");
            return;
        }

        LOG("[+] DX11 Present Hook erfolgreich!\n");
    }

    void Unhook() {
        LOG("[~] DX11 Unhook...\n");

        if (ImGui::GetCurrentContext()) {
            ImGui_ImplDX11_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();
        }

        CleanupDeviceD3D11();
    }
}

#else

// Fallback
#include <Windows.h>
namespace DX11 {
    void Hook(HWND) { LOG("[!] DX11 Backend nicht aktiviert.\n"); }
    void Unhook() {}
}

#endif
