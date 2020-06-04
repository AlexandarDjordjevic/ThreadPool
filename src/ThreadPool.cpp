#include <ThreadPool/ThreadPool.hpp>

namespace ThreadPool
{

    ThreadPool::ThreadPool()
    {
        terminate = false;
    }

    ThreadPool::~ThreadPool()
    {
        destroy();
    };

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
                    std::shared_ptr<ITask> task;
                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        condition.wait(lock, [this]() { return terminate || !taskQueue.empty(); });
                        if (terminate)
                        {
                            break;
                        }
                        else
                        {
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

} // namespace ThreadPool
