#include "GameScene.hpp"
#include "Core\Renderer.hpp"
#include <utility>
#include <assert.h>

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
}

void GameScene::SetRenderer(const Renderer* aRenderer)
{
    renderer = aRenderer;
}
