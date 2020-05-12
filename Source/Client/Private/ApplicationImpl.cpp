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
