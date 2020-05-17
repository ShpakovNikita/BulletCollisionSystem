#include "GameScene.hpp"
#include "Core\Renderer.hpp"
#include <utility>
#include <assert.h>
#include "Math\MathHelpers.hpp"
#include "Core\AppContext.hpp"

GameScene::GameScene(const AppContext& aAppContext)
    : quadtree(AABBox2{ {-1.0f, -1.0f}, {1.0f, 1.0f} }, [](const Line& line)
    {
        return MathHelpers::CreateBBox(std::get<0>(line), std::get<1>(line));
    })
    , appContext(aAppContext)
{

}

void GameScene::Update(float)
{
    DrawLevel();
}

void GameScene::DrawLevel()
{
    std::vector<Vector2> lines;
    lines.reserve(walls.size() * 2);
    for (const Line& line : walls)
    {
        lines.push_back(std::get<0>(line));
        lines.push_back(std::get<1>(line));
    }

    appContext.renderer->DrawLines(lines);
}

void GameScene::DrawCollisionQuadTree()
{
    quadtree.DebugDraw(appContext.renderer.get());
}
