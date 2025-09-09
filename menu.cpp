#include "menu.hpp"
#include "../globals.hpp"
#include "../config/CheatConfig.hpp"  // Optional: falls du config speichern willst
#include "imgui/imgui.h"
#include <Windows.h>

namespace Menu {

    void Render() {
        // Menü toggeln mit INSERT
        static bool prevInsert = false;
        bool curInsert = GetAsyncKeyState(VK_INSERT) & 1;
        if (curInsert && !prevInsert) {
            bShowMenu = !bShowMenu;
        }
        prevInsert = curInsert;

        if (!bShowMenu)
            return;

        ImGui::Begin("Edinaz Solutions", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("Features");
        ImGui::Separator();

        bool esp = CheatConfig::Get().Get<bool>("esp", true);
        if (ImGui::Checkbox("ESP", &esp)) {
            CheatConfig::Get().Set("esp", esp);
        }

        bool aimbot = CheatConfig::Get().Get<bool>("aimbot", true);
        if (ImGui::Checkbox("Aimbot", &aimbot)) {
            CheatConfig::Get().Set("aimbot", aimbot);
        }

        bool triggerbot = CheatConfig::Get().Get<bool>("triggerbot", true);
        if (ImGui::Checkbox("Triggerbot", &triggerbot)) {
            CheatConfig::Get().Set("triggerbot", triggerbot);
        }

        bool rcs = CheatConfig::Get().Get<bool>("rcs", true);
        if (ImGui::Checkbox("RCS", &rcs)) {
            CheatConfig::Get().Set("rcs", rcs);
        }

        ImGui::End();
    }

}
