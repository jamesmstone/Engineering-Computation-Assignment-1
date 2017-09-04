cmake_minimum_required(VERSION 3.7)
project(code)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(code ${SOURCE_FILES})
target_link_libraries(code m)