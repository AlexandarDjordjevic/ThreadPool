#ifndef THREAD_POOL_H_INC
#define THREAD_POOL_H_INC

#include <cstddef>
#include <memory>
#include <future>
#include <functional>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <condition_variable>

namespace utils {
class ThreadPool {
public:
    ThreadPool();
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    /**
     * @brief This funcition create requested number of threads
     *
     * @param thread_count
     */
    void create(size_t thread_count);

    /**
     * @brief This function is called when u want to stop all thread and
     *         clear thread pool.
     *
     */
    void stop();

    /**
     * @brief Add new task to the task queue
     *
     */

public:
    template<typename F, typename... Args>
    auto enqueue_task(F&& f, Args&&... args) {
        typedef decltype(f(args...)) ret_type;
        std::packaged_task<ret_type()> task(std::bind(f, args...));
        std::unique_lock<std::mutex> lock(m_mtx);

        auto future{task.get_future()};
        m_task_queue.push(std::make_shared<Task<ret_type>>(std::move(task)));
        m_cv.notify_one();

        return future;
    }

private:
    class ITask {
    private:
        std::packaged_task<void()> func;

    public:
        virtual ~ITask(){};
        virtual void execute() = 0;
    };

    template<typename RetType>
    class Task : public ITask {
    private:
        std::packaged_task<RetType()> func;

    public:
        virtual ~Task() {}
        explicit Task(std::packaged_task<RetType()> func) : func(std::move(func)) {}
        void execute() { func(); }
    };

private:
    bool m_terminate;
    std::vector<std::thread> m_workers;
    std::condition_variable m_cv;
    std::mutex m_mtx;
    std::queue<std::shared_ptr<ITask>> m_task_queue{};
};

} // namespace utils

#endif // THREAD_POOL_H_INC