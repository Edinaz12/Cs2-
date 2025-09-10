#pragma once
#include <vector>
#include "globals.hpp"

struct Entity {
    int id;
    int health;
    int team;
    Vector3 pos;
    bool visible;
};

void Aimbot(const std::vector<Entity>& ents, const Vector3& localPos, int localTeam);
