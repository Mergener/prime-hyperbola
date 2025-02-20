cmake_minimum_required(VERSION 3.10)
project(primes_hyperbola)

set(CMAKE_CXX_STANDARD 17)

add_executable(primes_hyperbola main.cpp)

# Link statically on Windows.
if (WIN32)
    target_link_libraries(primes_hyperbola -static-libstdc++ -static-libgcc)
endif()
