#ifndef THREAD_POOL_H_INC
#define THREAD_POOL_H_INC

#include <memory>
#include <future>
#include <functional>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <condition_variable>
namespace ThreadPool
{
    class ThreadPool
    {
    public:
        ThreadPool();
        ~ThreadPool();
        ThreadPool(const ThreadPool &) = delete;
        ThreadPool &operator=(const ThreadPool &) = delete;
        ThreadPool(ThreadPool &&) = delete;
        ThreadPool &operator=(ThreadPool &&) = delete;

        /**
             * @brief Get the number of maximal available threads
             * 
             * @return number of threads 
             */
        size_t getMaxThreadCount(void);

        /**
             * @brief This funcition create requested number of threads
             * 
             * @param threadCount 
             */
        void create(size_t threadCount);

        /**
             * @brief This function is called when u want to stop all thread and 
             *         clear thread pool.
             * 
             */
        void destroy();

        /**
             * @brief Add new task to the task queue
             * 
             */

        template <typename F, typename... Args>
        void enqueueTask(F &&f, Args &&... args)
        {
            std::packaged_task<void()> task(std::move(std::bind(f, args...)));
            std::unique_lock<std::mutex> lock(mutex);
            taskQueue.push(std::make_shared<Task>(std::move(task)));
            condition.notify_one();
        }

    private:
        class Task
        {
        private:
            std::packaged_task<void()> func;

        public:
            virtual ~Task() {}
            explicit Task(std::packaged_task<void()> func) : func(std::move(func)) {}
            void execute()
            {
                func();
            }
        };

        std::vector<std::thread> workers;
        bool terminate;
        std::condition_variable condition;
        std::mutex mutex;
        std::queue<std::shared_ptr<Task>> taskQueue;
    };
} // namespace ThreadPool

#endif //THREAD_POOL_H_INC