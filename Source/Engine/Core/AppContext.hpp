#pragma once
#include <chrono>
#include <memory>

struct SDL_Window;
class Renderer;
class Imgui;

struct ApplicationConfig
{
    uint32_t expectedFps = 144;

    uint32_t viewportWidth = 720;
    uint32_t viewportHeight = 720;

    uint32_t windowWidth = 1280;
    uint32_t windowHeight = 720;
};

class AppContext
{
public:
    AppContext(const ApplicationConfig& config);
    ~AppContext();
    /**
    * Getting run loop execution time, which means our app execution time
    *
    * @note The value of this function will always be quantized to the frame time
    *
    */
    const std::chrono::microseconds& GetApplicationExecutionTime() const;

    // window lifetime managed by SDLs functions
    mutable SDL_Window* window = nullptr;

    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Imgui> imgui;

    ApplicationConfig config;

private:
    friend class Application;

    bool exit = false;
    std::chrono::microseconds currentExecutionTime = {};
    std::chrono::time_point<std::chrono::steady_clock> applicationInitTime = {};
};