#pragma once
#include <string>

struct SDL_Window;

class Renderer
{
public:
    void DrawLine();

    inline void* GetApiContext() const { return apiContext; }
    inline const std::string& GetApiVersion() const { return apiVersion; }

private:
    void Init(SDL_Window* window);
    void Cleanup();
    void SubmitFrame();

    mutable void* apiContext = nullptr;
    std::string apiVersion;

    friend class Application;
};