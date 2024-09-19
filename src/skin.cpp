#include <iostream>
#include <format>
#include <string>
#include <filesystem>
#include "raylib.h"
#include "raygui.h"
#include "settings.h"

SettingsNM::Skin::Skin(std::string_view name) : name{ name } {
    namespace fs = std::filesystem;
    const fs::path skin = std::format("Skins/{}", name);
    fs::current_path(skin);

    auto loadTexture = [](Texture2D& texture, const std::string& path) {
        texture = LoadTexture(path.c_str());
        if (texture.id <= 0) {
            std::cout << std::format("Failed to load {} - Loading file from default skin\n", path);
            std::string dPath = "../Default/" + path;
            texture = LoadTexture(dPath.c_str());
        }
        };

    background_ = LoadTexture("BackgroundImage.png");
    std::string tilePath;
    for (int i = 0; i < 10; i++) {
        tilePath = std::format("ColorTile{}.png", i);
        loadTexture(colorTiles_[i], tilePath);
    }
     
    for (int i = 0; i < 10; i++) {
        tilePath = std::format("ColorTileRemoving{}.png", i);
        loadTexture(colorTilesRemoving_[i], tilePath);
    }

    loadTexture(emptyTiles_[0], "EmptyTile0.png");
    loadTexture(emptyTiles_[1], "EmptyTile1.png");
    loadTexture(trail_[0], "Trail0.png");
    loadTexture(trail_[1], "Trail1.png");
    loadTexture(cursor_, "cursor.png");

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
            std::string stringPath = dirEntry.path().string();
            loadSound(hitSound_, stringPath);
            hitLoaded = true;
        }

        if (dirEntry.path().stem() == "miss") {
            std::string stringPath = dirEntry.path().string();
            loadSound(missSound_, stringPath);
            missLoaded = true;
        }

        if (dirEntry.path().extension() == ".rgs") {
            std::string stringPath = dirEntry.path().string();
            GuiLoadStyle(stringPath.c_str());
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
}
