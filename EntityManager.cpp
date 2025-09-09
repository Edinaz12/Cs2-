#include "EntityManager.hpp"
#include "backend.hpp"
#include "globals.hpp"

// Sichtbarkeitsfunktion (falls TraceRay vorhanden)
extern bool IsVisible(const Vector3& from, const Vector3& to);

void EntityManager::Update() {
    m_entities.clear();

    if (!g_Mem.localPawn) return;

    // LocalPlayer
    m_localPos  = g_Mem.Read<Vector3>(g_Mem.localPawn + g_Mem.m_vecOrigin);
    m_localTeam = g_Mem.Read<int>(g_Mem.localPawn + g_Mem.m_iTeamNum);

    // Entities
    for (int i = 0; i < 64; ++i) {
        DWORD_PTR entBase = g_Mem.Read<DWORD_PTR>(g_Mem.entityList + i * 8);
        if (!entBase) continue;

        Entity e;
        e.id     = i;
        e.health = g_Mem.Read<int>(entBase + g_Mem.m_iHealth);
        e.team   = g_Mem.Read<int>(entBase + g_Mem.m_iTeamNum);
        e.pos    = g_Mem.Read<Vector3>(entBase + g_Mem.m_vecOrigin);
        e.visible = IsVisible(m_localPos, e.pos);

        if (e.health > 0) {
            m_entities.push_back(e);
        }
    }
}
