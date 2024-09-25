#include <iostream>
#include <format>
#include <string>
#include <filesystem>
#include "raylib.h"
#include "raygui.h"
#include "settings.h"

using namespace SettingsNM;
namespace fs = std::filesystem;

Skin::Skin() {
}

bool Skin::exists() {
    return fs::exists("Skins/" + name) && fs::is_directory("Skins/" + name);
}

bool Skin::load() {
    if (!exists()) {
        return false;
    }

    if (loaded) {
        unload();
    }

    const fs::path skin = "Skins/" + name;
    fs::current_path(skin);

    auto loadTexture = [](Texture2D& texture, const std::string& path) {
        texture = LoadTexture(path.c_str());
        if (texture.id <= 0) {
            std::cout << std::format("Failed to load {} - Loading file from default skin\n", path);
            std::string dPath = "../Default/" + path;
            texture = LoadTexture(dPath.c_str());
        }
        };

    background = LoadTexture("BackgroundImage.png");
    std::string path;
    for (int i = 0; i < maxColors; i++) {
        path = std::format("ColorTile{}.png", i);
        loadTexture(colorTiles[i], path);
    }

    for (int i = 0; i < maxColors; i++) {
        path = std::format("ColorTileRemoving{}.png", i);
        loadTexture(colorTilesRemoving[i], path);
    }

    loadTexture(emptyTiles[0], "EmptyTile0.png");
    loadTexture(emptyTiles[1], "EmptyTile1.png");
    loadTexture(trail[0], "Trail0.png");
    loadTexture(trail[1], "Trail1.png");
    loadTexture(cursor, "cursor.png");

    auto loadSound = [](Sound& sound, const std::string& path) {
        sound = LoadSound(path.c_str());
        if (sound.frameCount <= 0) {
            std::cout << std::format("Failed to load {} - Loading file from default skin", path);
            std::string dPath = "../Default/" + path;
            sound = LoadSound(dPath.c_str());
        }
        };

    bool hitLoaded = false;
    bool missLoaded = false;
    bool styleLoaded = false;
    for (const auto& dirEntry : fs::directory_iterator{ fs::current_path() }) {
        if (dirEntry.path().stem() == "hit") {
            path = dirEntry.path().string();
            loadSound(hitSound, path);
            hitLoaded = true;
        }

        if (dirEntry.path().stem() == "miss") {
            path = dirEntry.path().string();
            loadSound(missSound, path);
            missLoaded = true;
        }

        if (dirEntry.path().extension() == ".rgs") {
            path = dirEntry.path().string();
            GuiLoadStyle(path.c_str());
            styleLoaded = true;
        }

        if (hitLoaded && missLoaded && styleLoaded) {
            break;
        }
    }

    if (!styleLoaded) {
        GuiLoadStyleDefault();
    }

    fs::current_path("..\\..\\");
    std::cout << "Skin loaded succesfuly\n";
    loaded = true;
    return true;
}


Skin::~Skin() {
    unload();
}

void Skin::unload() {
    UnloadSound(missSound);
    UnloadSound(hitSound);
    UnloadTexture(background);
    for (int i = 0; i < 10; i++) {
        UnloadTexture(colorTiles[i]);
    }
    for (int i = 0; i < 10; i++) {
        UnloadTexture(colorTilesRemoving[i]);
    }
    UnloadTexture(emptyTiles[0]);
    UnloadTexture(emptyTiles[1]);
    UnloadTexture(trail[0]);
    UnloadTexture(trail[1]);
    UnloadTexture(cursor);
    loaded = false;
}