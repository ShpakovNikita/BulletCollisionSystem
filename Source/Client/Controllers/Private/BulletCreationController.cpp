#include "Controllers/BulletCreationController.hpp"
#include "Managers/BulletManager.hpp"
#include "Core/Renderer.hpp"
#include "Core/AppContext.hpp"
#include "Utils/Display.hpp"
#include "Core/JobsPool.hpp"

namespace SBulletCreationController
{
    constexpr float kDefaultBulletLifeTime = 5.0f;
}

BulletCreationController::BulletCreationController(BulletManager& aBulletManager, const AppContext& aAppContext)
    : appContext(aAppContext)
    , bulletManager(aBulletManager)
{

}

void BulletCreationController::InputEvent(const SDL_Event& event)
{
    if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
    {
        currentBulletCreateInfo;

        Vector2 mouseCoords = Display::GetViewportSpaceCoordinates(
            event.button.x, event.button.y, appContext.config.viewportWidth, appContext.config.viewportHeight);

        switch (event.type)
        {
        case SDL_MOUSEMOTION:
            if (currentBulletCreateInfo)
            {
                std::get<1>(currentBulletCreateInfo.value()) = mouseCoords;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                currentBulletCreateInfo = { mouseCoords, mouseCoords };
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                Vector2& startPoint = std::get<0>(currentBulletCreateInfo.value());
                Vector2& endPoint = std::get<1>(currentBulletCreateInfo.value());
                endPoint = mouseCoords;

                if (startPoint != endPoint)
                {
                    Vector2 fireVelocity = (endPoint - startPoint);
                    Vector2 fireDirection = fireVelocity.Normalized();
                    float bulletSpeed = fireVelocity.Length();

                    CreateFireTask(startPoint, fireDirection, bulletSpeed, appContext.GetApplicationExecutionTimeSec(), 
                        SBulletCreationController::kDefaultBulletLifeTime);
                }

                currentBulletCreateInfo = std::nullopt;
            }
            break;
        }
    }
}

void BulletCreationController::DrawTrajectory()
{
    if (currentBulletCreateInfo)
    {
        appContext.renderer->DrawLine(std::get<1>(currentBulletCreateInfo.value()), std::get<0>(currentBulletCreateInfo.value()));
    }
}

void BulletCreationController::CreateFireTask(
    const Vector2& pos, 
    const Vector2& dir,
    float speed,
    float time, 
    float lifeTime)
{
    appContext.jobsPool->CreateBackgroundJob([=] { bulletManager.Fire(pos, dir, speed, time, lifeTime); });
}
