#include "raylib.h"
#include "scene.h"

using namespace Scenes;

void SettingsScene::processInput() {

}

std::unique_ptr<Scene> SettingsScene::update(float delta) {
    return nullptr;
}

void SettingsScene::draw() {
    DrawRectangle(0, 0, 400, 400, BLACK);
}