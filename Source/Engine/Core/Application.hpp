#pragma once

struct SDL_Window;
class Renderer;
class Imgui;

class Application
{
public:
    void Run();

protected:
    virtual void Tick() = 0;
    virtual void Render() = 0;

    SDL_Window* window = nullptr;
    Renderer* renderer = nullptr;
    Imgui* imgui = nullptr;

private:
    void Init();
    void Cleanup();
};