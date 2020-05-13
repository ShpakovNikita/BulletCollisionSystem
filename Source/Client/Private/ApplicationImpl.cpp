#include "ApplicationImpl.hpp"
#include "imgui.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "Math\Vector2.hpp"
#include <vector>
#include "Core\Renderer.hpp"

void ApplicationImpl::Tick()
{
    if (showDemoWindow)
    {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

    std::vector<std::tuple<Vector2, Vector2>> level = { 
        { { -0.5f, 0.0f }, { 0.5f, 0.0f } },
        { { -0.5f, 0.1f }, { 0.5f, 0.1f } },
    };

    for (const auto& [wallStart, wallEnd] : level)
    {
        renderer->DrawLine(wallStart, wallEnd);
    }
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

std::string ApplicationImpl::GetAssetsDir()
{
#if defined(ASSETS_DIR)
    return ASSETS_DIR;
#else
    return "./../Assets/";
#endif
}
