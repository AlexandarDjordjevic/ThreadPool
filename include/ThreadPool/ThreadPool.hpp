#ifndef THREAD_POOL_H_INC
#define THREAD_POOL_H_INC

#include <memory>
#include <future>
#include <functional>

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
        void enqueueTask(F &&func, Args... args)
        {
            // std::packaged_task<void()> task(std::bind(func, args...));
            // std::unique_lock<std::mutex> lock(pimpl->mutex);
            // pimpl->taskQueue.push(std::make_shared<Task>(std::move(task)));
            // pimpl->condition.notify_one();
            //int dummy[sizeof...(Args)] = {(args, 0)...};
        }

    private:
        struct impl;
        std::unique_ptr<impl> pimpl;

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
    };
} // namespace ThreadPool

#endif //THREAD_POOL_H_INC