cmake_minimum_required(VERSION 3.21)
project(webserver)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_executable(
        webserver
        srcs/main.cpp
        srcs/server.cpp
        srcs/include.dir/server.hpp
        srcs/include.dir/connection.hpp
        srcs/include.dir/utils.hpp
        srcs/test.cpp
        srcs/connection.cpp
)
