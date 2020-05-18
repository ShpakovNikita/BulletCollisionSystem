#pragma once
#include "SDL_events.h"
#include "Math\Line.hpp"
#include <optional>

class BulletManager;
class AppContext;

class BulletCreationController
{
public:
    BulletCreationController(BulletManager& bulletManager, const AppContext& appContext);

    void InputEvent(const SDL_Event& event);

    void DrawTrajectory();

    void CreateFireTask(const Vector2& pos, const Vector2& dir, float speed, float time, float lifeTime);

    void SetDelayTask(bool delayTask);
    bool GetDelayTask();

private: 
    const AppContext& appContext;
    BulletManager& bulletManager;
    std::optional<Line> currentBulletCreateInfo;
    bool delayTask = false;
};