#pragma once 
#include <memory>
#include <global_input.h>

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
    public:
        void processInput() override;
        std::unique_ptr<Scene> update(float delta) override;
        void draw() override;
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


        } ui_{};

        bool dialogActive_ = false;

        struct {
            bool toggleDialog = false;
            bool scores = false;
            bool quit = false;

            bool play = false;
        } controls_{};

        SettingsSceneState settings_;

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
        } ui_;

        struct {
            bool toggleMenu = false;
        } controls_;

        bool menuActive_ = false;

        SettingsSceneState settings_;
    public:
        GameplayScene(SettingsSceneState& scene);
        void processInput() override;
        std::unique_ptr<Scene> update(float delta) override;
        void draw() override;
    };

    class ScoresScene final : public Scene {
    private:
        struct {
            bool backToMenu = false;
        } controls_;

        SettingsSceneState settings_;
    public:
        ScoresScene(SettingsScene& scene);
        void processInput() override;
        std::unique_ptr<Scene> update(float delta) override;
        void draw() override;
    };
}