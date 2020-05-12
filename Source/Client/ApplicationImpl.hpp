#pragma once
#include "Core\Application.hpp"

class ApplicationImpl final : public Application
{
private:
    void Tick() override;
    void Render() override;

    bool showDemoWindow = true;
};
