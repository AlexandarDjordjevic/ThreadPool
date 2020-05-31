#ifndef THREAD_POOL_H_INC
#define THREAD_POOL_H_INC

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>

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


            void destroy();
        private:
            std::vector<std::thread> mPool;
            bool terminate;
            std::condition_variable condition;
            std::mutex mutex;
            void threadFunction(void);


    };
}

#endif //THREAD_POOL_H_INC