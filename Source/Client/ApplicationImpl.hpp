#pragma once

#include "Core\Application.hpp"
#include "Managers\BulletManager.hpp"

class ApplicationImpl final : public Application
{
public:
    ApplicationImpl();

private:
    void Tick(const std::chrono::microseconds& deltaTime) override;
    void InputEvent(const SDL_Event& event) override;

    bool showDemoWindow = true;
    BulletManager bulletManager;
};
