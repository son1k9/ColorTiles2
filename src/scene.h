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
        bool play_ = false;
        bool scores_ = false; 
        bool quit_ = false;
        bool toggleSettings_ = false;
        SettingsScene& settingsScene_;
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
        GameplayScene(std::unique_ptr<SettingsScene> ptr);
        void processInput() override;
        std::unique_ptr<Scene> update(float delta) override;
        void draw() override;
    };

    class ScoresScene final : public Scene {
    private:
        SettingsScene& settingsScene_;
    public:
        ScoresScene(std::unique_ptr<SettingsScene> ptr);
        void processInput() override;
        std::unique_ptr<Scene> update(float delta) override;
        void draw() override;
    };
}