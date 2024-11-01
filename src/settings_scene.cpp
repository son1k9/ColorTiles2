#include <string>
#include <filesystem>
#include "raylib.h"
#include "scene.h"
#include "utils.h"
#include "raygui.h"

using namespace Scenes;

SettingsScene::SettingsScene() {
    namespace fs = std::filesystem;

    for (const auto& dirEntry : fs::directory_iterator{ "Skins" }) {
        if (dirEntry.is_directory()) {
            skinNames.push_back(dirEntry.path().filename().string());
        }
    }

    std::sort(skinNames.begin(), skinNames.end());

    for (int i = 0; i < skins.size(); i++) {
        if (SettingsNM::skin.name == skins[i]) {
            skinActive = i;
            previousSkin = i;
        }
        if (SettingsNM::skin.name == SettingsNM::defautSettings.skinName) {
            defaultSkinIndex = i;
        }
        skinsStr += skins[i];
        if ((i + 1) < skins.size()) {
            skinsStr += ";";
        }
    }
}

void SettingsScene::processInput() {

}

std::unique_ptr<Scene> SettingsScene::update(float delta) {
    if (changeSkin) {
        changeSkin = false;
        if (!setAndLoadSkin(skinNames[currentSkinIndex])){
            setAndLoadDefaultSkin();
            currentSkinIndex = defaultSkinIndex;
        }
    }

    return nullptr;
}

void SettingsScene::draw() {
    constexpr int width = 450;
    const int height = Utils::GetDisplayHeight();

    Color c = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
    Vector3 colorHSV = ColorToHSV(c);
    colorHSV.z += 0.1;
    if (colorHSV.z > 1.0f) {
        colorHSV.z = 1.0f;
    }
    const Color color = ColorFromHSV(colorHSV.x, colorHSV.y, colorHSV.z);

    GuiSetStyle(DEFAULT, BORDER_WIDTH, 2);
    DrawRectangle(0, 0, width, height, color);
    DrawLineEx(
        { .x = width, .y = 0 },
        { .x = width, .y = static_cast<float>(height) },
        GuiGetStyle(DEFAULT, BORDER_WIDTH),
        GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL)));

    constexpr int paddingX = 5;
    constexpr int paddingY = 15;
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 1);
    const Rectangle groupBox = {
        .x = paddingX,
        .y = paddingY,
        .width = width - 2 * paddingX,
        .height = static_cast<float>(height) - paddingY - paddingX };
    GuiGroupBox(groupBox, "Settings");

    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    if (ui.skinEdit) {
        GuiLock();
    }

    using namespace SettingsNM;

    GuiLabel(
        { .x = groupBox.x + 24, .y = groupBox.y + 64, .width = 150, .height = 24 },
        "Cursor size");
    GuiSliderBar(
        { .x = groupBox.x + 184, .y = groupBox.y + 64, .width = 120, .height = 24 },
        "0", "3",
        &settings.cursorSize, minSettings.cursorSize, maxSettings.cursorSize);
    std::string cursorSizeStr = std::to_string(settings.cursorSize);
    GuiLabel(
        { .x = groupBox.x + 334, .y = groupBox.y + 64, .width = 120, .height = 24 },
        cursorSizeStr.c_str());
    GuiLabel(
        { .x = groupBox.x + 24, .y = groupBox.y + 24, .width = 150, .height = 24 },
        "Skin");
    if (GuiDropdownBox({ .x = groupBox.x + 184, .y = groupBox.y + 24, .width = 120, .height = 24 },
        currentSkinName.c_str(), &currentSkinIndex, ui.skinEdit)) {
        if (ui.skinEdit){
            changeSkin = true;
        }
        ui.skinEdit = !ui.skinEdit;
    }
    GuiCheckBox({ .x = groupBox.x + 24, .y = groupBox.y + 104, .width = 24, .height = 24 },
        "Use skin's cursor", &settings.useCustomCursor);

    GuiUnlock();
}
