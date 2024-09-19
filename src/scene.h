#pragma once 
#include <memory>

namespace Scenes {
    //Just an interface
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

    class MainMenuScene final : public Scene {
    private:
        struct {
            bool fieldSizeEdit = false;
            bool colorsEdit = false;
            bool roundTimeEdit = false;
        } ui_{};

        SettingsScene& settingsScene_;

        struct {
            bool play = false;
            bool scores = false;
            bool quit = false;
            bool toggleSettings = false;
        } controls_{};

        bool settingsActive_ = false;

    public:
        MainMenuScene(SettingsScene& scene);
        void processInput() override;
        std::unique_ptr<Scene> update(float delta) override;
        void draw() override;
    };

    class GameplayScene final : public Scene {
    private:
        SettingsScene& settingsScene_;
    public:
        GameplayScene(SettingsScene& scene);
        void processInput() override;
        std::unique_ptr<Scene> update(float delta) override;
        void draw() override;
    };

    class ScoresScene final : public Scene {
    private:
        SettingsScene& settingsScene_;
    public:
        ScoresScene(SettingsScene& scene);
        void processInput() override;
        std::unique_ptr<Scene> update(float delta) override;
        void draw() override;
    };
}