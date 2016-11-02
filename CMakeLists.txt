cmake_minimum_required(VERSION 3.6)
project(hungry_crows)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)
add_executable(hungry_crows ${SOURCE_FILES})
target_link_libraries(hungry_crows pthread)
