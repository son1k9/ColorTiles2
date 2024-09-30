#include <scene.h>

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
        scene.draw();
    }
}