#pragma once
#include <string>
#include <stdint.h>
#include <future>

struct SDL_Window;

class Renderer
{
public:
    void DrawLine();

    inline void* GetApiContext() const { return apiContext; }
    inline const std::string& GetApiVersion() const { return apiVersion; }

private:
    void Init(SDL_Window* window);
    void Cleanup();
    void EndFrame();
    void StartFrame();
    void DrawBatches();

    void OpengGlPrepareTestData();
    uint32_t CreateShaderProgram(std::future<std::string>& vertexShaderSource, std::future<std::string>& fragmentShaderSource);
    uint32_t CompileShader(const std::string& shaderSource, uint32_t shaderType);

    SDL_Window* window = nullptr;
    mutable void* apiContext = nullptr;
    std::string apiVersion = {};

    uint32_t shaderProgramHandle = 0;
    uint32_t VAO = 0, VBO = 0, EBO = 0;

    friend class Application;
};