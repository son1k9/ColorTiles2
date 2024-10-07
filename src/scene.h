#pragma once 
#include <vector>
#include <string>
#include <memory>
#include <global_input.h>
#include "settings.h"
#include "utils.h"

namespace Scenes {
    //Interface
    class Scene {
    public:
        virtual void processInput() = 0;
        virtual std::unique_ptr<Scene> update(float delta) = 0;
        virtual void draw() = 0;
        virtual ~Scene() = default;
    };

    class SettingsScene final : public Scene {
    private:
        struct {
            bool skinEdit = false;
        } ui;

        struct {
            bool changeSkin = false;
        } controls;

        int previousSkinIndex = 0;
        int currentSkinIndex = 0;
        std::vector<std::string> skinNames;
        std::string currentSkinName;

    public:
        void processInput() override;
        std::unique_ptr<Scene> update(float delta) override;
        void draw() override;
        SettingsScene();
    };

    struct SettingsSceneState {
        SettingsScene& scene;
        bool toggleSettings = false;
        bool settingsActive = false;

        bool processInput();
        void update(float delta);
        void draw();
        SettingsSceneState(SettingsScene& scene);
    };

    class MainMenuScene final : public Scene {
    private:
        struct {
            bool fieldSizeEdit = false;
            bool colorsEdit = false;
            bool roundTimeEdit = false;
        } ui{};

        bool dialogActive_ = false;

        struct {
            bool toggleDialog = false;
            bool scores = false;
            bool quit = false;
            bool play = false;
        } controls{};

        SettingsSceneState settings;

    public:
        MainMenuScene(SettingsScene& scene);
        void processInput() override;
        std::unique_ptr<Scene> update(float delta) override;
        void draw() override;
    };

    class GameplayScene final : public Scene {
    private:
        struct {
            int f = 0;
        } ui;

        struct {
            bool toggleMenu = false;
            bool quit = false;
        } controls;

        bool menuActive = false;

        struct Tile {
            int color{};

            struct PositionData {
                Vector2 pos{};
                Vector2 vel{};
            };
        };

        const int colors{};
        const int fieldSize{};
        std::vector<Tile> field;

        std::string currentSeed;

        SettingsSceneState settings;

        int index2DTo1D(int x, int y) const {
            return y * fieldSize + x;
        }

        Utils::Vector2i index1DTo2D(int i) const {
            return { i % fieldSize, i / fieldSize };
        }

        void generateField(size_t seed);
        void reset();

    public:
        GameplayScene(SettingsScene& scene, const SettingsNM::Settings& settings);
        void processInput() override;
        std::unique_ptr<Scene> update(float delta) override;
        void draw() override;
    };

    class ScoresScene final : public Scene {
    private:
        struct {
            bool backToMenu = false;
        } controls;

        SettingsSceneState settings;
    public:
        ScoresScene(SettingsScene& scene);
        void processInput() override;
        std::unique_ptr<Scene> update(float delta) override;
        void draw() override;
    };
}