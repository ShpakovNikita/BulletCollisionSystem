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

void ApplicationImpl::Tick(const std::chrono::microseconds& [[ maybe_unused ]] deltaTime)
{
    float currentTimeInSeconds = GetApplicationExecutionTime().count() / (1000.0f * 1000.0f);
    bulletManager.Update(currentTimeInSeconds);

    if (showDemoWindow)
    {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

    const std::vector<Vector2> level = {
        { -0.5f, 0.0f }, { 0.5f, 0.0f },
        { 0.0f, 0.5f }, { 0.0f, -0.5f },
    };

    {
        std::optional<Vector2> intersectionPoint = Intersection::SegmentSegmentIntersection(level[0], level[1], level[2], level[3]);

        if (intersectionPoint)
        {
            renderer->DrawPoint(intersectionPoint.value(), 0.02f);
        }
    }

    renderer->DrawLines(level);
}

void ApplicationImpl::InputEvent(const SDL_Event& event)
{
    if (event.type == SDL_QUIT)
    {
        QuitApplication();
    }
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
    {
        QuitApplication();
    }
}

ApplicationImpl::ApplicationImpl()
    : Application({ 60 })
{
    
}
