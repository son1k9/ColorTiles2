#include "assert.h"
#include "raygui.h"
#include "random.h"
#include "scene.h"
#include "seed_generator.h"

using namespace Scenes;

static bool isEven(int x) {
    return (x & 1) == 0;
}

GameplayScene::GameplayScene(SettingsScene& scene, int fieldSize, int colors)
	: settings(scene), colors{ colors }, fieldSize{ fieldSize }, field(fieldSize), colorsVec(fieldSize) {
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
	settings.draw();

	const int width = Utils::GetDisplayWidth();
	const int height = Utils::GetDisplayHeight();

    ClearBackground(BLUE);

	//Draw game field

	if (!menuActive) {
		return;
	}

	//Draw menu
	if (settings.settingsActive) {
		GuiLock();
	}

	int buttonWidth = 300;
	int buttonHeight = 80;
	int gap = 25;

	controls.toggleMenu |= static_cast<bool>(GuiButton({
		.x = (width - buttonWidth) / 2.f,
		.y = (height - buttonHeight) / 2.f,
		.width = static_cast<float>(buttonWidth),
		.height = static_cast<float>(buttonHeight) },
		"Continue"));

	settings.toggleSettings |= static_cast<bool>(GuiButton({
		.x = (width - buttonWidth) / 2.f,
		.y = (height - buttonHeight) / 2.f + (buttonHeight + gap),
		.width = static_cast<float>(buttonWidth),
		.height = static_cast<float>(buttonHeight) },
		"Settings"));

	controls.quit = GuiButton({
		.x = (width - buttonWidth) / 2.f,
		.y = (height - buttonHeight) / 2.f + 2 * (buttonHeight + gap),
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
			colorsVec[k++] = i;
		}
	}

	while (k < colorsVec.size()) {
		colorsVec[k++] = 0;
	}

	Random::seed(seed);
	std::shuffle(colorsVec.begin(), colorsVec.end(), Random::mt);

	for (int i = 0; i < field.size(); i++) {
		field[i].color = colorsVec[i];
		field[i].isAlive = static_cast<bool>(colorsVec[i]);
	}
}