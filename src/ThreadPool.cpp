#include <ThreadPool/ThreadPool.hpp>

#include <iostream>

namespace ThreadPool
{

    ThreadPool::ThreadPool(){
        terminate = false;
    }

    ThreadPool::~ThreadPool() = default;

    size_t ThreadPool::getMaxThreadCount(){
        return std::thread::hardware_concurrency();
    }

    void ThreadPool::create(size_t threadCount){
        int threadId = 0;
        for (size_t i = 0; i < threadCount; i++){         
            this->mPool.push_back(std::thread([&, threadId](){
                while(true){
                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        condition.wait(lock, [this](){return terminate;});
                        if(terminate){
                            std::cout << "Terminating thread ID: " << threadId << std::endl;
                            break;
                        }
                        std::cout << "Unlocked thread (ID - " << threadId << ")"<<std::endl;
                    }  
                    
                }
            }));
            threadId++;
        }
    }

    void ThreadPool::destroy(){
        {
            std::unique_lock<std::mutex> lock(mutex);
            terminate = true;
            condition.notify_all();
        }
    
        for(auto &thread : mPool){
            if (thread.joinable()) {
                thread.join();
            }
        }

        mPool.clear();
    }

} // namespace ThreadPool

