#include <memory>
#include "raylib.h"
#include "scene.h"

bool windowShouldClose = false;

int main() {
    using namespace Scenes;
    InitWindow(1600, 900, "Color Tiles");
    InitAudioDevice();
    SetMasterVolume(0.5f);
    SetTargetFPS(2 * GetMonitorRefreshRate(GetCurrentMonitor()));
    SetExitKey(-1);

    //Read settings
    SettingsScene settingsScene;
    std::unique_ptr<Scene> scene = std::make_unique<MainMenuScene>(settingsScene);

    while (!windowShouldClose) {
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

    //Write settings

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

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