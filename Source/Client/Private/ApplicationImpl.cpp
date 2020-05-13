#include "ApplicationImpl.hpp"
#include "imgui.h"
#include "SDL_rect.h"
#include "SDL_render.h"

void ApplicationImpl::Tick()
{
    if (showDemoWindow)
    {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }
}

void ApplicationImpl::Render()
{
    // TODO: use opengl renderer here
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
