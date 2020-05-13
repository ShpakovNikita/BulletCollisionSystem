#pragma once

class Renderer;
struct SDL_Window;

class Imgui
{
private:
    void Init(Renderer* renderer, SDL_Window* window);
    void Cleanup();
    
    friend class Application;
};