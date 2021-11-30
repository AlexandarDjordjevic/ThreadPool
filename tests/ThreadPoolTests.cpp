#include <gmock/gmock-actions.h>
#include <gmock/gmock-function-mocker.h>
#include <gmock/gmock-spec-builders.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <utils/ThreadPool.hpp>

#include <iostream>

class MockObject {
public:
    MOCK_METHOD(void, do_stuff_1, ());
    MOCK_METHOD(void, do_stuff_2, (int));
};

class TestThreadPool : public testing::Test {
public:
    utils::ThreadPool threadPool;
};

TEST_F(TestThreadPool, TestVoidFuncVoid) {
    MockObject obj;
    threadPool.create(1);
    EXPECT_CALL(obj, do_stuff_1);
    auto future = threadPool.enqueue_task([&obj]() { obj.do_stuff_1(); });
    future.get();
}

TEST_F(TestThreadPool, TestIntFuncInt) {
    MockObject obj;
    threadPool.create(2);
    EXPECT_CALL(obj, do_stuff_1);
    EXPECT_CALL(obj, do_stuff_2(10));
    auto future_1 = threadPool.enqueue_task([&obj]() { obj.do_stuff_1(); });
    auto future_2 = threadPool.enqueue_task([&obj](int x) { obj.do_stuff_2(x); }, 10);
    future_1.get();
    future_2.get();
}