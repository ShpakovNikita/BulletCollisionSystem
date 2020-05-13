#pragma once
#include "SDL_events.h"
#include <string>

struct SDL_Window;
class Renderer;
class Imgui;

class Application
{
public:
    void Run();
    void QuitApplication();

protected:
    virtual void Tick() = 0;
    virtual void InputEvent(const SDL_Event& event) = 0;
    virtual std::string GetAssetsDir() = 0;

    SDL_Window* window = nullptr;
    Renderer* renderer = nullptr;
    Imgui* imgui = nullptr;

private:
    void Init();
    void Cleanup();

    bool exit = false;
};