#include "Controllers/BulletCreationController.hpp"
#include "Managers/BulletManager.hpp"
#include "Core/Renderer.hpp"

BulletCreationController::BulletCreationController(BulletManager& aBulletManager, SDL_Window& aWindow)
    : window(aWindow)
    , bulletManager(aBulletManager)
{

}

void BulletCreationController::InputEvent(const SDL_Event& event)
{
    if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
    {
        currentBulletCreateInfo;

        int width, height;
        SDL_GetWindowSize(&window, &width, &height);

        // TODO: remove after coordinates setup in engine
        float x = (static_cast<float>(event.button.x) / width - 0.5f) * 2.0f;
        float y = -(static_cast<float>(event.button.y) / height - 0.5f) * 2.0f;

        switch (event.type)
        {
        case SDL_MOUSEMOTION:
            if (currentBulletCreateInfo)
            {
                std::get<1>(currentBulletCreateInfo.value()) = { x, y };
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                currentBulletCreateInfo = { { x, y }, { x, y } };
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                Vector2& startPoint = std::get<0>(currentBulletCreateInfo.value());
                Vector2& endPoint = std::get<1>(currentBulletCreateInfo.value());
                endPoint = { x, y };

                if (startPoint != endPoint)
                {
                    Vector2 fireVelocity = (endPoint - startPoint);
                    Vector2 fireDirection = fireVelocity.Normalized();
                    float bulletSpeed = fireVelocity.Length();
                    float test = fireDirection.Length();

                    bulletManager.Fire(startPoint, fireDirection, bulletSpeed, test, 1000.0f);
                }

                currentBulletCreateInfo = std::nullopt;
            }
            break;
        }
    }
}

void BulletCreationController::DrawTrajectory(const Renderer* renderer)
{
    if (currentBulletCreateInfo)
    {
        renderer->DrawLine(std::get<1>(currentBulletCreateInfo.value()), std::get<0>(currentBulletCreateInfo.value()));
    }
}
