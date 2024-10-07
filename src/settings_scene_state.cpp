#include <scene.h>
#include "utils.h"

using namespace Scenes;

SettingsSceneState::SettingsSceneState(SettingsScene& scene) : scene(scene) {}

bool SettingsSceneState::processInput() {
    toggleSettings |= GlobalInput::settings();

    if (settingsActive) {
        scene.processInput();
        toggleSettings |= IsKeyPressed(KEY_ESCAPE);
        return false;
    }

    return true;
}

void SettingsSceneState::update(float delta) {
    if (settingsActive) {
        scene.update(delta);
    }

    if (toggleSettings) {
        toggleSettings = false;
        settingsActive = !settingsActive;
    }
}

void SettingsSceneState::draw() {
    if (settingsActive) {
        const int width = Utils::GetDisplayWidth();
        const int height = Utils::GetDisplayHeight();
        DrawRectangle(0, 0, width, height, ColorAlpha(BLACK, 0.2f));
        scene.draw();
    }
}