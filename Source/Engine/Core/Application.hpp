#pragma once

struct SDL_Renderer;
struct SDL_Window;

class Application
{
public:
    void Run();

protected:
    virtual void Tick() = 0;
    virtual void Render() = 0;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};