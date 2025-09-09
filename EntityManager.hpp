#pragma once
#include <vector>
#include "globals.hpp"
#include "memory.hpp"

struct Entity {
    int id = 0;
    int health = 0;
    int team = 0;
    Vector3 pos = {0.f, 0.f, 0.f};
    bool visible = false;

    float DistanceTo(const Vector3& other) const {
        float dx = pos.x - other.x;
        float dy = pos.y - other.y;
        float dz = pos.z - other.z;
        return sqrtf(dx * dx + dy * dy + dz * dz);
    }
};

class EntityManager {
public:
    void Update();

    const std::vector<Entity>& GetEntities() const { return m_entities; }
    const Vector3& GetLocalPos() const { return m_localPos; }
    int GetLocalTeam() const { return m_localTeam; }

private:
    std::vector<Entity> m_entities;
    Vector3 m_localPos {0.f, 0.f, 0.f};
    int m_localTeam = 0;
};
