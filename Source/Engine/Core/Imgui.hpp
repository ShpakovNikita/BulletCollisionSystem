#pragma once

#include "SDL_events.h"
#include "IEngineSubsystem.hpp"

class AppContext;

class Imgui
    : public IEngineSubsystem
{
public:
    Imgui(const AppContext& appContext);

    void Init() override;
    void Cleanup() override;
    void EndFrame() override;
    void StartFrame() override;

private:
    void InputEvent(const SDL_Event& event);

    const AppContext& appContext;

    friend class Application;
};