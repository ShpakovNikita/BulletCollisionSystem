#pragma once
#include <chrono>
#include <memory>
#include <string>

struct SDL_Window;
class Renderer;
class Imgui;
class JobsPool;

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

    const float GetApplicationExecutionTimeSec() const;
    const float GetApplicationExecutionTimeMs() const;

    const std::string& GetShadersDir() const;

    // window lifetime managed by SDLs functions
    mutable SDL_Window* window = nullptr;

    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Imgui> imgui;
    std::unique_ptr<JobsPool> jobsPool;

    ApplicationConfig config;

private:
    friend class Application;

    bool exit = false;
    std::chrono::microseconds currentExecutionTime = {};
    std::chrono::time_point<std::chrono::steady_clock> applicationInitTime = {};

    std::string shadersDir;
};