cmake_minimum_required(VERSION 3.21)
project(webserver)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserver src/main.cpp
        include/config/Config.hpp
        include/config/LocationBlock.hpp
        include/config/ConfigException.hpp
        include/config/ServerBlock.hpp
        include/Request.hpp
        src/config/Config.cpp
        src/config/LocationBlock.cpp
        src/config/ServerBlock.cpp
        src/Request.cpp
        srcs/include.dir/utils.hpp
        srcs/include.dir/MyException.hpp

        srcs/server.cpp srcs/include.dir/server.hpp
        srcs/connection.cpp srcs/include.dir/connection.hpp
        srcs/IOPoll.cpp srcs/include.dir/IOPoll.hpp
        srcs/client.cpp srcs/include.dir/client.hpp
        srcs/cgi/cgi.cpp srcs/cgi/cgi.hpp)
