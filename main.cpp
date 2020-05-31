#include <iostream>

#include <ThreadPool/ThreadPool.hpp>

#include <thread>

void Task1(){
    int i = 0;
    while (i < 5){
        i++;
        std::cout << "This is task 1." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void Task2(){
    int i = 0;
    while (i < 5){
        i++;
        std::cout << "This is task 2." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
    
}

int main(){

    std::cout << "Testing app!" << std::endl;
    char Input;
    ThreadPool::ThreadPool threadPool;

    std::cout << "Max thread num: " << threadPool.getMaxThreadCount() << std::endl;

    threadPool.create(4);

    threadPool.enqueueTask(Task1);
    threadPool.enqueueTask(Task2);
    std::cin >> Input;

    threadPool.destroy();

    return 0;
}