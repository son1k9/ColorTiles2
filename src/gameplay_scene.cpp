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
    : settings(scene), playField { settings.fieldSize, settings.colors } {
    reset();
}

void GameplayScene::reset() {
    if (newFieldOnReset) {
        currentSeed = SeedGenerator::getSeedString();
    }
    generateField(SeedGenerator::strToSeed(currentSeed));
}

void GameplayScene::processInput() {
    mousePosition = GetMousePosition();

    if (!settings.processInput()) {
        return;
    }

    // TODO: Check if mouse is on field first
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        controls.removeTiels = true;
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
        retrun nullptr;
    }

    if (controls.removeTiles) {
        auto position = worldToFieldPosition(mousePosition);
        filedRelease(position);
        return nullptr;
    }

    return nullptr;
}

void GameplayScene::draw() {
    const int width = Utils::GetDisplayWidth();
    const int height = Utils::GetDisplayHeight();

    ClearBackground(BLUE);

    // TODO: Draw game field

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

static void generateField(PlayField& playField, PlayField& playField, float color = 0.8f) {
    int tilesForColor = static_cast<int>(playField.size * colorProbality) / playField.colors;
    if (!isEven(tilesForColor)) {
        tilesForColor = 1;
    }

    int k = 0;
    for (int i = 1; i <= playField.colors; i++) {
        for (int j = 0; j < tilesForColor; j++) {
            playField.field[k++] = i;
        }
    }

    while (k < field.size) {
        playField.field[k++] = Tile{ 0 };
    }

    Random::seed(seed);
    std::shuffle(playField.field.begin(), playField.field.end(), Random::mt);
}

// NOTE: Maybe this should not be a method
Utils::Vector2i GameplayScene::worldToFieldPosition(Vector2 position) {
    // TODO: Do assert instead
}

void GameplayScene::fieldRelease(Utils::Vector2i position) {
    auto tile = getTileByPos(position);
    if (tile != 0) {
        miss(position);
        // Release on colored tile
        return;
    }

    auto gainedScore = removeTiles(position);
    if (gainedScore == 0) {
        miss(position);
        return;
    }

    // TODO: Add and play hitsound score here
}

static int removeTiles(PlayField& playField, Utils::Vector2i position) {
    auto tiles = findTilesForPos(PlayField& playField, position);
    int score = 0;

    for (const auto& [color, tilesIndex] : tiles) {
        if (tiles.size() >= 2) {
            int count = 0;
            for (const auto& tileIndex : tilesIndex) {
                playField.field[tileIndex] = 0;
                count++;
            }
            score += (int)pow(2, count - 1);
        }
    }

    return score;
}

void miss(Utils::Vector2i position) {
}
