#pragma once
#include <string>
#include <stdint.h>
#include <future>
#include <vector>
#include "Math/Vector3.hpp"

struct SDL_Window;
struct Vector2;

class Renderer
{
public:
    void DrawLine(const Vector2& startPoint, const Vector2& endPoint);

    inline void* GetApiContext() const { return apiContext; }
    inline const std::string& GetApiVersion() const { return apiVersion; }

private:
    struct BatchInfo
    {
        uint32_t drawType;
        std::vector<Vector3> vertices;
        std::vector<uint32_t> indices;

        uint32_t VAO = 0, VBO = 0, EBO = 0;
    };

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

    std::vector<BatchInfo> batches;

    friend class Application;
};