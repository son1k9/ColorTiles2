#include "raylib.h"
#include "scene.h"

using namespace Scenes;

ScoresScene::ScoresScene(SettingsScene& scene) : settings_(scene) {}

void ScoresScene::processInput() {
    if (!settings_.processInput()) {
        return;
    }

    controls_.backToMenu = IsKeyPressed(KEY_ESCAPE);
}

std::unique_ptr<Scene> ScoresScene::update(float delta) {
    settings_.update(delta);

    if (controls_.backToMenu) {
        return std::make_unique<MainMenuScene>(settings_.scene);
    }

    return nullptr;
}

void ScoresScene::draw() {
    settings_.draw();

    ClearBackground(RED);
}