#include "EntityManager.hpp"
#include "../backend/backend.hpp"
#include "../globals.hpp"
#include "../memory.hpp"
#include "../visibility/TraceRay.hpp"

void UpdateEntities(std::vector<Entity>& outEntities, Vector3& outLocalPos, int& outLocalTeam) {
    outEntities.clear();

    // LocalPlayer Info lesen
    outLocalPos = g_Mem.Read<Vector3>(g_Mem.localPawn + g_Mem.m_vecOrigin);
    outLocalTeam = g_Mem.Read<int>(g_Mem.localPawn + g_Mem.m_iTeamNum);

    for (int i = 0; i < 64; ++i) {
        DWORD_PTR entBase = g_Mem.Read<DWORD_PTR>(g_Mem.entityList + i * 8);
        if (!entBase) continue;

        Entity e;
        e.id     = i;
        e.health = g_Mem.Read<int>(entBase + g_Mem.m_iHealth);
        e.team   = g_Mem.Read<int>(entBase + g_Mem.m_iTeamNum);
        e.pos    = g_Mem.Read<Vector3>(entBase + g_Mem.m_vecOrigin);
        e.visible = IsVisible(outLocalPos, e.pos); // TraceRay check

        outEntities.push_back(e);
    }
}
