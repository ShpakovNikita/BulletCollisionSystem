#include "Core/Imgui.hpp"
#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_sdl.h"
#include "Core/Renderer.hpp"
#include "Core/AppContext.hpp"

Imgui::Imgui(const AppContext& aAppContext)
    : appContext(aAppContext)
{

}

void Imgui::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(appContext.window, appContext.renderer->GetApiContext());
    ImGui_ImplOpenGL3_Init(appContext.renderer->GetApiVersion().c_str());
}

void Imgui::InputEvent(const SDL_Event& event)
{
    ImGui_ImplSDL2_ProcessEvent(&event);
}

void Imgui::Cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void Imgui::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Imgui::StartFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(appContext.window);
    ImGui::NewFrame();
}
