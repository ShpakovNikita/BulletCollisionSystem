#include "GameScene.hpp"
#include "Core\Renderer.hpp"
#include <utility>
#include <assert.h>
#include "Math\MathHelpers.hpp"

GameScene::GameScene()
    : quadtree(AABBox2{ {-1.0f, -1.0f}, {1.0f, 1.0f} }, [](const Line& line)
{
    return MathHelpers::CreateBBox(std::get<0>(line), std::get<1>(line));
})
{

}

void GameScene::Update(float)
{
    DrawLevel();
}

void GameScene::DrawLevel()
{
    assert(renderer);

    std::vector<Vector2> lines;
    lines.reserve(walls.size() * 2);
    for (const Line& line : walls)
    {
        lines.push_back(std::get<0>(line));
        lines.push_back(std::get<1>(line));
    }

    renderer->DrawLines(lines);
    quadtree.DebugDraw(renderer);
}

void GameScene::SetRenderer(const Renderer* aRenderer)
{
    renderer = aRenderer;
}
