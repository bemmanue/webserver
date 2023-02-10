cmake_minimum_required(VERSION 3.21)
project(webserver)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserver main.cpp
        http/Request.cpp
        http/Request.hpp
        http/Status.hpp
        http/Status.cpp
        http/Utils.cpp
        http/Utils.hpp
        http/Response.cpp
        http/Response.hpp
        config/Config.cpp
        config/Config.hpp
        config/ConfigException.hpp
        config/LocationBlock.cpp
        config/LocationBlock.hpp
        config/ServerBlock.cpp
        config/ServerBlock.hpp
        config/Global.cpp
        config/Global.hpp
)
