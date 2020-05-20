#include "Controllers/WallCreationController.hpp"

#include "GameScene.hpp"
#include "Core/Renderer.hpp"
#include "Core/AppContext.hpp"
#include "Utils/Display.hpp"

WallCreationController::WallCreationController(GameScene& aGameScene, const AppContext& aAppContext)
    : appContext(aAppContext)
    , gameScene(aGameScene)
{

}

void WallCreationController::InputEvent(const SDL_Event& event)
{
    if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
    {
        Vector2 mouseCoords = Display::GetViewportSpaceCoordinates(
            event.button.x, event.button.y, appContext.config.viewportWidth, appContext.config.viewportHeight);

        switch (event.type)
        {
        case SDL_MOUSEMOTION:
            if (wallCreateInfo)
            {
                std::get<1>(wallCreateInfo.value()) = mouseCoords;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                wallCreateInfo = { mouseCoords, mouseCoords };
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                Vector2& startPoint = std::get<0>(wallCreateInfo.value());
                Vector2& endPoint = std::get<1>(wallCreateInfo.value());
                endPoint = mouseCoords;

                if (startPoint != endPoint)
                {
                    gameScene.AddWall({ startPoint, endPoint });
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
