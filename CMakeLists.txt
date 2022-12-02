cmake_minimum_required(VERSION 3.21)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserv
        srcs/main.cpp
        srcs/server.cpp
        srcs/include.dir/server.hpp srcs/include.dir/connection.hpp srcs/include.dir/utils.hpp)
