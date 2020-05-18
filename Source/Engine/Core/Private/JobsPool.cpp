#include "Core/JobsPool.hpp"
#include <thread>
#include <functional>
#include <optional>

void JobsPool::CreateBackgroundJob(const std::function<void()>& jobFunction)
{
    {
        std::lock_guard<std::mutex> lock(mutex);
        jobsQueue.push_back(jobFunction);
    }
    conditionVariable.notify_one();
}

void JobsPool::Init()
{
    // This function can return zero even if there is available threads, 
    // all the code here do not covering all platform cases
    uint32_t maxThreadsCount = std::thread::hardware_concurrency();
    for (uint32_t i = 0; i < maxThreadsCount; ++i)
    {
        backgroundThreads.push_back(std::thread(std::bind(&JobsPool::ThreadBackgroundFunc, this)));
    }
}

void JobsPool::Cleanup()
{
    terminateThreads = true;
    conditionVariable.notify_all();

    for (std::thread& backgroundThread : backgroundThreads)
    {
        backgroundThread.join();
    }

    backgroundThreads.clear();
}

void JobsPool::ThreadBackgroundFunc()
{
    bool breakThreadLoop = false;
    while (!breakThreadLoop)
    {
        std::optional<std::function<void()>> job = std::nullopt;

        {
            std::unique_lock<std::mutex> lock(mutex);

            conditionVariable.wait(lock, [this] {return !jobsQueue.empty() || terminateThreads; });

            if (!jobsQueue.empty())
            {
                job = jobsQueue.front();
                jobsQueue.pop_front();
            }

            breakThreadLoop = terminateThreads && jobsQueue.empty();
        }
        
        if (job)
        {
            (*job)();
        }
    }
}
