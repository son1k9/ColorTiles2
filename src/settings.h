#pragma once
#include <map>
#include <string_view>
#include <assert.h>
#include <string>
#include "raylib.h"

namespace SettingsNM {
    constexpr int maxColors = 10;

    class Skin {
    private:
        Sound hitSound_{};
        Sound missSound_{};

        Texture2D background_{};
        Texture2D colorTiles_[maxColors]{};
        Texture2D colorTilesRemoving_[maxColors]{};

        Texture2D emptyTiles_[2]{};
        Texture2D trail_[2]{};

        Texture2D cursor_;

        void assertIndex(int i, int less) const {
            assert(i > 0 && i < less);
        }

    public:
        const std::string name;

        Skin(std::string_view name);
        Skin(const Skin& other) = delete;
        Skin(Skin&& other) = delete;
        Skin& operator=(const Skin& other) = delete;
        Skin& operator=(Skin&& other) = delete;

        const Sound& hitSound() const {
            return hitSound_;
        }

        const Sound& missSound() const {
            return missSound_;
        }

        const Texture2D& background() const {
            return background_;
        }

        const Texture2D& colorTile(int i) const {
            assertIndex(i, maxColors);
            return colorTiles_[i];
        }

        const Texture2D& colorTileRemoving(int i) const {
            assertIndex(i, maxColors);
            return colorTilesRemoving_[i];
        }

        const Texture2D& emptyTile(int i) const {
            assertIndex(i, 2);
            return emptyTiles_[i];
        }

        const Texture2D& trail(int i) const {
            assertIndex(i, 2);
            return trail_[i];
        }

        const Texture2D& cursor() const {
            return cursor_;
        }
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