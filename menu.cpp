#include "menu.hpp"
#include "config.hpp"
#include "globals.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <algorithm>
#include <string>
#include <cmath>

namespace Menu {
    bool bShowMenu = true;

    enum class Tab {
        Aimbot,
        Visuals,
        Misc
    };
    static Tab currentTab = Tab::Aimbot;

    // Animation state
    float tabAlpha[3] = {1.f, 0.f, 0.f}; // Only Aimbot active at start

    // Modern Theme
    void ApplyModernTheme() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        colors[ImGuiCol_WindowBg]         = ImVec4(0.10f, 0.12f, 0.18f, 1.00f);
        colors[ImGuiCol_ChildBg]          = ImVec4(0.13f, 0.14f, 0.21f, 1.00f);
        colors[ImGuiCol_Border]           = ImVec4(0.20f, 0.24f, 0.34f, 1.00f);
        colors[ImGuiCol_FrameBg]          = ImVec4(0.18f, 0.21f, 0.28f, 1.00f);
        colors[ImGuiCol_Button]           = ImVec4(0.22f, 0.55f, 0.60f, 1.00f);
        colors[ImGuiCol_ButtonHovered]    = ImVec4(0.26f, 0.65f, 0.75f, 1.00f);
        colors[ImGuiCol_ButtonActive]     = ImVec4(0.30f, 0.75f, 0.80f, 1.00f);
        colors[ImGuiCol_Header]           = ImVec4(0.22f, 0.45f, 0.55f, 1.00f);
        colors[ImGuiCol_HeaderHovered]    = ImVec4(0.26f, 0.55f, 0.65f, 1.00f);
        colors[ImGuiCol_HeaderActive]     = ImVec4(0.30f, 0.65f, 0.75f, 1.00f);
        colors[ImGuiCol_SliderGrab]       = ImVec4(0.33f, 0.57f, 0.75f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.43f, 0.67f, 0.85f, 1.00f);
        colors[ImGuiCol_Text]             = ImVec4(0.96f, 0.98f, 1.00f, 1.00f);

        style.WindowRounding = 18.0f;
        style.ChildRounding = 14.0f;
        style.FrameRounding = 10.0f;
        style.GrabRounding = 10.0f;
        style.ScrollbarRounding = 10.0f;
    }

    void AnimateTabs() {
        float target[3] = {0.f, 0.f, 0.f};
        target[(int)currentTab] = 1.f;
        float speed = 0.12f;
        for (int i = 0; i < 3; ++i)
            tabAlpha[i] += (target[i] - tabAlpha[i]) * speed;
    }

    void DrawSidebar() {
        ImGui::BeginChild("Sidebar", ImVec2(140, 0), true, ImGuiWindowFlags_NoScrollbar);

        struct SideTab { Tab tab; const char* label; const char* icon; int idx; };
        SideTab tabs[] = {
            {Tab::Aimbot, "Aimbot", "🎯", 0},
            {Tab::Visuals, "Visuals", "🧿", 1},
            {Tab::Misc, "Misc", "⚙", 2}
        };
        AnimateTabs();

        for (auto& t : tabs) {
            float highlight = tabAlpha[t.idx];
            ImVec4 activeCol = ImVec4(0.30f, 0.70f, 0.90f, 0.15f + 0.65f * highlight);
            ImVec4 textCol = ImVec4(0.96f, 0.98f, 1.0f, 0.65f + 0.35f * highlight);

            ImGui::PushStyleColor(ImGuiCol_Button, activeCol);
            ImGui::PushStyleColor(ImGuiCol_Text, textCol);

            std::string btnTxt = std::string(t.icon) + " " + t.label;
            if (ImGui::Button(btnTxt.c_str(), ImVec2(-1, 44))) {
                currentTab = t.tab;
            }
            ImGui::PopStyleColor(2);

            // Animation: Draw left indicator bar for selected tab
            if (highlight > 0.05f) {
                ImVec2 pMin = ImGui::GetItemRectMin();
                ImVec2 pMax = ImGui::GetItemRectMax();
                ImGui::GetWindowDrawList()->AddRectFilled(
                    ImVec2(pMin.x, pMin.y + 8),
                    ImVec2(pMin.x + 5, pMax.y - 8),
                    IM_COL32(38, 188, 255, int(140 * highlight)), 2.5f
                );
            }
        }
        ImGui::EndChild();
    }

    void DrawTitleBar() {
        ImGui::SetCursorPos(ImVec2(24, 16));
        ImGui::TextColored(ImVec4(0.2f, 0.85f, 1.0f, 1.0f), "Edinaz Solutions");
        ImGui::SameLine(ImGui::GetWindowWidth() - 130);
        ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "Injected");
        ImGui::Separator();
    }

    void FadeInChild() {
        float alpha = tabAlpha[(int)currentTab];
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
    }

    void FadeOutChild() {
        ImGui::PopStyleVar();
    }

    void DrawAimbotTab() {
        FadeInChild();
        ImGui::Text("Aimbot");
        ImGui::Separator();
        ImGui::Checkbox("Enable Aimbot", &Config::aimbot);
        ImGui::SliderFloat("FOV", &Config::aimbot_fov, 1.f, 30.f, "%.1f");
        ImGui::SliderFloat("Smoothing", &Config::aimbot_smooth, 1.f, 10.f, "%.1f");
        ImGui::Checkbox("Enable Triggerbot", &Config::triggerbot);
        ImGui::SliderInt("Trigger Delay (ms)", &Config::trigger_delay, 0, 200);
        FadeOutChild();
    }

    void DrawVisualsTab() {
        FadeInChild();
        ImGui::Text("Visuals");
        ImGui::Separator();
        ImGui::Checkbox("Enable ESP", &Config::esp);
        ImGui::Checkbox("Box ESP", &Config::box_esp);
        ImGui::Checkbox("Head ESP", &Config::head_esp);
        ImGui::Checkbox("Health Bar", &Config::health_esp);
        ImGui::Checkbox("Distance", &Config::distance_esp);
        FadeOutChild();
    }

    void DrawMiscTab() {
        FadeInChild();
        ImGui::Text("Miscellaneous");
        ImGui::Separator();
        ImGui::Checkbox("Bunnyhop", &Config::bunnyhop);
        ImGui::Checkbox("No Recoil", &Config::no_recoil);

        ImGui::Spacing();
        ImGui::Text("Security");
        ImGui::Separator();
        ImGui::Checkbox("Hide From Debugger", &Config::hide_from_debugger);
        ImGui::Checkbox("Anti-Screenshot", &Config::anti_screenshot);
        ImGui::Checkbox("Obfuscate Strings", &Config::obfuscate_strings);
        ImGui::Checkbox("Randomize Window Class", &Config::randomize_window_class);
        ImGui::Checkbox("Hide Threads", &Config::hide_threads);
        ImGui::Checkbox("Use Manual Map", &Config::use_manual_map);

        ImGui::Spacing();
        if (ImGui::Button("Save Config", ImVec2(100, 28))) Config::Save("settings.json");
        ImGui::SameLine();
        if (ImGui::Button("Load Config", ImVec2(100, 28))) Config::Load("settings.json");
        FadeOutChild();
    }

    void Render() {
        if (!bShowMenu) return;
        ApplyModernTheme();

        ImGui::SetNextWindowSize(ImVec2(650, 500), ImGuiCond_Once);
        ImGui::Begin("##EdinazMainWindow", &bShowMenu, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

        DrawTitleBar();

        ImGui::Columns(2, nullptr, false);
        DrawSidebar();
        ImGui::NextColumn();

        ImGui::BeginChild("MainArea", ImVec2(0, 0), true);
        switch (currentTab) {
            case Tab::Aimbot:   DrawAimbotTab(); break;
            case Tab::Visuals:  DrawVisualsTab(); break;
            case Tab::Misc:     DrawMiscTab(); break;
        }
        ImGui::EndChild();
        ImGui::End();
    }
}
