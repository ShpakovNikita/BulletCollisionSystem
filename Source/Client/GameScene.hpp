#pragma once

#include "Math/Line.hpp"
#include "Math/DataStructures/Quadtree.hpp"

#include <vector>

class AppContext;

class GameScene
{
public:
    GameScene(const AppContext& appContext);

    void Update(float time);

    void DrawCollisionQuadTree();

    void AddWall(const Line& wall);
    void RemoveWall(const Line& wall);
    std::vector<Line> GetBBoxCollidedWalls(const Line& wall);

private:
    void DrawLevel();

    std::vector<Line> walls;
    Quadtree<Line> quadtree;

    const AppContext& appContext;
};