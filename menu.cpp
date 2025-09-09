#include "menu.hpp"
#include "config.hpp"
#include "globals.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace Menu {
    bool bShowMenu = true;

    enum class Tab {
        Aimbot,
        ESP,
        Misc,
        Security
    };

    static Tab currentTab = Tab::Aimbot;

    void ApplyModernTheme() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        colors[ImGuiCol_WindowBg]         = ImVec4(0.07f, 0.09f, 0.15f, 1.00f);
        colors[ImGuiCol_ChildBg]          = ImVec4(0.10f, 0.12f, 0.20f, 1.00f);
        colors[ImGuiCol_Border]           = ImVec4(0.15f, 0.20f, 0.30f, 1.00f);
        colors[ImGuiCol_FrameBg]          = ImVec4(0.13f, 0.17f, 0.26f, 1.00f);
        colors[ImGuiCol_Button]           = ImVec4(0.15f, 0.50f, 0.55f, 1.00f);
        colors[ImGuiCol_ButtonHovered]    = ImVec4(0.20f, 0.60f, 0.65f, 1.00f);
        colors[ImGuiCol_ButtonActive]     = ImVec4(0.25f, 0.70f, 0.75f, 1.00f);
        colors[ImGuiCol_Header]           = ImVec4(0.15f, 0.40f, 0.50f, 1.00f);
        colors[ImGuiCol_HeaderHovered]    = ImVec4(0.20f, 0.50f, 0.60f, 1.00f);
        colors[ImGuiCol_HeaderActive]     = ImVec4(0.25f, 0.60f, 0.70f, 1.00f);
        colors[ImGuiCol_SliderGrab]       = ImVec4(0.30f, 0.55f, 0.70f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.40f, 0.65f, 0.80f, 1.00f);
        colors[ImGuiCol_Text]             = ImVec4(0.90f, 0.95f, 1.00f, 1.00f);

        style.WindowRounding = 12.0f;
        style.ChildRounding = 10.0f;
        style.FrameRounding = 6.0f;
        style.GrabRounding = 6.0f;
        style.ScrollbarRounding = 6.0f;
    }

    void DrawSidebar() {
        ImGui::BeginChild("Sidebar", ImVec2(120, 0), true);

        if (ImGui::Button("🎯 Aimbot", ImVec2(-1, 40))) currentTab = Tab::Aimbot;
        if (ImGui::Button("🧿 ESP", ImVec2(-1, 40)))    currentTab = Tab::ESP;
        if (ImGui::Button("⚙ Misc", ImVec2(-1, 40)))    currentTab = Tab::Misc;
        if (ImGui::Button("🔒 Security", ImVec2(-1, 40))) currentTab = Tab::Security;

        ImGui::EndChild();
    }

    void DrawHeader() {
        ImGui::TextColored(ImVec4(0.3f, 0.9f, 1.0f, 1.f), "Edinaz Solutions - Premium Build");
        ImGui::SameLine(ImGui::GetWindowWidth() - 130);
        ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "Status: Injected");
        ImGui::Separator();
    }

    void DrawAimbotTab() {
        ImGui::Text("Aimbot Settings");
        ImGui::BeginGroup();
        ImGui::Checkbox("Enable Aimbot", &Config::aimbot);
        ImGui::SliderFloat("FOV", &Config::aimbot_fov, 1.f, 30.f, "%.1f");
        ImGui::SliderFloat("Smoothing", &Config::aimbot_smooth, 1.f, 10.f, "%.1f");
        ImGui::EndGroup();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Triggerbot");
        ImGui::Checkbox("Enable Triggerbot", &Config::triggerbot);
        ImGui::SliderInt("Trigger Delay (ms)", &Config::trigger_delay, 0, 200);
    }

    void DrawESPTab() {
        ImGui::Text("ESP Settings");
        ImGui::Checkbox("Enable ESP", &Config::esp);
        ImGui::Checkbox("Box ESP", &Config::box_esp);
        ImGui::Checkbox("Health Bar", &Config::health_esp);
        ImGui::Checkbox("Name ESP", &Config::name_esp);
    }

    void DrawMiscTab() {
        ImGui::Text("Miscellaneous");
        ImGui::Checkbox("Bunnyhop", &Config::bunnyhop);
        ImGui::Checkbox("No Recoil", &Config::no_recoil);
    }

    void DrawSecurityTab() {
        ImGui::Text("Security Options");
        ImGui::Separator();

        ImGui::Checkbox("Hide From Debugger", &Config::hide_from_debugger);
        ImGui::SameLine(); ImGui::TextDisabled("(Bypass simple debugging)");

        ImGui::Checkbox("Anti-Screenshot", &Config::anti_screenshot);
        ImGui::SameLine(); ImGui::TextDisabled("(Block overlays from being captured)");

        ImGui::Checkbox("Obfuscate Strings", &Config::obfuscate_strings);
        ImGui::SameLine(); ImGui::TextDisabled("(Basic XOR encryption)");

        ImGui::Checkbox("Randomize Window Class", &Config::randomize_window_class);
        ImGui::SameLine(); ImGui::TextDisabled("(Prevents class name scans)");

        ImGui::Checkbox("Hide Threads", &Config::hide_threads);
        ImGui::SameLine(); ImGui::TextDisabled("(Removes cheat from thread lists)");

        ImGui::Checkbox("Use Manual Map", &Config::use_manual_map);
        ImGui::SameLine(); ImGui::TextDisabled("(Bypass LoadLibrary injection)");
    }

    void Render() {
        if (!bShowMenu) return;

        ApplyModernTheme();

        ImGui::SetNextWindowSize(ImVec2(650, 480), ImGuiCond_Once);
        ImGui::Begin("##MainWindow", &bShowMenu, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

        ImGui::Columns(2, nullptr, false);
        DrawSidebar();
        ImGui::NextColumn();

        ImGui::BeginChild("MainArea", ImVec2(0, 0), true);
        DrawHeader();

        switch (currentTab) {
            case Tab::Aimbot:   DrawAimbotTab(); break;
            case Tab::ESP:      DrawESPTab(); break;
            case Tab::Misc:     DrawMiscTab(); break;
            case Tab::Security: DrawSecurityTab(); break;
        }

        ImGui::EndChild();
        ImGui::End();
    }
}
