#pragma once
#include <fstream>
#include <nlohmann/json.hpp>

namespace CheatConfig {
    inline bool espBox = true;
    inline bool healthBar = true;
    inline bool drawNames = true;

    inline bool aimbotEnabled = false;
    inline float aimbotFov = 10.0f;
    inline float aimbotSmoothing = 5.0f;
    inline int aimbotHitboxIndex = 0;

    inline bool triggerbotEnabled = false;
    inline int triggerDelay = 25;

    inline const char* configPath = "config.json";

    inline void Save() {
        nlohmann::json j;
        j["espBox"] = espBox;
        j["healthBar"] = healthBar;
        j["drawNames"] = drawNames;
        j["aimbotEnabled"] = aimbotEnabled;
        j["aimbotFov"] = aimbotFov;
        j["aimbotSmoothing"] = aimbotSmoothing;
        j["aimbotHitboxIndex"] = aimbotHitboxIndex;
        j["triggerbotEnabled"] = triggerbotEnabled;
        j["triggerDelay"] = triggerDelay;

        std::ofstream file(configPath);
        if (file.is_open()) {
            file << j.dump(4); // 4 = pretty print
            file.close();
        }
    }

    inline void Load() {
        std::ifstream file(configPath);
        if (!file.is_open()) return;

        nlohmann::json j;
        file >> j;
        file.close();

        espBox             = j.value("espBox", espBox);
        healthBar          = j.value("healthBar", healthBar);
        drawNames          = j.value("drawNames", drawNames);
        aimbotEnabled      = j.value("aimbotEnabled", aimbotEnabled);
        aimbotFov          = j.value("aimbotFov", aimbotFov);
        aimbotSmoothing    = j.value("aimbotSmoothing", aimbotSmoothing);
        aimbotHitboxIndex  = j.value("aimbotHitboxIndex", aimbotHitboxIndex);
        triggerbotEnabled  = j.value("triggerbotEnabled", triggerbotEnabled);
        triggerDelay       = j.value("triggerDelay", triggerDelay);
    }
}
