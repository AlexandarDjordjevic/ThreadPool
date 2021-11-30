cmake_minimum_required(VERSION 3.8)

set(this ThreadPool)

project(${this})

set(CMAKE_CXX_STANDARD 14)

set(Src 
    src/ThreadPool.cpp
)

add_library(${this} STATIC ${Src})

target_include_directories(${this} PUBLIC include)

target_link_libraries(${this} pthread)

add_subdirectory(tests)

