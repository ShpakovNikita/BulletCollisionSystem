#include "Core/AppContext.hpp"
#include "Core/Renderer.hpp"
#include "Core/Imgui.hpp"

AppContext::AppContext(const ApplicationConfig& aConfig)
    : config(aConfig)
{
    renderer = std::make_unique<Renderer>(*this);
    imgui = std::make_unique<Imgui>(*this);
}

AppContext::~AppContext() = default;

const std::chrono::microseconds& AppContext::GetApplicationExecutionTime() const
{
    return currentExecutionTime;
}

