#pragma once
#include "Core\Application.hpp"

class ApplicationImpl final : public Application
{
private:
    void Tick() override;
    void Render() override;
    void InputEvent(const SDL_Event& event) override;
    std::string GetAssetsDir() override;

    bool showDemoWindow = true;
};
