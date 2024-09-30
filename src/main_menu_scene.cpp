#include <memory>
#include "raygui.h"
#include "rayutils.h"
#include "scene.h"
#include "global_input.h"
#include "scene.h"

using namespace Scenes;

extern bool windowShouldClose;

MainMenuScene::MainMenuScene(SettingsScene& scene) : settings_(scene) {}

void MainMenuScene::processInput() {
    if (!settings_.processInput()) {
        return;
    }

    if (dialogActive_) {
        controls_.toggleDialog |= IsKeyPressed(KEY_ESCAPE);
        controls_.play |= IsKeyPressed(KEY_ENTER);
        return;
    }

    controls_.toggleDialog |= IsKeyPressed(KEY_ENTER);
}

std::unique_ptr<Scene> MainMenuScene::update(float delta) {
    settings_.update(delta);

    if (controls_.toggleDialog) {
        dialogActive_ = !dialogActive_;
        controls_.toggleDialog = false;
    }

    if (controls_.quit) {
        windowShouldClose = true;
        return nullptr;
    }

    if (controls_.play) {
        return std::make_unique<GameplayScene>(settings_.scene);
    }

    if (controls_.scores) {
        return std::make_unique<ScoresScene>(settings_.scene);
    }

    return nullptr;
}

void MainMenuScene::draw() {
    settings_.draw();

    GuiSetStyle(DEFAULT, TEXT_SIZE, 40);
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 2);

    int width = Rayutils::GetDisplayWidth();
    int height = Rayutils::GetDisplayHeight();

    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    int buttomWidth = 300;
    int buttonHeight = 80;
    int gap = 25;

    if (settings_.settingsActive || dialogActive_) {
        GuiLock();
    }

    controls_.toggleDialog |= static_cast<bool>(GuiButton({
        .x = (width - buttomWidth) / 2.f,
        .y = (height - buttonHeight) / 2.f,
        .width = static_cast<float>(buttomWidth),
        .height = static_cast<float>(buttonHeight) },
        "Play"));

    controls_.scores = GuiButton({
        .x = (width - buttomWidth) / 2.f,
        .y = (height - buttonHeight) / 2.f + buttonHeight + gap,
        .width = static_cast<float>(buttomWidth),
        .height = static_cast<float>(buttonHeight) },
        "Scores");

    settings_.toggleSettings |= static_cast<bool>(GuiButton({
        .x = (width - buttomWidth) / 2.f,
        .y = (height - buttonHeight) / 2.f + 2 * (buttonHeight + gap),
        .width = static_cast<float>(buttomWidth),
        .height = static_cast<float>(buttonHeight) },
        "Settings"));

    controls_.quit = GuiButton({
        .x = (width - buttomWidth) / 2.f,
        .y = (height - buttonHeight) / 2.f + 3 * (buttonHeight + gap),
        .width = static_cast<float>(buttomWidth),
        .height = static_cast<float>(buttonHeight) },
        "Quit");

    GuiUnlock();

    if (dialogActive_) {
        DrawRectangle((width - 200) / 2, (height - 200) / 2, 200, 200, BLACK);
    }
}