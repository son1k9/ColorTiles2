#include <memory>
#include "settings.h"
#include "raygui.h"
#include "utils.h"
#include "scene.h"
#include "global_input.h"
#include "scene.h"

using namespace Scenes;

extern bool windowShouldClose;

MainMenuScene::MainMenuScene(SettingsScene& scene) : settings(scene) {}

void MainMenuScene::processInput() {
    if (!settings.processInput()) {
        return;
    }

    if (dialogActive_) {
        controls.toggleDialog |= IsKeyPressed(KEY_ESCAPE);
        controls.play |= IsKeyPressed(KEY_ENTER);
        return;
    }

    controls.toggleDialog |= IsKeyPressed(KEY_ENTER);
}

std::unique_ptr<Scene> MainMenuScene::update(float delta) {
    settings.update(delta);

    if (controls.toggleDialog) {
        dialogActive_ = !dialogActive_;
        controls.toggleDialog = false;
    }

    if (controls.quit) {
        windowShouldClose = true;
        return nullptr;
    }

    if (controls.play) {
        return std::make_unique<GameplayScene>(settings.scene, SettingsNM::settings);
    }

    if (controls.scores) {
        return std::make_unique<ScoresScene>(settings.scene);
    }

    return nullptr;
}

void MainMenuScene::draw() {
    GuiSetStyle(DEFAULT, TEXT_SIZE, 40);
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 2);

    int width = Utils::GetDisplayWidth();
    int height = Utils::GetDisplayHeight();

    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    int buttonWidth = 300;
    int buttonHeight = 80;
    int gap = 25;

    if (settings.settingsActive || dialogActive_) {
        GuiLock();
    }

    controls.toggleDialog |= static_cast<bool>(GuiButton({
        .x = (width - buttonWidth) / 2.f,
        .y = (height - buttonHeight) / 2.f,
        .width = static_cast<float>(buttonWidth),
        .height = static_cast<float>(buttonHeight) },
        "Play"));

    controls.scores = GuiButton({
        .x = (width - buttonWidth) / 2.f,
        .y = (height - buttonHeight) / 2.f + buttonHeight + gap,
        .width = static_cast<float>(buttonWidth),
        .height = static_cast<float>(buttonHeight) },
        "Scores");

    settings.toggleSettings |= static_cast<bool>(GuiButton({
        .x = (width - buttonWidth) / 2.f,
        .y = (height - buttonHeight) / 2.f + 2 * (buttonHeight + gap),
        .width = static_cast<float>(buttonWidth),
        .height = static_cast<float>(buttonHeight) },
        "Settings"));

    controls.quit = GuiButton({
        .x = (width - buttonWidth) / 2.f,
        .y = (height - buttonHeight) / 2.f + 3 * (buttonHeight + gap),
        .width = static_cast<float>(buttonWidth),
        .height = static_cast<float>(buttonHeight) },
        "Quit");

    GuiUnlock();

    if (dialogActive_) {
        DrawRectangle((width - 200) / 2, (height - 200) / 2, 200, 200, BLACK);
    }

    settings.draw();
}
