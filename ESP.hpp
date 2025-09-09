#pragma once
#include <vector>
#include <Windows.h>
#include "../memory.hpp"     // Entity, Vector3
#include "../globals.hpp"    // Vector2
#include "../config.hpp"     // CheatConfig::Get()

void DrawESP(const std::vector<Entity>& ents, const Vector3& localPos, const float* mat);

// Optional: eigene Drawing-Wrappers (für später austauschbar auf ImGui)
void DrawBox(int x, int y, int w, int h, COLORREF color);
void DrawTextOut(int x, int y, const char* text, COLORREF color);
void DrawCircle(int x, int y, int radius, COLORREF color);
void DrawHealthBar(int x, int y, int width, int height, int hp);
void BeginDraw();
void EndDraw();
