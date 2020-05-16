#pragma once
#include "SDL_events.h"
#include "Math\Line.hpp"
#include <optional>

class BulletManager;
class Renderer;
struct SDL_Window;

class BulletCreationController
{
public:
    BulletCreationController(BulletManager& bulletManager, SDL_Window& window);

    void InputEvent(const SDL_Event& event);

    void DrawTrajectory(const Renderer* renderer);

private: 
    SDL_Window& window;
    BulletManager& bulletManager;
    std::optional<Line> currentBulletCreateInfo;
};