#include "menu.hpp"
#include "../config/config.hpp"  // CheatConfig

#include "imgui.h"

namespace Menu {
    void Render() {
        static bool open = true;
        if (!open) return;

        ImGui::Begin("Edinaz Solutions", &open, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("Visuals");
        ImGui::Separator();
        ImGui::Checkbox("ESP", CheatConfig::Get().GetPtr<bool>("esp"));
        ImGui::Checkbox("RCS", CheatConfig::Get().GetPtr<bool>("rcs"));

        ImGui::Spacing();
        ImGui::Text("Aimbot");
        ImGui::Separator();
        ImGui::Checkbox("Enable Aimbot", CheatConfig::Get().GetPtr<bool>("aimbot"));
        ImGui::SliderFloat("FOV", CheatConfig::Get().GetPtr<float>("fov"), 1.0f, 50.0f, "%.1f");
        ImGui::SliderFloat("Smoothing", CheatConfig::Get().GetPtr<float>("smoothing"), 1.0f, 20.0f, "%.1f");

        ImGui::Spacing();
        ImGui::Text("Triggerbot");
        ImGui::Separator();
        ImGui::Checkbox("Enable Triggerbot", CheatConfig::Get().GetPtr<bool>("triggerbot"));
        ImGui::SliderInt("Delay (ms)", CheatConfig::Get().GetPtr<int>("trigger_delay"), 1, 100);
        ImGui::Combo("Hitbox", CheatConfig::Get().GetPtr<std::string>("trigger_hitbox"),
            [](void* data, int idx, const char** out_text) {
                static const char* items[] = { "any", "head", "chest" };
                if (idx < 0 || idx >= 3) return false;
                *out_text = items[idx];
                return true;
            }, nullptr, 3);

        ImGui::End();
    }
}
