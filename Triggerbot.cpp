#include "Triggerbot.hpp"
#include <Windows.h>
#include <chrono>
#include <cmath>
#include <string>

#include "../globals.hpp"         // g_ScreenWidth, g_ScreenHeight, HEAD_OFFSET
#include "../config.hpp"          // CheatConfig::Get()
#include "../backend/backend.hpp" // g_Mem
#include "../utils/utils.hpp"     // WorldToScreen()

void Triggerbot(const std::vector<Entity>& ents, const Vector3& localPos, int localTeam) {
    static auto lastShot = std::chrono::steady_clock::now();
    if (!(GetAsyncKeyState(VK_XBUTTON1) & 0x8000)) return;

    std::string hitbox = CheatConfig::Get().Get<std::string>("trigger_hitbox", "any");
    int delay = CheatConfig::Get().Get<int>("trigger_delay", 10);

    for (const auto& e : ents) {
        if (!e.visible || e.health <= 0 || e.team == localTeam) continue;

        Vector3 pos = e.pos;

        if (hitbox == "head") pos.z += HEAD_OFFSET;
        else if (hitbox == "chest") pos.z += HEAD_OFFSET / 2;

        Vector2 screen;
        if (!WorldToScreen(pos, screen, reinterpret_cast<float*>(g_Mem.viewMatrix))) continue;

        int cx = g_ScreenWidth / 2;
        int cy = g_ScreenHeight / 2;

        float dx = fabs(screen.x - cx);
        float dy = fabs(screen.y - cy);
        float maxDist = 3.0f;

        if (dx <= maxDist && dy <= maxDist) {
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastShot).count() >= delay) {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                lastShot = now;
                break;
            }
        }
    }
}
