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
#include "Math\DataStructures\Quadtree.hpp"
#include "Controllers\WallCreationController.hpp"
#include "Controllers\BulletCreationController.hpp"

void ApplicationImpl::Tick(const std::chrono::microseconds& deltaTime)
{
    Application::Tick(deltaTime);

    float deltaTimeInSeconds = deltaTime.count() / (1000.0f * 1000.0f);
    // float currentTimeInSeconds = GetApplicationExecutionTime().count() / (1000.0f * 1000.0f);

    bulletManager.Update(deltaTimeInSeconds);
    gameScene.Update(deltaTimeInSeconds);

    DrawUI(deltaTimeInSeconds);

    wallCreationController->DrawTrajectory(renderer);
    bulletCreationController->DrawTrajectory(renderer);

    if (uiData.drawCollisionQuadTree)
    {
        gameScene.DrawCollisionQuadTree();
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
    if (!io.WantCaptureMouse)
    {
        wallCreationController->InputEvent(event);
        bulletCreationController->InputEvent(event);
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
        { { 0.8f, 0.9f }, { 0.9f, 0.8f } },
        { { 0.8f, 0.9f }, { 0.9f, 0.8f } },
        { { 0.8f, 0.9f }, { 0.9f, 0.8f } },
        { { 0.8f, 0.88f }, { 0.88f, 0.8f } },
        { { 0.8f, 0.85f }, { 0.85f, 0.8f } },
        { { 0.8f, 0.82f }, { 0.82f, 0.8f } },
        { { 0.8f, 0.82f }, { 0.82f, 0.8f } },
        { { 0.8f, 0.82f }, { 0.82f, 0.8f } },
        { { 0.8f, 0.82f }, { 0.82f, 0.8f } },
    };

    for (const Line& wall : gameScene.walls)
    {
        gameScene.quadtree.Insert(wall);
    }

    wallCreationController = std::make_unique<WallCreationController>(gameScene, *window);
    bulletCreationController = std::make_unique<BulletCreationController>(bulletManager, *window);

    gameScene.SetRenderer(renderer);
    bulletManager.SetRenderer(renderer);
}

void ApplicationImpl::DrawUI(float frameTimeSec)
{
    ImGui::Begin("Control panel");

    const float frameTimeMs = frameTimeSec * 1000.0f;

    {
        const ImVec4 textColor = { 1.0f, 1.0f, 0.0f, 1.0f };
        char label[256];
        snprintf(label, sizeof(label), "Current frame time: %f ms", frameTimeMs);
        ImGui::TextColored(textColor, label);
    }

    {
        float currentTimeInSeconds = GetApplicationExecutionTime().count() / (1000.0f * 1000.0f);
        const ImVec4 textColor = { 1.0f, 1.0f, 0.0f, 1.0f };
        char label[256];
        snprintf(label, sizeof(label), "Current global time: %f sec", currentTimeInSeconds);
        ImGui::TextColored(textColor, label);
    }

    {
        const size_t kFrameTimesCount = 100;
        static std::vector<float> frameTimes(kFrameTimesCount, 0.0f);
        const float frameTimeLimit = 1000.0f / config.fpsLimit;

        // Shift vector values to create circular buffer effect
        for (size_t i = 1; i < kFrameTimesCount; ++i)
        {
            frameTimes[i - 1] = frameTimes[i];
        }
        frameTimes[kFrameTimesCount - 1] = frameTimeMs;

        ImGui::PlotLines("Frame Times", frameTimes.data(), kFrameTimesCount, 0, "", 0.0f, frameTimeLimit);
    }

    ImGui::Separator();

    {
        const ImVec4 textColor = { 0.0f, 1.0f, 0.0f, 1.0f };
        ImGui::TextColored(textColor, "HERE IS A LITTLE GUIDE:");

        ImGui::TextWrapped("1. You can create bullets by clicking in the left mouse button, dragging it int desired"
                           "direction, and releasing mouse button. Bullet position will be the initial click position,"
                           "direction will be shown as line, and speed will be counted as this line's length.");
        ImGui::TextWrapped("2. You can create bullets by passing data in the UI below and clicking the Fire button.");
        ImGui::TextWrapped("3. You can create walls similarly to the first bullet creation option, but instead of lest"
                           "mouse button you should use right one.");
        ImGui::TextWrapped("Notes: Also, below you can find some other interesting debug stuff to click on.");
    }

    ImGui::Separator();

    {
        static float fireDir[2] = {0.0f, 0.0f};
        ImGui::InputFloat2("Fire direction", fireDir);
        Vector2 fireDirection = { fireDir[0], fireDir[1] };

        static float firePos[2] = { 0.0f, 0.0f };
        ImGui::InputFloat2("Fire position", firePos);
        Vector2 firePosition = { firePos[0], firePos[1] };

        static float speed = 0.0f;
        ImGui::InputFloat("Fire speed", &speed);

        static float shotStartTime = 0.0f;
        ImGui::InputFloat("Start time", &shotStartTime);

        static float bulletLifetime = 0.0f;
        ImGui::InputFloat("Bullet lifetime", &bulletLifetime);

        if (ImGui::Button("Fire!"))
        {
            if (fireDirection == Vector2::kZero || speed == 0.0f || bulletLifetime == 0.0f)
            {
                uiData.fireInvalidDataErrorShown = true;
            }
            else
            {
                uiData.fireInvalidDataErrorShown = false;
                bulletManager.Fire(firePosition, fireDirection, speed, shotStartTime, bulletLifetime);
            }
        }

        if (uiData.fireInvalidDataErrorShown)
        {
            const ImVec4 textColor = { 1.0f, 0.0f, 0.0f, 1.0f };
            ImGui::TextColored(textColor, "You passed wrong data! Something crucial is equals to 0.0f");
        }
    }

    ImGui::Separator();

    {
        ImGui::Checkbox("Is collision QuadTree debug draw enabled", &uiData.drawCollisionQuadTree);
    }

    ImGui::End();
}

ApplicationImpl::~ApplicationImpl() = default;