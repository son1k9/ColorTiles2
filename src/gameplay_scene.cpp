#include "scene.h"

using namespace Scenes;

GameplayScene::GameplayScene(SettingsScene& scene) : settingsScene_(scene) {

}

void GameplayScene::processInput() {

}

std::unique_ptr<Scene> GameplayScene::update(float delta) {
    return nullptr;
}

void GameplayScene::draw() {

}