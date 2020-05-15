#include "ApplicationImpl.hpp"
#include "imgui.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "Math\Vector2.hpp"
#include <vector>
#include "Core\Renderer.hpp"
#include <chrono>
#include <optional>
#include "Utils\Intersection.hpp"
#include "SDL_mouse.h"

void ApplicationImpl::Tick(const std::chrono::microseconds& deltaTime)
{
    Application::Tick(deltaTime);

    float deltaTimeInSeconds = deltaTime.count() / (1000.0f * 1000.0f);
    // float currentTimeInSeconds = GetApplicationExecutionTime().count() / (1000.0f * 1000.0f);

    bulletManager.Update(deltaTimeInSeconds);
    gameScene.Update(deltaTimeInSeconds);

    if (showDemoWindow)
    {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

    if (currentBulletCreateInfo)
    {
        renderer->DrawLine(std::get<1>(currentBulletCreateInfo.value()), std::get<0>(currentBulletCreateInfo.value()));
    }
}

void ApplicationImpl::InputEvent(const SDL_Event& event)
{
    Application::InputEvent(event);

    if (event.type == SDL_QUIT)
    {
        QuitApplication();
    }
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
    {
        QuitApplication();
    }

    const ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse)
    {
        return;
    }

    if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
    {
        currentBulletCreateInfo;

        int width, height;
        SDL_GetWindowSize(window, &width, &height);

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

ApplicationImpl::ApplicationImpl()
    : Application({ 60 })
    , bulletManager(gameScene)
{
}

void ApplicationImpl::Init()
{
    Application::Init();

    gameScene.walls = {
        { { -0.9f, 0.9f }, { 0.9f, 0.9f } },
        { { 0.9f, 0.9f }, { 0.9f, -0.9f } },
        { { 0.9f, -0.9f }, { -0.9f, -0.9f } },
        { { -0.9f, -0.9f }, { -0.9f, 0.9f } },
    };

    gameScene.SetRenderer(renderer);
    bulletManager.SetRenderer(renderer);
}
