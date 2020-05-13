#pragma once

class Renderer;
struct SDL_Window;

class Imgui
{
private:
    void Init(Renderer* renderer, SDL_Window* window);
    void Cleanup();
    void EndFrame();
    void StartFrame();

    SDL_Window* window = nullptr;

    friend class Application;
};