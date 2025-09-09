#include "ESP.hpp"
#include "globals.hpp"
#include "config.hpp"
#include "memory.hpp"
#include "../backend/backend.hpp"  // g_Mem

#include "imgui.h"

void DrawESP(const std::vector<Entity>& ents, const Vector3& localPos, const float* viewMatrix) {
    auto draw = ImGui::GetBackgroundDrawList();

    bool showHP   = CheatConfig::Get().Get("hp_enabled", true);
    bool showDist = CheatConfig::Get().Get("distance", true);
    bool headESP  = CheatConfig::Get().Get("head_enabled", true);

    for (const auto& e : ents) {
        if (!e.visible || e.health <= 0) continue;

        float dist = e.DistanceTo(localPos);
        Vector2 screen;
        if (!WorldToScreen(e.pos, screen, viewMatrix)) continue;

        int height = std::max(40, int(500 / (dist + 1)));
        int width = height / 2;

        ImVec2 topLeft = ImVec2(screen.x - width / 2, screen.y - height);
        ImVec2 bottomRight = ImVec2(screen.x + width / 2, screen.y);

        // Box
        draw->AddRect(topLeft, bottomRight, IM_COL32(88, 166, 255, 255));

        // Head
        if (headESP) {
            float headX = screen.x;
            float headY = screen.y - height;
            float radius = width / 4.0f;
            draw->AddCircle(ImVec2(headX, headY), radius, IM_COL32(47, 129, 247, 255));
        }

        // Healthbar
        if (showHP) {
            float hpPerc = e.health / 100.0f;
            float barHeight = height * hpPerc;
            draw->AddRectFilled(
                ImVec2(topLeft.x - 5, topLeft.y + (height - barHeight)),
                ImVec2(topLeft.x - 2, bottomRight.y),
                IM_COL32(0, 255, 0, 255)
            );
        }

        // Distance
        if (showDist) {
            char buf[32];
            sprintf_s(buf, "%.1fm", dist);
            draw->AddText(ImVec2(topLeft.x, bottomRight.y + 2), IM_COL32(255, 255, 255, 255), buf);
        }
    }
}
