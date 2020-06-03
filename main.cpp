#include <iostream>

#include <ThreadPool/ThreadPool.hpp>

#include <thread>

void Task1(char x, int y)
{
    int i = 0;
    while (i < 5)
    {
        i++;
        std::cout << "This is task 1. X: " << x << " Y:" << y << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void Task2(std::string msg)
{
    int i = 0;
    while (i < 5)
    {
        i++;
        std::cout << "This is task 2. Msg: " << msg << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}

void Task3()
{
    int i = 0;
    while (i < 5)
    {
        i++;
        std::cout << "This is task 3." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main()
{

    std::cout << "Testing app!" << std::endl;
    char Input;
    ThreadPool::ThreadPool threadPool;

    std::cout << "Max thread num: " << threadPool.getMaxThreadCount() << std::endl;

    threadPool.create(4);

    threadPool.enqueueTask(Task1, '1', 2);
    threadPool.enqueueTask(Task2, "Hello World");
    //threadPool.enqueueTask(Task2, "Hello World");

    std::cin >>
        Input;

    threadPool.destroy();

    return 0;
}
