#include "scene.h"

using namespace Scenes;

ScoresScene::ScoresScene(SettingsScene& scene) : settingsScene_(scene) {

}

void ScoresScene::processInput() {

}

std::unique_ptr<Scene> ScoresScene::update(float delta) {
    return nullptr;
}

void ScoresScene::draw() {

}