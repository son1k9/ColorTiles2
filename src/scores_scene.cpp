#include "raylib.h"
#include "scene.h"

using namespace Scenes;

ScoresScene::ScoresScene(SettingsScene& scene) : settings(scene) {}

void ScoresScene::processInput() {
    if (!settings.processInput()) {
        return;
    }

    controls.backToMenu = IsKeyPressed(KEY_ESCAPE);
}

std::unique_ptr<Scene> ScoresScene::update(float delta) {
    settings.update(delta);

    if (controls.backToMenu) {
        return std::make_unique<MainMenuScene>(settings.scene);
    }

    return nullptr;
}

void ScoresScene::draw() {
    settings.draw();

    ClearBackground(RED);
}