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
    : config(aConfig)
{}

Application::~Application() = default;

void Application::Run()
{
    Init();

    std::chrono::duration frameDelta = std::chrono::microseconds(0);
    std::chrono::duration frameTimeLimit = std::chrono::microseconds(1000 * 1000 / config.fpsLimit);

    while (!exit)
    {
        std::chrono::time_point<std::chrono::steady_clock> currentTime = std::chrono::steady_clock::now();
        currentExecutionTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - applicationInitTime);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            imgui->InputEvent(event);
            InputEvent(event);
        }

        renderer->StartFrame();
        imgui->StartFrame();

        Tick(frameDelta);

        renderer->DrawBatches();

        imgui->EndFrame();
        renderer->EndFrame();

        std::chrono::time_point<std::chrono::steady_clock> frameEndTime = std::chrono::steady_clock::now();
        frameDelta = std::chrono::duration_cast<std::chrono::microseconds>(frameEndTime - currentTime);
        frameDelta = std::clamp(frameDelta, std::chrono::microseconds(0), frameTimeLimit);
    }

    Cleanup();
}

void Application::QuitApplication()
{
    exit = true;
}

const std::chrono::microseconds& Application::GetApplicationExecutionTime() const
{
    return currentExecutionTime;
}

void Application::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        throw std::runtime_error("Error: " + std::string(SDL_GetError()));
    }

    SDL_WindowFlags window_flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);

    renderer = new Renderer();
    renderer->Init(window);

    imgui = new Imgui();
    imgui->Init(renderer, window);

    applicationInitTime = std::chrono::steady_clock::now();
}

void Application::Cleanup()
{
    imgui->Cleanup();
    delete imgui;
    imgui = nullptr;

    renderer->Cleanup();
    delete renderer;
    renderer = nullptr;

    SDL_DestroyWindow(window);
    SDL_Quit();
}

