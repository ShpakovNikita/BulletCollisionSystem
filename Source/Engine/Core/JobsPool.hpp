#pragma once

#include "IEngineSubsystem.hpp"

#include <vector>
#include <functional>
#include <mutex>

namespace std { class thread; }

class JobsPool
    : public IEngineSubsystem
{
public:
    void CreateBackgroundJob(const std::function<void()>& jobFunction);

    void Init() override;
    void Cleanup() override;
    void EndFrame() override;
    void StartFrame() override;

    size_t GetBackgroundThreadsCount();

private:
    void ThreadBackgroundFunc();

    std::vector<std::thread> backgroundThreads;
    std::list<std::function<void()>> jobsQueue;

    std::mutex mutex;
    std::condition_variable conditionVariable;

    volatile bool terminateThreads = false;

    friend class Application;
};