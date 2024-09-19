#include <memory>
#include "raygui.h"
#include "rayutils.h"
#include "scene.h"
#include "global_input.h"
#include "scene.h"

using namespace Scenes;

extern bool windowShouldClose;

MainMenuScene::MainMenuScene(SettingsScene& scene) : settingsScene_(scene) {}

void MainMenuScene::processInput() {
    controls_.toggleSettings |= GlobalInput::settings();

    if (settingsActive_) {
        settingsScene_.processInput();
        controls_.toggleSettings |= IsKeyDown(KEY_ESCAPE);
        return;
    }

    controls_.play |= IsKeyPressed(KEY_ENTER);
}

std::unique_ptr<Scene> MainMenuScene::update(float delta) {
    if (settingsActive_) {
        settingsScene_.update(delta);
    }

    if (controls_.toggleSettings) {
        controls_.toggleSettings = false;
        settingsActive_ = !settingsActive_;
    }

    if (controls_.quit) {
        windowShouldClose = true;
        return nullptr;
    }

    if (controls_.play) {
        return std::make_unique<GameplayScene>(settingsScene_);
    }

    if (controls_.scores) {
        return std::make_unique<ScoresScene>(settingsScene_);
    }

    return nullptr;
}

void MainMenuScene::draw() {
    if (settingsActive_) {
        settingsScene_.draw();
    }

    GuiSetStyle(DEFAULT, TEXT_SIZE, 40);
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 2);

    int width = Rayutils::GetDisplayWidth();
    int height = Rayutils::GetDisplayHeight();

    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    int buttomWidth = 300;
    int buttonHeight = 80;
    int gap = 25;

    controls_.play = GuiButton({
        .x = (width - buttomWidth) / 2.f,
        .y = (height - buttonHeight) / 2.f,
        .width = static_cast<float>(buttomWidth),
        .height = static_cast<float>(buttonHeight) },
        "Play");

    controls_.scores = GuiButton({
        .x = (width - buttomWidth) / 2.f,
        .y = (height - buttonHeight) / 2.f + buttonHeight + gap,
        .width = static_cast<float>(buttomWidth),
        .height = static_cast<float>(buttonHeight) },
        "Scores");

    controls_.toggleSettings |= GuiButton({
        .x = (width - buttomWidth) / 2.f,
        .y = (height - buttonHeight) / 2.f + 2 * (buttonHeight + gap),
        .width = static_cast<float>(buttomWidth),
        .height = static_cast<float>(buttonHeight) },
        "Settings");

    controls_.quit = GuiButton({
        .x = (width - buttomWidth) / 2.f,
        .y = (height - buttonHeight) / 2.f + 3 * (buttonHeight + gap),
        .width = static_cast<float>(buttomWidth),
        .height = static_cast<float>(buttonHeight) },
        "Quit");
}