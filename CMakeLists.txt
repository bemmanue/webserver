cmake_minimum_required(VERSION 3.20)
project(webserver)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserver main.cpp
        src/http/Request.cpp
        src/http/Request.hpp
        src/http/Status.hpp
        src/http/Status.cpp
        src/http/Utils.cpp
        src/http/Utils.hpp
        src/http/URI.cpp
        src/http/URI.hpp
        src/http/Headers.hpp
        src/http/Headers.cpp
        src/http/Response.cpp
        src/http/Response.hpp
        src/config/Config.cpp
        src/config/Config.hpp
        src/config/ConfigException.hpp
        src/config/LocationConfig.cpp
        src/config/LocationConfig.hpp
        src/config/ServerConfig.cpp
        src/config/ServerConfig.hpp
        src/core/Client.cpp
        src/core/Client.hpp
        )
