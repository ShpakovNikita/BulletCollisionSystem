#pragma once
#include "SDL_events.h"
#include "Math\Line.hpp"
#include <optional>

class GameScene;
class Renderer;
struct SDL_Window;

class WallCreationController
{
public:
    WallCreationController(GameScene& gameScene, SDL_Window& window);

    void InputEvent(const SDL_Event& event);

    void DrawTrajectory(const Renderer* renderer);

private:
    SDL_Window& window;
    GameScene& gameScene;
    std::optional<Line> wallCreateInfo;
};