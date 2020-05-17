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
    void DrawLines(const std::vector<Vector2>& linePoints, const Vector3& color = Vector3::kGreen, bool drawAsStrip = false) const;
    void DrawLine(const Vector2& startPoint, const Vector2& endPoint, const Vector3& color = Vector3::kGreen) const;
    void DrawPoint(const Vector2& position, float radius, const Vector3& color = Vector3::kGreen, uint32_t segmentsCount = 8) const;

    inline void* GetApiContext() const { return apiContext; }
    inline const std::string& GetApiVersion() const { return apiVersion; }

private:
    struct BatchInfo
    {
        struct Vertex
        {
            Vector3 position;
            Vector3 color;
        };

        uint32_t drawType;
        std::vector<Vertex> vertices;

        uint32_t VAO = 0, VBO = 0;
        bool persistent = false;
    };

    static void BindBuffersForBatch(BatchInfo& batchInfo);

    void Init(SDL_Window* window);
    void Cleanup();
    void EndFrame();
    void StartFrame();
    void DrawBatches();
    void CleanupBatches(bool cleanupPersistentBatches);

    uint32_t CreateShaderProgram(std::future<std::string>& vertexShaderSource, std::future<std::string>& fragmentShaderSource);
    uint32_t CompileShader(const std::string& shaderSource, uint32_t shaderType);

    SDL_Window* window = nullptr;
    mutable void* apiContext = nullptr;
    std::string apiVersion = {};

    uint32_t simpleShaderProgramHandle = 0;

    mutable std::vector<BatchInfo> batches;

    friend class Application;
};