#include <ThreadPool/ThreadPool.hpp>

//Remove cout
#include <iostream>

namespace ThreadPool
{

    ThreadPool::ThreadPool()
    {
        terminate = false;
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
            workers.push_back(std::thread([&, threadId]() {
                while (true)
                {
                    std::shared_ptr<Task> task;
                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        condition.wait(lock, [this]() { return terminate || !taskQueue.empty(); });
                        if (terminate)
                        {
                            std::cout << "Terminating thread ID: " << threadId << std::endl;
                            break;
                        }
                        else
                        {
                            std::cout << "Unlocked thread (ID - " << threadId << ")" << std::endl;
                            task = taskQueue.front();
                            taskQueue.pop();
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
            std::unique_lock<std::mutex> lock(mutex);
            terminate = true;
            condition.notify_all();
        }

        for (auto &thread : workers)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }

        workers.clear();
    }

    // void ThreadPool::enqueueTask1(func_t funk, char x, int y)
    // {
    //     funk(x, y);
    // }

} // namespace ThreadPool
