#pragma once
#include "SDL_events.h"
#include <chrono>
#include "AppContext.hpp"


class Application
{
public:
    Application(const ApplicationConfig& config);
    virtual ~Application();

    void Run();
    void QuitApplication();
    const AppContext& GetAppContext() const;

protected:
    virtual void Tick(const std::chrono::microseconds& deltaTime);
    virtual void InputEvent(const SDL_Event& event);
    virtual void Init();
    virtual void Cleanup();

    AppContext appContext;
};