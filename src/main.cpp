#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <memory>
#include "settings.h"
#include "raylib.h"
#include "scene.h"

bool windowShouldClose = false;

static bool processGlobalInput() {
    if (IsKeyPressed(KEY_F11)) {
        static int lastWidth{};
        static int lastHeight{};
        if (!IsWindowFullscreen()) {
            lastWidth = GetScreenWidth();
            lastHeight = GetScreenHeight();

            const auto monitor = GetCurrentMonitor();
            const auto mWidth = GetMonitorWidth(monitor);
            const auto mHeight = GetMonitorHeight(monitor);

            SetWindowSize(mWidth, mHeight);
            ToggleFullscreen();
        }
        else {
            ToggleFullscreen();
            SetWindowSize(lastWidth, lastHeight);
        }
    }
    return true;
}

int main() {
    using namespace Scenes;

    auto settingsPath = "settings.cfg";

    InitWindow(1600, 900, "Color Tiles");
    InitAudioDevice();
    SetMasterVolume(0.5f);
    SetTargetFPS(2 * GetMonitorRefreshRate(GetCurrentMonitor()));
    SetExitKey(-1);

    SettingsNM::setDefaultSettings();
    {
        std::ifstream settingsFile(settingsPath);
        if (settingsFile.is_open()) {
            std::stringstream buff;
            buff << settingsFile.rdbuf();
            auto settingsMap = SettingsNM::cfgToMap(buff.str());
            SettingsNM::setSettingsFromMap(settingsMap);
            SettingsNM::setSkinFromSettings();
        }
    }

    if (!SettingsNM::skin.load() && !SettingsNM::setAndLoadDefaultSkin()) {
        std::cout<<"Default skin does not exists";
        return 1;
    }

    SettingsScene settingsScene;
    std::unique_ptr<Scene> scene = std::make_unique<MainMenuScene>(settingsScene);

    while (!windowShouldClose && !WindowShouldClose()) {
        const auto delta = GetFrameTime();
        if (processGlobalInput()) {
            scene->processInput();
        }

        if (auto nextScene = scene->update(delta); nextScene) {
            scene = std::move(nextScene);
            continue;
        }

        BeginDrawing(); {
            scene->draw();
        }
        EndDrawing();
    }

    {
        auto settingsMap = SettingsNM::settingsToMap();
        auto cfg = SettingsNM::mapToCfg(settingsMap);
        std::ofstream settingsFile(settingsPath);
        settingsFile << cfg;
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
