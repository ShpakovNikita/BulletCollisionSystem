#include "Core/Renderer.hpp"
#include "SDL_video.h"
#include "GL/gl3w.h"
#include <stdexcept>

void Renderer::DrawLine()
{

}

void Renderer::Init(SDL_Window* window)
{
    // Decide GL+GLSL versions
    // GL 3.0 + GLSL 130
    apiVersion = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    apiContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, apiContext);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    bool err = gl3wInit() != 0;
    if (err)
    {
        throw std::runtime_error("Failed to initialize OpenGL loader!");
    }
}

void Renderer::Cleanup()
{
    SDL_GL_DeleteContext(apiContext);
}

void Renderer::SubmitFrame()
{

}

