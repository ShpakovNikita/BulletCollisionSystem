#pragma once
#include "SDL_events.h"

class AppContext;

class Imgui
{
public:
    Imgui(const AppContext& appContext);

private:
    void Init();
    void Cleanup();
    void EndFrame();
    void StartFrame();
    void InputEvent(const SDL_Event& event);

    const AppContext& appContext;

    friend class Application;
};