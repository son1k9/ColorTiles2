#include "assert.h"
#include "raygui.h"
#include "random.h"
#include "scene.h"
#include "seed_generator.h"
#include "settings.h"

using namespace Scenes;

static bool isEven(int x) {
    return (x & 1) == 0;
}

GameplayScene::GameplayScene(SettingsScene& scene, const SettingsNM::Settings& settings)
    : settings(scene), colors{ settings.colors }, fieldSize{ settings.fieldSize }, field(settings.fieldSize) {
    reset();
}

void GameplayScene::reset() {
    currentSeed = SeedGenerator::getSeedString();
    generateField(SeedGenerator::strToSeed(currentSeed));
}

void GameplayScene::processInput() {
    if (!settings.processInput()) {
        return;
    }

    controls.toggleMenu |= IsKeyPressed(KEY_ESCAPE);
}

std::unique_ptr<Scene> GameplayScene::update(float delta) {
    settings.update(delta);

    if (controls.quit) {
        return std::make_unique<MainMenuScene>(settings.scene);
    }

    if (controls.toggleMenu) {
        menuActive = !menuActive;
        controls.toggleMenu = false;
    }

    return nullptr;
}

void GameplayScene::draw() {
    const int width = Utils::GetDisplayWidth();
    const int height = Utils::GetDisplayHeight();

    ClearBackground(BLUE);

    //Draw game field

    settings.draw();

    if (!menuActive) {
        return;
    }

    if (settings.settingsActive) {
        GuiLock();
    }

    constexpr int buttonWidth = 300;
    constexpr int buttonHeight = 80;
    constexpr int gap = 25;
    constexpr int totalHeight = 80 * 3 + 25 * 2;

    const float x = (width - buttonWidth) / 2.f;
    const float y = (height - totalHeight) / 2.f;

    controls.toggleMenu |= static_cast<bool>(GuiButton({
        .x = x,
        .y = y,
        .width = static_cast<float>(buttonWidth),
        .height = static_cast<float>(buttonHeight) },
        "Continue"));

    settings.toggleSettings |= static_cast<bool>(GuiButton({
        .x = x,
        .y = y + buttonHeight + gap,
        .width = static_cast<float>(buttonWidth),
        .height = static_cast<float>(buttonHeight) },
        "Settings"));

    controls.quit = GuiButton({
        .x = x,
        .y = y + 2 * (buttonHeight + gap),
        .width = static_cast<float>(buttonWidth),
        .height = static_cast<float>(buttonHeight) },
        "Quit");

    GuiUnlock();
}

void GameplayScene::generateField(size_t seed) {
    constexpr float colorProbality = 0.8f;
    const int tilesForColor = [this]() -> int {
        int tForColor = static_cast<int>(field.size() * colorProbality) / colors;
        if (!isEven(tForColor)) {
            tForColor -= 1;
        }
        return tForColor;
        }();

    int k = 0;
    for (int i = 1; i <= colors; i++) {
        for (int j = 0; j < tilesForColor; j++) {
            field[k++] = Tile{ i };
        }
    }

    while (k < field.size()) {
        field[k++] = Tile{ 0 };
    }

    Random::seed(seed);
    std::shuffle(field.begin(), field.end(), Random::mt);
}