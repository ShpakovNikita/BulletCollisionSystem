#include "ApplicationImpl.hpp"

#include "imgui.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_mouse.h"

#include "Math/Vector2.hpp"
#include "Core/Renderer.hpp"
#include "Utils/Intersection.hpp"
#include "Math/DataStructures/Quadtree.hpp"
#include "Controllers/WallCreationController.hpp"
#include "Controllers/BulletCreationController.hpp"
#include "GameScene.hpp"
#include "Managers/BulletManager.hpp"

#include <vector>
#include <chrono>
#include <optional>
#include "Core/JobsPool.hpp"

namespace SApplicationImpl
{
    const std::vector<Line> kApplicationStartupWalls = {
        { { -0.98f, 0.98f }, { 0.98f, 0.98f } },
        { { 0.98f, 0.98f }, { 0.98f, -0.98f } },
        { { 0.98f, -0.98f }, { -0.98f, -0.98f } },
        { { -0.98f, -0.98f }, { -0.98f, 0.98f } },
    };

    float ChronoMicrosecondsToFloatSeconds(const std::chrono::microseconds& deltaTime)
    {
        return deltaTime.count() / (1000.0f * 1000.0f);
    }
}

void ApplicationImpl::Tick(const std::chrono::microseconds& deltaTime)
{
    Application::Tick(deltaTime);

    // This is very uncommon for me to have an update argument as a global time, that's why only test
    // task class have current global time as an argument
    float currentTimeInSeconds = appContext.GetApplicationExecutionTimeSec();
    bulletManager->Update(currentTimeInSeconds);

    float deltaTimeInSeconds = SApplicationImpl::ChronoMicrosecondsToFloatSeconds(deltaTime);
    gameScene->Update(deltaTimeInSeconds);
    DrawUI(deltaTimeInSeconds);

    wallCreationController->DrawTrajectory();
    bulletCreationController->DrawTrajectory();

    if (uiData.drawCollisionQuadTree)
    {
        gameScene->DrawCollisionQuadTree();
    }
}

void ApplicationImpl::InputEvent(const SDL_Event& event)
{
    Application::InputEvent(event);

    if (event.type == SDL_QUIT)
    {
        QuitApplication();
    }
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(appContext.window))
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
    : Application({ 144 })
{}

void ApplicationImpl::Init()
{
    Application::Init();

    gameScene = std::make_unique<GameScene>(appContext);
    bulletManager = std::make_unique<BulletManager>(*gameScene, appContext);

    for (const Line& wall : SApplicationImpl::kApplicationStartupWalls)
    {
        gameScene->AddWall(wall);
    }

    wallCreationController = std::make_unique<WallCreationController>(*gameScene, appContext);
    bulletCreationController = std::make_unique<BulletCreationController>(*bulletManager, appContext);
}

void ApplicationImpl::DrawUI(float frameTimeSec)
{
    ImGui::Begin("Control panel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowPos({ static_cast<float>(appContext.config.viewportWidth), 0.0f });
    ImGui::SetWindowSize({
        static_cast<float>(appContext.config.windowWidth - appContext.config.viewportWidth),
        static_cast<float>(appContext.config.windowHeight) });

    const float currentTimeInSeconds = appContext.GetApplicationExecutionTimeSec();
    const float frameTimeMs = frameTimeSec * 1000.0f;

    {
        const ImVec4 textColor = { 1.0f, 1.0f, 0.0f, 1.0f };
        char label[256];
        snprintf(label, sizeof(label), "Current frame time: %f ms", frameTimeMs);
        ImGui::TextColored(textColor, label);
    }

    {
        const ImVec4 textColor = { 1.0f, 1.0f, 0.0f, 1.0f };
        char label[256];
        snprintf(label, sizeof(label), "Current global time: %f sec", currentTimeInSeconds);
        ImGui::TextColored(textColor, label);
    }  
    
    {
        const ImVec4 textColor = { 0.7f, 0.7f, 1.0f, 1.0f };
        char label[256];
        snprintf(label, sizeof(label), "Background threads count: %zu", appContext.jobsPool->GetBackgroundThreadsCount());
        ImGui::TextColored(textColor, label);
    }

    {
        const ImVec4 textColor = { 0.0f, 1.0f, 0.0f, 1.0f };
        char label[256];
        snprintf(label, sizeof(label), "Current number of bullets in queue: %zu ", bulletManager->GetBulletsInQueueCount());
        ImGui::TextColored(textColor, label);
    }

    {
        const ImVec4 textColor = { 0.0f, 1.0f, 0.0f, 1.0f };
        char label[256];
        snprintf(label, sizeof(label), "Current number of bullets on scene: %zu ", bulletManager->GetBulletsOnSceneCount());
        ImGui::TextColored(textColor, label);
    }

    {
        const size_t kFrameTimesCount = 100;
        static std::vector<float> frameTimes(kFrameTimesCount, 0.0f);
        const float frameTimeLimit = 2.0f * 1000.0f / appContext.config.expectedFps;

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

        ImGui::TextWrapped("1. You can create bullets by clicking in the left mouse button, dragging it into the desired "
                           "direction, and releasing a mouse button. Bullet position will be the initial click position, "
                           "the direction will be shown as a line, and speed will be counted as this line's length.");
        ImGui::TextWrapped("2. You can create bullets by passing data in the UI below and clicking the Fire button.");
        ImGui::TextWrapped("3. You can create walls similar to the first bullet creation option, but instead of the left "
                           "mouse button, you should use the right one.");
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
            if (fireDirection == Vector2::kZero || speed == 0.0f || bulletLifetime == 0.0f || shotStartTime < currentTimeInSeconds)
            {
                uiData.fireInvalidDataErrorShown = true;
            }
            else
            {
                uiData.fireInvalidDataErrorShown = false;
                bulletCreationController->CreateFireTask(firePosition, fireDirection, speed, shotStartTime, bulletLifetime);
            }
        }

        if (uiData.fireInvalidDataErrorShown)
        {
            const ImVec4 textColor = { 1.0f, 0.0f, 0.0f, 1.0f };
            ImGui::TextColored(textColor, "You've passed the wrong data! Something crucial is invalid");
        }
    }

    ImGui::Separator();

    {
        ImGui::Checkbox("Is collision QuadTree debug draw enabled", &uiData.drawCollisionQuadTree);
    }

    ImGui::End();
}

ApplicationImpl::~ApplicationImpl() = default;