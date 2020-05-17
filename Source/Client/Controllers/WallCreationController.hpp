#pragma once
#include "SDL_events.h"
#include "Math\Line.hpp"
#include <optional>

class GameScene;
class AppContext;

class WallCreationController
{
public:
    WallCreationController(GameScene& gameScene, const AppContext& appContext);

    void InputEvent(const SDL_Event& event);

    void DrawTrajectory();

private:
    const AppContext& appContext;
    GameScene& gameScene;
    std::optional<Line> wallCreateInfo;
};