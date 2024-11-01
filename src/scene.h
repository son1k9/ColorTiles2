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

        int currentSkinIndex = 0;
        int defaultSkinIndex = 0;
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
        using Tile = int;
        using PlayField = std::vector<Tile>;
        using ColorToTilesMap = std::map<int, std::vector<int>>;

        struct {
             int f = 0;
        } ui;

        struct {
            bool toggleMenu = false;
            bool quit = false;
            bool removeTiles = false;
        } controls;

        bool menuActive = false;

        Vector2 mousePosition;

        bool newFieldOnReset = true;

        const int colors{};
        const int fieldSize{};
        PlayField field;

        std::string currentSeed;

        SettingsSceneState settings;

        int index2DTo1D(int x, int y) const {
            return y * fieldSize + x;
        }

        Utils::Vector2i index1DTo2D(int i) const {
            return { i % fieldSize, i / fieldSize };
        }

        Tile getTileByPos(Utils::Vector2i position) {
            int index = index2DTo1D(position.x, position.y);
            // TODO: Assert index
            return field[index];
        }

        void generateField(size_t seed);
        void reset();
        Utils::Vector2i worldToFieldPosition(Vector2 position) const;
        void fieldRelease(Utils::Vector2i position);
        int removeTiles(Utils::Vector2i position);
        ColorToTilesMap findTilesForPos(Utils::Vector2i position) const;
        void miss(Utils::Vector2i position);

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
