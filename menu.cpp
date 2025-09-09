#include "menu.hpp"
#include "../config/CheatConfig.hpp"
#include "imgui/imgui.h"
#include <Windows.h>

namespace Menu {

    void Render() {
        // INSERT-Toggle
        static bool prevInsert = false;
        bool curInsert = GetAsyncKeyState(VK_INSERT) & 1;
        if (curInsert && !prevInsert)
            bShowMenu = !bShowMenu;
        prevInsert = curInsert;

        if (!bShowMenu)
            return;

        ImGui::Begin("Edinaz Solutions", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("Visuals");
        ImGui::Separator();

        bool esp = CheatConfig::Get().Get<bool>("esp", true);
        if (ImGui::Checkbox("ESP", &esp)) {
            CheatConfig::Get().Set("esp", esp);
        }

        bool hp = CheatConfig::Get().Get<bool>("hp_enabled", true);
        if (ImGui::Checkbox("Health Bar", &hp)) {
            CheatConfig::Get().Set("hp_enabled", hp);
        }

        bool dist = CheatConfig::Get().Get<bool>("distance", true);
        if (ImGui::Checkbox("Distance Text", &dist)) {
            CheatConfig::Get().Set("distance", dist);
        }

        bool head = CheatConfig::Get().Get<bool>("head_enabled", true);
        if (ImGui::Checkbox("Head ESP", &head)) {
            CheatConfig::Get().Set("head_enabled", head);
        }

        ImGui::Text("Combat");
        ImGui::Separator();

        bool aimbot = CheatConfig::Get().Get<bool>("aimbot", true);
        if (ImGui::Checkbox("Aimbot", &aimbot)) {
            CheatConfig::Get().Set("aimbot", aimbot);
        }

        float fov = CheatConfig::Get().Get<float>("fov", 10.0f);
        if (ImGui::SliderFloat("Aimbot FOV", &fov, 1.0f, 50.0f)) {
            CheatConfig::Get().Set("fov", fov);
        }

        float smoothing = CheatConfig::Get().Get<float>("smoothing", 3.0f);
        if (ImGui::SliderFloat("Smoothing", &smoothing, 1.0f, 20.0f)) {
            CheatConfig::Get().Set("smoothing", smoothing);
        }

        bool trigger = CheatConfig::Get().Get<bool>("triggerbot", true);
        if (ImGui::Checkbox("Triggerbot", &trigger)) {
            CheatConfig::Get().Set("triggerbot", trigger);
        }

        int delay = CheatConfig::Get().Get<int>("trigger_delay", 10);
        if (ImGui::SliderInt("Trigger Delay", &delay, 0, 100)) {
            CheatConfig::Get().Set("trigger_delay", delay);
        }

        bool rcs = CheatConfig::Get().Get<bool>("rcs", true);
        if (ImGui::Checkbox("RCS", &rcs)) {
            CheatConfig::Get().Set("rcs", rcs);
        }

        ImGui::End();
    }

}
