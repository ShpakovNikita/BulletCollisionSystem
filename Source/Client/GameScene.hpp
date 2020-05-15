#pragma once

#include <vector>
#include "Math\Line.hpp"

class Renderer;

class GameScene
{
public:
    void Update(float time);

    void SetRenderer(const Renderer* renderer);

    std::vector<Line> walls;

private:
    void DrawLevel();

    const Renderer* renderer = nullptr;
};