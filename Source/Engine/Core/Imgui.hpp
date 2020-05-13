#pragma once
#include "SDL_events.h"

class Renderer;
struct SDL_Window;

class Imgui
{
private:
    void Init(Renderer* renderer, SDL_Window* window);
    void Cleanup();
    void EndFrame();
    void StartFrame();
    void InputEvent(const SDL_Event& event);

    SDL_Window* window = nullptr;

    friend class Application;
};