#include "globals.hpp"
#include "memory.hpp"
#include "EntityManager.hpp"
#include "ESP.hpp"
#include "Aimbot.hpp"
#include "Triggerbot.hpp"
#include "backend.hpp"

// Optional: für Sleep
#include <thread>

// Hauptfunktion für Cheat-Logik (wird jeden Frame im Hook aufgerufen)
void CheatMain()
{
    // Entity-Daten vorbereiten
    std::vector<Entity> ents;
    Vector3 localPos = { 0.f, 0.f, 0.f };
    int localTeam = 0;

    // Entity-Daten lesen
    UpdateEntities(ents, localPos, localTeam);

    // ViewMatrix lesen
    float* mat = reinterpret_cast<float*>(g_Mem.viewMatrix);

    // === ESP zeichnen ===
    if (CheatConfig::Get().Get<bool>("esp", true))
        DrawESP(ents, localPos, mat);

    // === Aimbot ausführen ===
    if (CheatConfig::Get().Get<bool>("aimbot", true))
        Aimbot(ents, localPos, localTeam);

    // === Triggerbot ausführen ===
    if (CheatConfig::Get().Get<bool>("triggerbot", true))
        Triggerbot(ents, localPos, localTeam);

    // === Recoil Control (RCS) ===
    if (CheatConfig::Get().Get<bool>("rcs", true))
        ApplyRCS(); // optional: kannst du später implementieren
}
