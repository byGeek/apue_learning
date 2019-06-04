cmake_minimum_required (VERSION 3.0)
project (apue_learning)

set (apue_learning_VERSION_MAJOR 1)
set (aput_learning_VERSION_MINOR 0)

find_package(Threads)

add_executable(server 
    ch17/unix_domain_socket.cpp
    ch17/server.cpp
)

add_executable(client
    ch17/unix_domain_socket.cpp
    ch17/client.cpp
)

# add_executable(sock_client
#     ch16/sock_client.cpp
# )

#target_link_libraries(sock_server ${CMAKE_THREAD_LIBS_INIT})