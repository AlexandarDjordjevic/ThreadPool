#include <mutex>
#include <thread>
#include <utility>
#include <utils/ThreadPool.hpp>

namespace utils {

ThreadPool::ThreadPool() : m_terminate{false} {}

ThreadPool::~ThreadPool() { stop(); }

void ThreadPool::create(size_t thread_count) {
    m_workers.resize(thread_count);
    for (auto i{0}; i < thread_count; i++) {
        m_workers.emplace_back([this]() {
            while (true) {
                auto terminate{false};
                std::shared_ptr<ITask> task;

                auto get_task{[this]() -> std::pair<bool, std::shared_ptr<ITask>> {
                    std::unique_lock<std::mutex> lock(m_mtx);
                    m_cv.wait(lock, [this]() { return m_terminate || !m_task_queue.empty(); });
                    if (m_terminate) {
                        return {true, nullptr};
                    }
                    auto task{m_task_queue.front()};
                    m_task_queue.pop();
                    return {false, task};
                }};

                std::tie(terminate, task) = get_task();

                if (terminate) {
                    break;
                }
                task->execute();
            }
        });
    }
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_terminate = true;
        m_cv.notify_all();
    }

    for (auto& thread : m_workers) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    m_workers.clear();
}


} // namespace utils
