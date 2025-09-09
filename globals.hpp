#pragma once
#include <Windows.h>
#include <cmath>

// ==== Vector Structs ====
struct Vector2 { float x, y; };
struct Vector3 { float x, y, z; };

// ==== Globals ====
inline int g_ScreenWidth = 1920;
inline int g_ScreenHeight = 1080;
inline const float HEAD_OFFSET = 65.0f;

// ==== WorldToScreen ====
inline bool WorldToScreen(const Vector3& pos, Vector2& out, const float* m) {
    float w = m[3] * pos.x + m[7] * pos.y + m[11] * pos.z + m[15];
    if (w < 0.001f) return false;

    float x = m[0] * pos.x + m[4] * pos.y + m[8] * pos.z + m[12];
    float y = m[1] * pos.x + m[5] * pos.y + m[9] * pos.z + m[13];

    x /= w;
    y /= w;

    out.x = (g_ScreenWidth / 2.0f) + (x * g_ScreenWidth / 2.0f);
    out.y = (g_ScreenHeight / 2.0f) - (y * g_ScreenHeight / 2.0f);

    return std::isfinite(out.x) && std::isfinite(out.y);
}
