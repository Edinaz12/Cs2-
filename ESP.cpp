#include "ESP.hpp"
#include <algorithm>
#include <string>

extern HDC g_hdc;
extern HDC g_memDC;

void BeginDraw() {
    HBRUSH clearBrush = CreateSolidBrush(RGB(0, 0, 0));
    RECT rect = { 0, 0, g_ScreenWidth, g_ScreenHeight };
    FillRect(g_memDC, &rect, clearBrush);
    DeleteObject(clearBrush);
}

void EndDraw() {
    BitBlt(g_hdc, 0, 0, g_ScreenWidth, g_ScreenHeight, g_memDC, 0, 0, SRCCOPY);
}

void DrawBox(int x, int y, int w, int h, COLORREF color) {
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    HGDIOBJ oldPen = SelectObject(g_memDC, pen);
    Rectangle(g_memDC, x, y, x + w, y + h);
    SelectObject(g_memDC, oldPen);
    DeleteObject(pen);
}

void DrawTextOut(int x, int y, const char* text, COLORREF color) {
    SetTextColor(g_memDC, color);
    SetBkMode(g_memDC, TRANSPARENT);
    TextOutA(g_memDC, x, y, text, static_cast<int>(strlen(text)));
}

void DrawCircle(int x, int y, int radius, COLORREF color) {
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    HGDIOBJ oldPen = SelectObject(g_memDC, pen);
    Ellipse(g_memDC, x - radius, y - radius, x + radius, y + radius);
    SelectObject(g_memDC, oldPen);
    DeleteObject(pen);
}

void DrawHealthBar(int x, int y, int width, int height, int hp) {
    int hpHeight = height * hp / 100;
    int hpY = y + height - hpHeight;

    HBRUSH green = CreateSolidBrush(RGB(0, 255, 0));
    RECT hpBar = { x, hpY, x + width, y + height };
    FillRect(g_memDC, &hpBar, green);
    DeleteObject(green);
}

void DrawESP(const std::vector<Entity>& ents, const Vector3& localPos, const float* mat) {
    COLORREF boxColor = RGB(88, 166, 255);
    COLORREF headColor = RGB(47, 129, 247);

    bool showHP   = CheatConfig::Get().Get("hp_enabled", true);
    bool showDist = CheatConfig::Get().Get("distance", true);
    bool headESP  = CheatConfig::Get().Get("head_enabled", true);

    for (const auto& e : ents) {
        if (!e.visible || e.health <= 0) continue;

        float dist = e.DistanceTo(localPos);
        Vector2 screen;
        if (!WorldToScreen(e.pos, screen, mat)) continue;

        int height = std::max(40, int(500 / (dist + 1)));
        int width = height / 2;

        int x = int(screen.x - width / 2);
        int y = int(screen.y - height);

        DrawBox(x, y, width, height, boxColor);

        if (headESP) {
            int headRadius = width / 4;
            DrawCircle(int(screen.x), y - headRadius, headRadius, headColor);
        }

        if (showHP) {
            DrawHealthBar(x - 6, y, 4, height, e.health);
        }

        if (showDist) {
            char buffer[32];
            sprintf_s(buffer, "%.1fm", dist);
            DrawTextOut(x + width / 2 - 16, y + height + 2, buffer, RGB(200, 200, 200));
        }
    }
}
