cmake_minimum_required(VERSION 3.21)
project(webserver)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserver src/main.cpp
        include/Config.hpp
        include/Location.hpp
        src/Config.cpp
        src/Location.cpp
        )