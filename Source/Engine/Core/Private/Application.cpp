#include "Core/Application.hpp"

#include "imgui.h"
#include "examples/imgui_impl_sdl.h"
#include "examples/imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#include <GL/gl3w.h>
#include <string>
#include <stdexcept>
#include "Core/Imgui.hpp"
#include "Core/Renderer.hpp"


Application::Application(const ApplicationConfig& aConfig)
    : appContext(aConfig)
{}

Application::~Application() = default;

void Application::Run()
{
    Init();

    std::chrono::duration frameDelta = std::chrono::microseconds(0);
    std::chrono::duration expectedFrameTime = std::chrono::microseconds(1000 * 1000 / appContext.config.expectedFps);

    std::chrono::time_point<std::chrono::steady_clock> currentTime = std::chrono::steady_clock::now();

    while (!appContext.exit)
    {
        std::chrono::time_point<std::chrono::steady_clock> newCurrentTime = std::chrono::steady_clock::now();
        frameDelta = std::chrono::duration_cast<std::chrono::microseconds>(newCurrentTime - currentTime);

        if (frameDelta >= std::chrono::seconds(1))
        {
            frameDelta = std::clamp(frameDelta, std::chrono::microseconds(0), expectedFrameTime);
        }

        appContext.currentExecutionTime = std::chrono::duration_cast<std::chrono::microseconds>(appContext.currentExecutionTime + frameDelta);
        currentTime = newCurrentTime;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            appContext.imgui->InputEvent(event);
            InputEvent(event);
        }

        appContext.renderer->StartFrame();
        appContext.imgui->StartFrame();

        Tick(frameDelta);

        appContext.renderer->DrawBatches();

        appContext.imgui->EndFrame();
        appContext.renderer->EndFrame();

        // Because of force enabled VSync we don't have to sleep here
    }

    Cleanup();
}

void Application::QuitApplication()
{
    appContext.exit = true;
}

const AppContext& Application::GetAppContext() const
{
    return appContext;
}

void Application::Tick(const std::chrono::microseconds&)
{}

void Application::InputEvent(const SDL_Event&)
{}

void Application::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        throw std::runtime_error("Error: " + std::string(SDL_GetError()));
    }

    SDL_WindowFlags window_flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    appContext.window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);

    appContext.renderer->Init(appContext.window);
    appContext.imgui->Init(appContext.renderer.get(), appContext.window);

    appContext.applicationInitTime = std::chrono::steady_clock::now();
}

void Application::Cleanup()
{
    appContext.imgui->Cleanup();
    appContext.renderer->Cleanup();

    SDL_DestroyWindow(appContext.window);
    SDL_Quit();
}
