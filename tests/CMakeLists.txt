cmake_minimum_required(VERSION 3.8)

set(This runTests)

set(CMAKE_CXX_STANDARD 14)

set(Src
        ThreadPoolTests.cpp
    )

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(${This} ${Src})

# Link runTests with what we want to test and the GTest and pthread library
target_link_libraries(${This} 
                        ${GTEST_BOTH_LIBRARIES}
                        gmock
                        pthread
                        ThreadPool
                     )

add_test(
    NAME ${This}
    COMMAND ${This}
)