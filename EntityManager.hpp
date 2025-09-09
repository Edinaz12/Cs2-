#pragma once
#include <vector>
#include "../memory.hpp"   // Entity, Vector3

void UpdateEntities(std::vector<Entity>& outEntities, Vector3& outLocalPos, int& outLocalTeam);
