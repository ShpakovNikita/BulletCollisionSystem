#include "Core/Imgui.hpp"
#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_sdl.h"
#include "Core/Renderer.hpp"

void Imgui::Init(Renderer* renderer, SDL_Window* aWindow)
{
    window = aWindow;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, renderer->GetApiContext());
    ImGui_ImplOpenGL3_Init(renderer->GetApiVersion().c_str());
}

void Imgui::Cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    window = nullptr;
}

void Imgui::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Imgui::StartFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();
}
