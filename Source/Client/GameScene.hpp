#pragma once

#include <vector>
#include "Math\Line.hpp"
#include "Math\DataStructures\Quadtree.hpp"

class Renderer;

class GameScene
{
public:
    GameScene();

    void Update(float time);

    void SetRenderer(const Renderer* renderer);

    std::vector<Line> walls;
    Quadtree<Line> quadtree;

private:
    void DrawLevel();

    const Renderer* renderer = nullptr;
};