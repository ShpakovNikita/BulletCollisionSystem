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

    ImGuiIO& io = ImGui::GetIO();
    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        Tick();

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);

        Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    Cleanup();
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

