#ifndef THREAD_POOL_H_INC
#define THREAD_POOL_H_INC

#include <memory>
#include <functional>

namespace ThreadPool{
    class ThreadPool{
        public:
            ThreadPool();
            ~ThreadPool();
            ThreadPool(const ThreadPool&) = delete;
            ThreadPool& operator=(const ThreadPool&) = delete;
            ThreadPool(ThreadPool&&) = delete;
            ThreadPool& operator=(ThreadPool&&) = delete;

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
            void enqueueTask(std::function<void()>);
        private:
            struct impl;
            std::unique_ptr<impl> pimpl;
    };
}

#endif //THREAD_POOL_H_INC