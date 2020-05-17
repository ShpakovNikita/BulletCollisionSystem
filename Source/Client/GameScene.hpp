#pragma once

#include <vector>
#include "Math\Line.hpp"
#include "Math\DataStructures\Quadtree.hpp"

class AppContext;

class GameScene
{
public:
    GameScene(const AppContext& appContext);

    void Update(float time);

    void DrawCollisionQuadTree();

    // TODO: move both to private
    std::vector<Line> walls;
    Quadtree<Line> quadtree;

private:
    void DrawLevel();

    const AppContext& appContext;
};