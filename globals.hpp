#pragma once
#include <Windows.h>
#include <cmath>

// ==== Vector2 und Vector3 Klassen ====
struct Vector2 {
    float x, y;
    Vector2() : x(0), y(0) {}
    Vector2(float _x, float _y) : x(_x), y(_y) {}

    Vector2 operator+(const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
    Vector2 operator-(const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
    Vector2 operator*(float s) const { return Vector2(x * s, y * s); }
    Vector2 operator/(float s) const { return Vector2(x / s, y / s); }

    float Length() const { return std::sqrt(x * x + y * y); }
    Vector2 Normalize() const { float l = Length(); return l > 0 ? Vector2(x / l, y / l) : Vector2(); }
    float Dot(const Vector2& rhs) const { return x * rhs.x + y * rhs.y; }
};

struct Vector3 {
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    Vector3 operator+(const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
    Vector3 operator-(const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
    Vector3 operator*(float s) const { return Vector3(x * s, y * s, z * s); }
    Vector3 operator/(float s) const { return Vector3(x / s, y / s, z / s); }

    float Length() const { return std::sqrt(x * x + y * y + z * z); }
    Vector3 Normalize() const { float l = Length(); return l > 0 ? Vector3(x / l, y / l, z / l) : Vector3(); }
    float Dot(const Vector3& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }
    Vector3 Cross(const Vector3& rhs) const {
        return Vector3(
            y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x
        );
    }
};

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
