#pragma once

class IEngineSubsystem
{
public:
    virtual ~IEngineSubsystem() = default;

    virtual void Init() = 0;
    virtual void Cleanup() = 0;
    virtual void EndFrame() = 0;
    virtual void StartFrame() = 0;
};