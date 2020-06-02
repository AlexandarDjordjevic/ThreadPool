#include <ThreadPool/ThreadPool.hpp>

#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <condition_variable>

//Remove cout
#include <iostream>

namespace ThreadPool
{

    struct ThreadPool::impl
    {
        std::vector<std::thread> workers;
        bool terminate;
        std::condition_variable condition;
        std::mutex mutex;
        std::queue<std::shared_ptr<Task>> taskQueue;
    };

    ThreadPool::ThreadPool()
        : pimpl(new ThreadPool::impl)
    {
        pimpl->terminate = false;
    }

    ThreadPool::~ThreadPool() = default;

    size_t ThreadPool::getMaxThreadCount()
    {
        return std::thread::hardware_concurrency();
    }

    void ThreadPool::create(size_t threadCount)
    {
        int threadId = 0;
        for (size_t i = 0; i < threadCount; i++)
        {
            pimpl->workers.push_back(std::thread([&, threadId]() {
                while (true)
                {
                    std::shared_ptr<Task> task;
                    {
                        std::unique_lock<std::mutex> lock(pimpl->mutex);
                        pimpl->condition.wait(lock, [this]() { return pimpl->terminate || !pimpl->taskQueue.empty(); });
                        if (pimpl->terminate)
                        {
                            std::cout << "Terminating thread ID: " << threadId << std::endl;
                            break;
                        }
                        else
                        {
                            std::cout << "Unlocked thread (ID - " << threadId << ")" << std::endl;
                            task = pimpl->taskQueue.front();
                            pimpl->taskQueue.pop();
                        }
                    }
                    (*task).execute();
                }
            }));
            threadId++;
        }
    }

    void ThreadPool::destroy()
    {
        {
            std::unique_lock<std::mutex> lock(pimpl->mutex);
            pimpl->terminate = true;
            pimpl->condition.notify_all();
        }

        for (auto &thread : pimpl->workers)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }

        pimpl->workers.clear();
    }

      void ThreadPool::enqueueTask1(func_t funk, char x, int y)
    {
        funk(x, y);
    }

} // namespace ThreadPool
