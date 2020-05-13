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


void Application::Run()
{
    Init();

    while (!exit)
    {
        renderer->StartFrame();
        imgui->StartFrame();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            InputEvent(event);
        }

        Tick();
        Render();

        renderer->DrawBatches();

        imgui->EndFrame();
        renderer->EndFrame();
    }

    Cleanup();
}

void Application::QuitApplication()
{
    exit = true;
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

