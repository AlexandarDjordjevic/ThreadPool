cmake_minimum_required(VERSION 3.8)

set(this ThreadPool)

project(${this})

set(CMAKE_CXX_STANDARD 14)

set(Src 
    src/ThreadPool.cpp
)

set(Include 
    include/ThreadPool/ThreadPool.hpp
)

#add_library(${this} SHARED main.cpp ${Src})
add_executable(${this} main.cpp ${Src})

set_target_properties(${this} PROPERTIES
    FOLDER Libraries
)

target_include_directories(${this} PUBLIC include)

target_link_libraries(${this} pthread)

