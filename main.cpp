#include <iostream>

#include <ThreadPool/ThreadPool.hpp>



int main(){

    std::cout << "Testing app!" << std::endl;
    char Input;
    ThreadPool::ThreadPool threadPool;

    std::cout << "Max thread num: " << threadPool.getMaxThreadCount() << std::endl;

    threadPool.create(4);

    std::cin >> Input;

    threadPool.destroy();

    return 0;
}