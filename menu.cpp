#include "menu.hpp"
#include "../config/config.hpp"  // CheatConfig
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

        style.FrameRounding = 5.0f;
        style.GrabRounding  = 5.0f;
        style.WindowRounding = 8.0f;
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
        // Insert-Toggle
        static bool menuOpen = true;
        if (GetAsyncKeyState(VK_INSERT) & 1)
            menuOpen = !menuOpen;
        if (!menuOpen) return;

        // Dark Theme setzen
        ApplyTheme();

        ImGui::Begin("Edinaz Solutions", &menuOpen, ImGuiWindowFlags_AlwaysAutoResize);

        // ==== Visuals ====
        ImGui::Text("Visuals");
        ImGui::Separator();
        bool esp = CheatConfig::Get().Get<bool>("esp", true);
        if (ImGui::Checkbox("ESP", &esp)) CheatConfig::Get().Set("esp", esp);

        bool hpBar = CheatConfig::Get().Get<bool>("hp_enabled", true);
        if (ImGui::Checkbox("Health Bar", &hpBar)) CheatConfig::Get().Set("hp_enabled", hpBar);

        bool distText = CheatConfig::Get().Get<bool>("distance", true);
        if (ImGui::Checkbox("Distance Text", &distText)) CheatConfig::Get().Set("distance", distText);

        bool headEsp = CheatConfig::Get().Get<bool>("Head ESP", true);
        if (ImGui::Checkbox("Head ESP", &headEsp)) CheatConfig::Get().Set("head_enabled", headEsp);

        bool rcs = CheatConfig::Get().Get<bool>("rcs", true);
        if (ImGui::Checkbox("RCS", &rcs)) CheatConfig::Get().Set("rcs", rcs);

        ImGui::Spacing();

        // ==== Aimbot ====
        ImGui::Text("Aimbot");
        ImGui::Separator();
        bool aimbot = CheatConfig::Get().Get<bool>("aimbot", true);
        if (ImGui::Checkbox("Enable Aimbot", &aimbot)) CheatConfig::Get().Set("aimbot", aimbot);

        float fov = CheatConfig::Get().Get<float>("fov", 10.0f);
        if (ImGui::SliderFloat("FOV", &fov, 1.0f, 50.0f, "%.1f")) CheatConfig::Get().Set("fov", fov);

        float smoothing = CheatConfig::Get().Get<float>("smoothing", 3.0f);
        if (ImGui::SliderFloat("Smoothing", &smoothing, 1.0f, 20.0f, "%.1f"))
            CheatConfig::Get().Set("smoothing", smoothing);

        ImGui::Spacing();

        // ==== Triggerbot ====
        ImGui::Text("Triggerbot");
        ImGui::Separator();
        bool trigger = CheatConfig::Get().Get<bool>("triggerbot", true);
        if (ImGui::Checkbox("Enable Triggerbot", &trigger)) CheatConfig::Get().Set("triggerbot", trigger);

        int delayMs = CheatConfig::Get().Get<int>("trigger_delay", 10);
        if (ImGui::SliderInt("Delay (ms)", &delayMs, 0, 150)) CheatConfig::Get().Set("trigger_delay", delayMs);

        std::string hitboxStr = CheatConfig::Get().Get<std::string>("trigger_hitbox", "any");
        int hitboxIdx = HitboxToIndex(hitboxStr);
        if (ImGui::Combo("Hitbox", &hitboxIdx, kHitboxItems, IM_ARRAYSIZE(kHitboxItems))) {
            CheatConfig::Get().Set("trigger_hitbox", std::string(kHitboxItems[hitboxIdx]));
        }

        ImGui::Spacing();
        ImGui::Separator();

        // ==== Config Save/Load ====
        if (ImGui::Button("💾 Save Config")) {
            CheatConfig::Get().Save("edinaz.json");
        }
        ImGui::SameLine();
        if (ImGui::Button("📂 Load Config")) {
            CheatConfig::Get().Load("edinaz.json");
        }

        ImGui::End();
    }
}
