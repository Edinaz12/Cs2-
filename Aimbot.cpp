#include "Aimbot.hpp"

#include <Windows.h>
#include <cfloat>
#include "config.hpp"           // CheatConfig::Get()
#include "../backend/backend.hpp"  // g_Mem
#include "globals.hpp"          // g_ScreenWidth, g_ScreenHeight, HEAD_OFFSET
#include "../utils/utils.hpp"      // WorldToScreen()

void Aimbot(const std::vector<Entity>& ents, const Vector3& localPos, int localTeam) {
    if (!(GetAsyncKeyState(VK_RBUTTON) & 0x8000)) return;

    float fov = CheatConfig::Get().Get<float>("fov", 10.0f);
    float smoothing = CheatConfig::Get().Get<float>("smoothing", 3.0f);

    const Entity* bestTarget = nullptr;
    float bestScreenDist = FLT_MAX;

    Vector2 screenCenter(g_ScreenWidth / 2.0f, g_ScreenHeight / 2.0f);

    for (const auto& e : ents) {
        if (!e.visible || e.health <= 0 || e.team == localTeam)
            continue;

        Vector3 target = e.pos;
        target.z += HEAD_OFFSET;

        Vector2 screen;
        float* mat = reinterpret_cast<float*>(g_Mem.viewMatrix);
        if (!WorldToScreen(target, screen, mat))
            continue;

        float dist = (screen - screenCenter).Length();

        if (dist < fov && dist < bestScreenDist) {
            bestScreenDist = dist;
            bestTarget = &e;
        }
    }

    if (!bestTarget) return;

    Vector3 target = bestTarget->pos;
    target.z += HEAD_OFFSET;

    Vector2 screen;
    float* mat = reinterpret_cast<float*>(g_Mem.viewMatrix);
    if (!WorldToScreen(target, screen, mat))
        return;

    Vector2 delta = screen - screenCenter;

    // Mausbewegung mit Smoothing
    INPUT in = { 0 };
    in.type = INPUT_MOUSE;
    in.mi.dx = LONG(delta.x / smoothing);
    in.mi.dy = LONG(delta.y / smoothing);
    in.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &in, sizeof(INPUT));
}
