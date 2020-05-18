#pragma once
#include <vector>
#include <functional>
#include <mutex>
#include <atomic>

namespace std { class thread; }

class JobsPool
{
public:
    void CreateBackgroundJob(const std::function<void()>& jobFunction);

private:
    void Init();
    void Cleanup();

    void ThreadBackgroundFunc();

    std::vector<std::thread> backgroundThreads;
    std::list<std::function<void()>> jobsQueue;

    std::mutex mutex;
    std::condition_variable conditionVariable;

    volatile bool terminateThreads = false;

    friend class Application;
};