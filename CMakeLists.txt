cmake_minimum_required(VERSION 3.24)
project(primos_jayme)

set(CMAKE_CXX_STANDARD 17)

add_executable(primos_jayme main.cpp)

set(CMAKE_CXX_FLAGS "-static-libstdc++ -static-libgcc --static")
