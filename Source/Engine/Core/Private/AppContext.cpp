#include "Core/AppContext.hpp"

#include "Core/Renderer.hpp"
#include "Core/Imgui.hpp"
#include "Core/JobsPool.hpp"

#include <filesystem>
#include <stdexcept>

AppContext::AppContext(const ApplicationConfig& aConfig)
    : config(aConfig)
{
    renderer = std::make_unique<Renderer>(*this);
    imgui = std::make_unique<Imgui>(*this);
    jobsPool = std::make_unique<JobsPool>();

    // Instead of fully functional engine resource system, here is a simple solution for packaging
    // and running project via IDE
    if (std::filesystem::exists(ENGINE_SHADERS_DIR))
    {
        shadersDir = std::string(ENGINE_SHADERS_DIR);
    }
    else if (std::filesystem::exists(RESOURCE_SHADERS_DIR))
    {
        shadersDir = std::string(RESOURCE_SHADERS_DIR);
    }
    else
    {
        throw std::runtime_error("Shaders folder not found!");
    }
}

AppContext::~AppContext() = default;

const std::chrono::microseconds& AppContext::GetApplicationExecutionTime() const
{
    return currentExecutionTime;
}

const float AppContext::GetApplicationExecutionTimeSec() const
{
    return currentExecutionTime.count() / (1000.0f * 1000.0f);
}

const float AppContext::GetApplicationExecutionTimeMs() const
{
    return currentExecutionTime.count() / 1000.0f;
}

const std::string& AppContext::GetShadersDir() const
{
    return shadersDir;
}

