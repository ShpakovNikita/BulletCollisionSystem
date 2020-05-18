#include "Core/AppContext.hpp"
#include "Core/Renderer.hpp"
#include "Core/Imgui.hpp"
#include "Core/JobsPool.hpp"

AppContext::AppContext(const ApplicationConfig& aConfig)
    : config(aConfig)
{
    renderer = std::make_unique<Renderer>(*this);
    imgui = std::make_unique<Imgui>(*this);
    jobsPool = std::make_unique<JobsPool>();
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

