#include "Core/Renderer.hpp"
#include "SDL_video.h"
#include "GL/gl3w.h"
#include <stdexcept>
#include <iostream>
#include "Utils/File.hpp"
#include <future>
#include <thread>

std::mutex g_display_mutex;

void Renderer::DrawLine()
{

}

void Renderer::Init(SDL_Window* aWindow)
{
    window = aWindow;

    // TODO: load shaders from Client
    std::string vertexShaderPath = std::string(ASSETS_DIR) + std::string("Shaders/Triangle.vert");
    std::string fragmentShaderPath = std::string(ASSETS_DIR) + std::string("Shaders/Triangle.frag");

    std::future<std::string> vertexShaderSource = std::async(std::launch::async, &File::ReadFile, std::ref(vertexShaderPath));
    std::future<std::string> fragmentShaderSource = std::async(std::launch::async, &File::ReadFile, std::ref(fragmentShaderPath));

    // Decide GL+GLSL versions
    // GL 3.0 + GLSL 330
    apiVersion = "#version 330";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

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

    shaderProgramHandle = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
    OpengGlPrepareTestData();
}

void Renderer::Cleanup()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    SDL_GL_DeleteContext(apiContext);

    window = nullptr;
}

void Renderer::EndFrame()
{
    SDL_GL_SwapWindow(window);
}

void Renderer::StartFrame()
{
    const float clearColor[] = { 0.45f, 0.55f, 0.60f, 1.00f };

    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    
    glViewport(0, 0, width, height);
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::DrawBatches()
{
    glUseProgram(shaderProgramHandle);
    glBindVertexArray(VAO);

    glDrawElements(GL_LINE_STRIP, 8, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::OpengGlPrepareTestData()
{
    GLfloat vertices[] = {
          0.5f,  0.5f, 0.0f,  // Top Right
          0.5f, -0.5f, 0.0f,  // Bottom Right
         -0.5f, -0.5f, 0.0f,  // Bottom Left
         -0.5f,  0.5f, 0.0f,   // Top Left 
    };
    GLuint indices[] = {
        0, 1, 3, 0,
        1, 2, 3, 1,
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
   
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

GLuint Renderer::CreateShaderProgram(std::future<std::string>& vertexShaderSource, std::future<std::string>& fragmentShaderSource)
{
    GLuint vertexShader = CompileShader(vertexShaderSource.get(), GL_VERTEX_SHADER);
    GLuint fragmentShader = CompileShader(fragmentShaderSource.get(), GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        throw std::runtime_error("Shader linking failed! " + std::string(infoLog));
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

uint32_t Renderer::CompileShader(const std::string& shaderSource, uint32_t shaderType)
{
    GLuint shader;
    shader = glCreateShader(shaderType);

    const char* source = shaderSource.c_str();
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        throw std::runtime_error("Shader compilation failed! " + std::string(infoLog));
    }

    return shader;
}

