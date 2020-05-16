#pragma once

#include "Core\Application.hpp"
#include "Managers\BulletManager.hpp"
#include "GameScene.hpp"
#include <memory>

class WallCreationController;
class BulletCreationController;

class ApplicationImpl final : public Application
{
public:
    ApplicationImpl();
    ~ApplicationImpl();

private:
    struct UiData
    {
        bool drawCollisionQuadTree = false;
        bool fireInvalidDataErrorShown = false;
    } uiData;

    void Tick(const std::chrono::microseconds& deltaTime) override;
    void InputEvent(const SDL_Event& event) override;
    void Init() override;

    void DrawUI(float frameTimeSec);

    GameScene gameScene;
    BulletManager bulletManager;

    std::unique_ptr<WallCreationController> wallCreationController;
    std::unique_ptr<BulletCreationController> bulletCreationController;
};
