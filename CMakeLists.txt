cmake_minimum_required (VERSION 3.0)
project (apue_learning)

set (apue_learning_VERSION_MAJOR 1)
set (aput_learning_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads)

# add_executable(server 
#     ch17/unix_domain_socket.cpp
#     ch17/server.cpp
# )

# add_executable(client
#     ch17/unix_domain_socket.cpp
#     ch17/client.cpp
# )

add_executable(test_static
    static_variable_dtor_test.cpp
)

#target_link_libraries(sock_server ${CMAKE_THREAD_LIBS_INIT})