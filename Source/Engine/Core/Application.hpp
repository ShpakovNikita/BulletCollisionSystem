#pragma once
#include "SDL_events.h"
#include <string>
#include <chrono>

struct SDL_Window;
class Renderer;
class Imgui;

struct ApplicationConfig
{
    uint32_t fpsLimit = 60;
};

class Application
{
public:
    Application(const ApplicationConfig& config);
    virtual ~Application();

    void Run();
    void QuitApplication();

protected:
    virtual void Tick(const std::chrono::microseconds& deltaTime);
    virtual void InputEvent(const SDL_Event& event);
    virtual void Init();
    virtual void Cleanup();

    /**
    * Getting run loop execution time, which means our app execution time
    *
    * @note The value of this function will always be quantized to the frame time
    *
    */
    const std::chrono::microseconds& GetApplicationExecutionTime() const;

    SDL_Window* window = nullptr;
    Renderer* renderer = nullptr;
    Imgui* imgui = nullptr;
    ApplicationConfig config;

private:
    bool exit = false;
    std::chrono::microseconds currentExecutionTime = {};
    std::chrono::time_point<std::chrono::steady_clock> applicationInitTime = {};
};