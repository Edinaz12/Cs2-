#include "menu.hpp"
#include "../config/config.hpp"  // falls du CheatConfig verwenden willst
#include "imgui.h"
#include <Windows.h>

namespace Menu {

    static const char* kHitboxItems[] = { "any", "head", "chest" };

    static int HitboxToIndex(const std::string& s) {
        if (s == "head") return 1;
        if (s == "chest") return 2;
        return 0;
    }

    void ApplyTheme() {
        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();

        style.WindowRounding = 8.0f;
        style.FrameRounding  = 5.0f;
        style.GrabRounding   = 5.0f;
        style.ChildRounding  = 8.0f;

        ImVec4* colors = style.Colors;
        colors[ImGuiCol_TitleBg]       = ImVec4(0.09f, 0.09f, 0.15f, 1.0f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.35f, 1.0f);
        colors[ImGuiCol_FrameBg]       = ImVec4(0.15f, 0.15f, 0.20f, 1.0f);
        colors[ImGuiCol_Button]        = ImVec4(0.20f, 0.20f, 0.35f, 1.0f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.50f, 1.0f);
        colors[ImGuiCol_ButtonActive]  = ImVec4(0.40f, 0.40f, 0.70f, 1.0f);
    }

    void Render() {
        if (GetAsyncKeyState(VK_INSERT) & 1)
            bShowMenu = !bShowMenu;

        if (!bShowMenu) return;

        ApplyTheme();

        // Fenster zentrieren
        ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2),
                                ImGuiCond_Once, ImVec2(0.5f, 0.5f));

        ImGui::Begin("Edinaz Solutions", &bShowMenu, ImGuiWindowFlags_AlwaysAutoResize);

        if (ImGui::BeginTabBar("Tabs")) {
            if (ImGui::BeginTabItem("Visuals")) {
                ImGui::Checkbox("ESP Box", &CheatConfig::espBox);
                ImGui::Checkbox("Health Bar", &CheatConfig::healthBar);
                ImGui::Checkbox("Names", &CheatConfig::drawNames);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Aimbot")) {
                ImGui::Checkbox("Enable Aimbot", &CheatConfig::aimbotEnabled);
                ImGui::SliderFloat("FOV", &CheatConfig::aimbotFov, 0.0f, 30.0f, "%.1f");
                ImGui::SliderFloat("Smoothing", &CheatConfig::aimbotSmoothing, 1.0f, 20.0f, "%.1f");
                ImGui::Combo("Hitbox", &CheatConfig::aimbotHitboxIndex, kHitboxItems, IM_ARRAYSIZE(kHitboxItems));
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Triggerbot")) {
                ImGui::Checkbox("Enable Triggerbot", &CheatConfig::triggerbotEnabled);
                ImGui::SliderInt("Delay (ms)", &CheatConfig::triggerDelay, 0, 100);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Config")) {
                if (ImGui::Button("Save Config"))
                    CheatConfig::Save();
                if (ImGui::Button("Load Config"))
                    CheatConfig::Load();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }

}
