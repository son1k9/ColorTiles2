#include "scene.h"

using namespace Scenes;

GameplayScene::GameplayScene(SettingsSceneState& scene) : settings_(scene) {}

void GameplayScene::processInput() {
    if (!settings_.processInput()) {
        return;
    }
}

std::unique_ptr<Scene> GameplayScene::update(float delta) {
    return nullptr;
}

void GameplayScene::draw() {
    ClearBackground(BLUE);
}