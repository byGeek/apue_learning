cmake_minimum_required (VERSION 3.0)
project (apue_learning)

set (apue_learning_VERSION_MAJOR 1)
set (aput_learning_VERSION_MINOR 0)

find_package(Threads)

add_executable(apue_test ch11/mutex_multiple_process.cpp)

target_link_libraries(apue_test ${CMAKE_THREAD_LIBS_INIT})