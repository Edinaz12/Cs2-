#include "menu.hpp"
#include <Windows.h>
#include "imgui.h"

// adjust include path to your config header
#include "../config/config.hpp"   // CheatConfig

namespace Menu {

    // helper: map config string <-> combo index
    static int HitboxToIndex(const std::string& s) {
        if (s == "head")  return 1;
        if (s == "chest") return 2;
        return 0; // "any"
    }
    static const char* kHitboxItems[] = { "any", "head", "chest" };

    void Render() {
        // INSERT toggles the menu
        static bool menuOpen = true;
        if (GetAsyncKeyState(VK_INSERT) & 1)
            menuOpen = !menuOpen;

        if (!menuOpen) return;

        ImGui::Begin("Edinaz Solutions", &menuOpen, ImGuiWindowFlags_AlwaysAutoResize);

        // ---------------- Visuals ----------------
        ImGui::Text("Visuals");
        ImGui::Separator();

        bool esp = CheatConfig::Get().Get<bool>("esp", true);
        if (ImGui::Checkbox("ESP", &esp)) {
            CheatConfig::Get().Set("esp", esp);
        }

        // optional detail toggles used by your ESP
        bool hpBar = CheatConfig::Get().Get<bool>("hp_enabled", true);
        if (ImGui::Checkbox("Health Bar", &hpBar)) {
            CheatConfig::Get().Set("hp_enabled", hpBar);
        }

        bool distText = CheatConfig::Get().Get<bool>("distance", true);
        if (ImGui::Checkbox("Distance Text", &distText)) {
            CheatConfig::Get().Set("distance", distText);
        }

        bool headEsp = CheatConfig::Get().Get<bool>("head_enabled", true);
        if (ImGui::Checkbox("Head ESP", &headEsp)) {
            CheatConfig::Get().Set("head_enabled", headEsp);
        }

        bool rcs = CheatConfig::Get().Get<bool>("rcs", true);
        if (ImGui::Checkbox("RCS", &rcs)) {
            CheatConfig::Get().Set("rcs", rcs);
        }

        ImGui::Spacing();

        // ---------------- Aimbot ----------------
        ImGui::Text("Aimbot");
        ImGui::Separator();

        bool aimbot = CheatConfig::Get().Get<bool>("aimbot", true);
        if (ImGui::Checkbox("Enable Aimbot", &aimbot)) {
            CheatConfig::Get().Set("aimbot", aimbot);
        }

        float fov = CheatConfig::Get().Get<float>("fov", 10.0f);
        if (ImGui::SliderFloat("FOV", &fov, 1.0f, 50.0f, "%.1f")) {
            CheatConfig::Get().Set("fov", fov);
        }

        float smoothing = CheatConfig::Get().Get<float>("smoothing", 3.0f);
        if (ImGui::SliderFloat("Smoothing", &smoothing, 1.0f, 20.0f, "%.1f")) {
            CheatConfig::Get().Set("smoothing", smoothing);
        }

        ImGui::Spacing();

        // ---------------- Triggerbot ----------------
        ImGui::Text("Triggerbot");
        ImGui::Separator();

        bool trigger = CheatConfig::Get().Get<bool>("triggerbot", true);
        if (ImGui::Checkbox("Enable Triggerbot", &trigger)) {
            CheatConfig::Get().Set("triggerbot", trigger);
        }

        int delayMs = CheatConfig::Get().Get<int>("trigger_delay", 10);
        if (ImGui::SliderInt("Delay (ms)", &delayMs, 0, 150)) {
            CheatConfig::Get().Set("trigger_delay", delayMs);
        }

        // map string <-> int for combo
        std::string hitboxStr = CheatConfig::Get().Get<std::string>("trigger_hitbox", "any");
        int hitboxIdx = HitboxToIndex(hitboxStr);
        if (ImGui::Combo("Hitbox", &hitboxIdx, kHitboxItems, IM_ARRAYSIZE(kHitboxItems))) {
            CheatConfig::Get().Set("trigger_hitbox", std::string(kHitboxItems[hitboxIdx]));
        }

        ImGui::End();
    }

} // namespace Menu
