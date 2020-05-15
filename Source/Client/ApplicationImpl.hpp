#pragma once

#include "Core\Application.hpp"
#include "Managers\BulletManager.hpp"
#include "GameScene.hpp"
#include <optional>

class ApplicationImpl final : public Application
{
public:
    ApplicationImpl();

private:
    void Tick(const std::chrono::microseconds& deltaTime) override;
    void InputEvent(const SDL_Event& event) override;
    void Init() override;

    bool showDemoWindow = true;
    GameScene gameScene;
    BulletManager bulletManager;

    // TODO: move to separate class
    std::optional<Line> currentBulletCreateInfo;
};
