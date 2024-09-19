#pragma once
#include <map>
#include <string_view>
#include <assert.h>
#include <string>
#include "raylib.h"

namespace SettingsNM {
    constexpr int maxColors = 10;

    struct Skin {
        Sound hitSound{};
        Sound missSound{};

        Texture2D background{};
        Texture2D colorTiles[maxColors]{};
        Texture2D colorTilesRemoving[maxColors]{};

        Texture2D emptyTiles[2]{};
        Texture2D trail[2]{};

        Texture2D cursor;

        const std::string name;
        Skin(const std::string& name);
        Skin(const Skin& other) = delete;
        Skin(Skin&& other) = delete;
        Skin& operator=(const Skin& other) = delete;
        Skin& operator=(Skin&& other) = delete;
        ~Skin();
        void load();
    };

    struct Settings {
        float cursorSize{};
        int fieldSize{};
        int roundTime{};
        int colors{};
        bool useCustomCursor{};
        std::string skinName;
    };

    extern std::unique_ptr<Skin> skin;
    extern Settings settings;

    inline const Settings defaultSettings{
        .cursorSize = 1.0f,
        .fieldSize = 20,
        .roundTime = 120,
        .colors = 10,
        .useCustomCursor = true,
        .skinName = "Default",
    };

    inline const Settings minSettings{
        .cursorSize = 0.1f,
        .fieldSize = 4,
        .roundTime = 5,
        .colors = 1,
        .useCustomCursor = true,
        .skinName = "Default",
    };

    inline const Settings maxSettings{
        .cursorSize = 3.0f,
        .fieldSize = 20,
        .roundTime = 120,
        .colors = 10,
        .useCustomCursor = true,
        .skinName = "Default",
    };

    std::map<std::string, std::string> cfgToMap(std::string_view cfg);
    std::string mapToCfg(std::map<std::string, std::string> m);

    void setSettingsFromMap(std::map<std::string, std::string>& values);
    std::map<std::string, std::string> settingsToMap();

    void setDefaultSettings();
}