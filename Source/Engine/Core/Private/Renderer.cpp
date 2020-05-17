#include "Core/Renderer.hpp"
#include "SDL_video.h"
#include "GL/gl3w.h"
#include <stdexcept>
#include <iostream>
#include "Utils/File.hpp"
#include <future>
#include <thread>
#include "Math/Vector2.hpp"
#include "Core/AppContext.hpp"

Renderer::Renderer(const AppContext& aAppContext)
    : appContext(aAppContext)
{}

void Renderer::DrawLines(const std::vector<Vector2>& linePoints, const Vector3& color /*= Vector3::kGreen*/, bool drawAsStrip /*= false*/) const
{
    BatchInfo batchInfo;
    batchInfo.persistent = false;
    batchInfo.drawType = drawAsStrip ? GL_LINE_STRIP : GL_LINES;
    batchInfo.vertices.reserve(linePoints.size());

    for (const Vector2& linePoint : linePoints)
    {
        BatchInfo::Vertex vertex;
        vertex.color = color;
        vertex.position = { linePoint.x, linePoint.y, 0.0f };
        batchInfo.vertices.push_back(std::move(vertex));
    }

    BindBuffersForBatch(batchInfo);

    batches.push_back(std::move(batchInfo));
}

void Renderer::DrawLine(const Vector2& startPoint, const Vector2& endPoint, const Vector3& color /*= Vector3::kGreen*/) const
{
    BatchInfo batchInfo;
    batchInfo.persistent = false;
    batchInfo.drawType = GL_LINES;

    BatchInfo::Vertex lineStart;
    lineStart.color = color;
    lineStart.position = { startPoint.x, startPoint.y, 0.0f };

    BatchInfo::Vertex lineEnd;
    lineEnd.color = color;
    lineEnd.position = { endPoint.x, endPoint.y, 0.0f };

    batchInfo.vertices = { lineStart, lineEnd };

    // Sure it is better to use one VAO for all the lines, but it isn't the purpose of this test task
    BindBuffersForBatch(batchInfo);

    batches.push_back(std::move(batchInfo));
}

void Renderer::DrawPoint(const Vector2& position, float radius, const Vector3& color /*= Vector3::kGreen*/, uint32_t segmentsCount /*= 8*/) const
{
    BatchInfo batchInfo;
    batchInfo.persistent = false;
    batchInfo.drawType = GL_TRIANGLE_FAN;

    batchInfo.vertices.reserve(segmentsCount);

    for (uint32_t i = 0; i < segmentsCount; ++i)
    {
        const float alpha = 2.0f * static_cast<float>(M_PI) * i / segmentsCount;
        Vector3 fanVertexPosition = { 
            position.x + radius * std::cosf(alpha),
            position.y + radius * std::sinf(alpha),
            0.0f };

        BatchInfo::Vertex vertex;
        vertex.color = color;
        vertex.position = fanVertexPosition;

        batchInfo.vertices.push_back(std::move(vertex));
    }

    // Sure it is better to use one VAO for all the points, but it isn't the purpose of this test task
    BindBuffersForBatch(batchInfo);

    batches.push_back(std::move(batchInfo));
}

void Renderer::BindBuffersForBatch(BatchInfo& batchInfo)
{
    glGenVertexArrays(1, &batchInfo.VAO);
    glBindVertexArray(batchInfo.VAO);

    glGenBuffers(1, &batchInfo.VBO);

    glBindBuffer(GL_ARRAY_BUFFER, batchInfo.VBO);
    glBufferData(GL_ARRAY_BUFFER, batchInfo.vertices.size() * sizeof(BatchInfo::Vertex), batchInfo.vertices.data(), GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Renderer::Init()
{
    std::string vertexShaderPath = std::string(ENGINE_SHADERS_DIR) + std::string("Solid.vert");
    std::string fragmentShaderPath = std::string(ENGINE_SHADERS_DIR) + std::string("Solid.frag");

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

    apiContext = SDL_GL_CreateContext(appContext.window);
    SDL_GL_MakeCurrent(appContext.window, apiContext);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    if (gl3wInit() != GL3W_OK)
    {
        throw std::runtime_error("Failed to initialize OpenGL loader!");
    }

    simpleShaderProgramHandle = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}

void Renderer::Cleanup()
{
    CleanupBatches(true);

    SDL_GL_DeleteContext(apiContext);
}

void Renderer::EndFrame()
{
    SDL_GL_SwapWindow(appContext.window);

    CleanupBatches(false);
}

void Renderer::StartFrame()
{
    const float clearColor[] = { 0.45f, 0.55f, 0.60f, 1.00f };

    glViewport(0, 0, appContext.config.viewportWidth, appContext.config.viewportHeight);
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::DrawBatches()
{
    glUseProgram(simpleShaderProgramHandle);

    // No queue sorting and draw calls optimization provided here, because this is a sample renderer
    for (const BatchInfo& batchInfo : batches)
    {
        glBindVertexArray(batchInfo.VAO);
        
        glDrawArrays(batchInfo.drawType, 0, static_cast<GLsizei>(batchInfo.vertices.size()));

        glBindVertexArray(0);
    }
}

void Renderer::CleanupBatches(bool cleanupPersistentBatches)
{
    auto removeBatchConditionsCheck = [cleanupPersistentBatches](const BatchInfo& batchInfo) {
        return !batchInfo.persistent || cleanupPersistentBatches;
    };

    for (const BatchInfo& batchInfo : batches)
    {
        if (removeBatchConditionsCheck(batchInfo))
        {
            glDeleteVertexArrays(1, &batchInfo.VAO);
            glDeleteBuffers(1, &batchInfo.VBO);
        }
    }

    batches.erase(std::remove_if(batches.begin(), batches.end(), removeBatchConditionsCheck), batches.end());
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

