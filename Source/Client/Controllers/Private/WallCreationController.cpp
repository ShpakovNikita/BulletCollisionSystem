#include "Controllers/WallCreationController.hpp"
#include "GameScene.hpp"
#include "Core/Renderer.hpp"
#include "Core/AppContext.hpp"

WallCreationController::WallCreationController(GameScene& aGameScene, const AppContext& aAppContext)
    : appContext(aAppContext)
    , gameScene(aGameScene)
{

}

void WallCreationController::InputEvent(const SDL_Event& event)
{
    if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
    {
        int width, height;
        SDL_GetWindowSize(appContext.window, &width, &height);

        // TODO: remove after coordinates setup in engine
        float x = (static_cast<float>(event.button.x) / width - 0.5f) * 2.0f;
        float y = -(static_cast<float>(event.button.y) / height - 0.5f) * 2.0f;

        switch (event.type)
        {
        case SDL_MOUSEMOTION:
            if (wallCreateInfo)
            {
                std::get<1>(wallCreateInfo.value()) = { x, y };
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                wallCreateInfo = { { x, y }, { x, y } };
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                Vector2& startPoint = std::get<0>(wallCreateInfo.value());
                Vector2& endPoint = std::get<1>(wallCreateInfo.value());
                endPoint = { x, y };

                if (startPoint != endPoint)
                {
                    gameScene.walls.push_back({ startPoint, endPoint });
                    gameScene.quadtree.Insert(gameScene.walls.back());
                }

                wallCreateInfo = std::nullopt;
            }
            break;
        }
    }
}

void WallCreationController::DrawTrajectory()
{
    if (wallCreateInfo)
    {
        appContext.renderer->DrawLine(std::get<1>(wallCreateInfo.value()), std::get<0>(wallCreateInfo.value()));
    }
}
