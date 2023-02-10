cmake_minimum_required(VERSION 3.21)
project(webserver)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserver main.cpp
        src/http/Request.cpp
        src/http/Request.hpp
        src/http/Status.hpp
        src/http/Status.cpp
        src/http/Utils.cpp
        src/http/Utils.hpp
        src/http/Response.cpp
        src/http/Response.hpp
        src/config/Config.cpp
        src/config/Config.hpp
        src/config/ConfigException.hpp
        src/config/LocationBlock.cpp
        src/config/LocationBlock.hpp
        src/config/ServerBlock.cpp
        src/config/ServerBlock.hpp
        src/config/Global.cpp
        src/config/Global.hpp
)
