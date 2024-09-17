#include <memory>
#include "scene.h"
#include "global_input.h"
#include "scene.h"

using namespace Scenes;

MainMenuScene::MainMenuScene(SettingsScene& scene) : settingsScene_(scene) {}

void MainMenuScene::processInput() {
    toggleSettings_ |= GlobalInput::settings();

    if (settingsActive_) {
        settingsScene_.processInput();
        return;
    }

    play_ |= IsKeyPressed(KEY_ENTER);
}

std::unique_ptr<Scene> MainMenuScene::update(float delta) {
    if (toggleSettings_) {
        settingsActive_ = !settingsActive_;
    }

    if (settingsActive_) {
        settingsScene_.update(delta);
    }

    if (quit_) {
        //Close        
        return nullptr;
    }

    if (play_) {
        return std::make_unique<GameplayScene>();
    }

    if (scores_) {
        return std::make_unique<ScoresScene>();
    }

    return nullptr;
}

void MainMenuScene::draw() {
    if (settingsActive_) {
        settingsScene_.draw();
    }
}