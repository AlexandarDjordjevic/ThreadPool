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
        auto enqueueTask(F &&f, Args &&... args)
        {
            typedef decltype(f(args...)) retType;
            std::packaged_task<retType()> task(std::move(std::bind(f, args...)));
            std::unique_lock<std::mutex> lock(mutex);

            std::future<retType> future = task.get_future();
            taskQueue.push(std::make_shared<Task<retType>>(std::move(task)));
            condition.notify_one();

            return future;
        }

    private:
        class ITask
        {
        private:
            std::packaged_task<void()> func;

        public:
            virtual ~ITask(){};
            virtual void execute() = 0;
        };
        template <typename RetType>
        class Task : public ITask
        {
        private:
            std::packaged_task<RetType()> func;

        public:
            virtual ~Task() {}
            explicit Task(std::packaged_task<RetType()> func) : func(std::move(func)) {}
            void execute()
            {
                func();
            }
        };

        std::vector<std::thread> workers;
        bool terminate;
        std::condition_variable condition;
        std::mutex mutex;
        std::queue<std::shared_ptr<ITask>> taskQueue;
    };
} // namespace ThreadPool

#endif //THREAD_POOL_H_INC