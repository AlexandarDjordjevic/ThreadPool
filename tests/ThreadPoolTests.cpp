#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <ThreadPool/ThreadPool.hpp>

#include <iostream>

namespace{
    int task0Var = 0;
    void task0(){task0Var = 1;}
    int task1(int x){return x;};
}

TEST(ThreadPool, TestVoidFuncVoid){
    ThreadPool::ThreadPool threadPool;
    threadPool.create(1);
    auto future = threadPool.enqueueTask(task0);
    future.get();
    EXPECT_EQ(task0Var, 1);
}

TEST(ThreadPool, TestIntFuncInt){
    ThreadPool::ThreadPool threadPool;
    threadPool.create(5);
    auto future = threadPool.enqueueTask(task1, 1);
    int result = future.get();
    EXPECT_EQ(result, 1);
}