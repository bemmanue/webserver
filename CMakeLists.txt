cmake_minimum_required(VERSION 3.21)
project(webserver)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserver src/main.cpp
        include/config/Config.hpp
        include/config/LocationBlock.hpp
        include/config/ConfigException.hpp
        include/config/ServerBlock.hpp
        src/config/Config.cpp
        src/config/LocationBlock.cpp
        src/config/ServerBlock.cpp
        src/Request.cpp
        include/utils.hpp
        include/MyException.hpp
        src/server.cpp include/server.hpp
        src/connection.cpp include/connection.hpp
        src/IOPoll.cpp include/IOPoll.hpp
        src/client.cpp include/client.hpp
        src/cgi/cgi.cpp src/cgi/cgi.hpp)
