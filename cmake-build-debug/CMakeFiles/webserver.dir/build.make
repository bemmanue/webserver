# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/uliakulikova/Desktop/webserver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/uliakulikova/Desktop/webserver/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/webserver.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/webserver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/webserver.dir/flags.make

CMakeFiles/webserver.dir/main.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/uliakulikova/Desktop/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/webserver.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/main.cpp.o -c /Users/uliakulikova/Desktop/webserver/main.cpp

CMakeFiles/webserver.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/uliakulikova/Desktop/webserver/main.cpp > CMakeFiles/webserver.dir/main.cpp.i

CMakeFiles/webserver.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/uliakulikova/Desktop/webserver/main.cpp -o CMakeFiles/webserver.dir/main.cpp.s

CMakeFiles/webserver.dir/src/http/Request.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/src/http/Request.cpp.o: ../src/http/Request.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/uliakulikova/Desktop/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/webserver.dir/src/http/Request.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/src/http/Request.cpp.o -c /Users/uliakulikova/Desktop/webserver/src/http/Request.cpp

CMakeFiles/webserver.dir/src/http/Request.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/src/http/Request.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/uliakulikova/Desktop/webserver/src/http/Request.cpp > CMakeFiles/webserver.dir/src/http/Request.cpp.i

CMakeFiles/webserver.dir/src/http/Request.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/src/http/Request.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/uliakulikova/Desktop/webserver/src/http/Request.cpp -o CMakeFiles/webserver.dir/src/http/Request.cpp.s

CMakeFiles/webserver.dir/src/http/Status.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/src/http/Status.cpp.o: ../src/http/Status.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/uliakulikova/Desktop/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/webserver.dir/src/http/Status.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/src/http/Status.cpp.o -c /Users/uliakulikova/Desktop/webserver/src/http/Status.cpp

CMakeFiles/webserver.dir/src/http/Status.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/src/http/Status.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/uliakulikova/Desktop/webserver/src/http/Status.cpp > CMakeFiles/webserver.dir/src/http/Status.cpp.i

CMakeFiles/webserver.dir/src/http/Status.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/src/http/Status.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/uliakulikova/Desktop/webserver/src/http/Status.cpp -o CMakeFiles/webserver.dir/src/http/Status.cpp.s

CMakeFiles/webserver.dir/src/http/Utils.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/src/http/Utils.cpp.o: ../src/http/Utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/uliakulikova/Desktop/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/webserver.dir/src/http/Utils.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/src/http/Utils.cpp.o -c /Users/uliakulikova/Desktop/webserver/src/http/Utils.cpp

CMakeFiles/webserver.dir/src/http/Utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/src/http/Utils.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/uliakulikova/Desktop/webserver/src/http/Utils.cpp > CMakeFiles/webserver.dir/src/http/Utils.cpp.i

CMakeFiles/webserver.dir/src/http/Utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/src/http/Utils.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/uliakulikova/Desktop/webserver/src/http/Utils.cpp -o CMakeFiles/webserver.dir/src/http/Utils.cpp.s

CMakeFiles/webserver.dir/src/http/Response.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/src/http/Response.cpp.o: ../src/http/Response.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/uliakulikova/Desktop/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/webserver.dir/src/http/Response.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/src/http/Response.cpp.o -c /Users/uliakulikova/Desktop/webserver/src/http/Response.cpp

CMakeFiles/webserver.dir/src/http/Response.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/src/http/Response.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/uliakulikova/Desktop/webserver/src/http/Response.cpp > CMakeFiles/webserver.dir/src/http/Response.cpp.i

CMakeFiles/webserver.dir/src/http/Response.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/src/http/Response.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/uliakulikova/Desktop/webserver/src/http/Response.cpp -o CMakeFiles/webserver.dir/src/http/Response.cpp.s

CMakeFiles/webserver.dir/src/config/Config.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/src/config/Config.cpp.o: ../src/config/Config.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/uliakulikova/Desktop/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/webserver.dir/src/config/Config.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/src/config/Config.cpp.o -c /Users/uliakulikova/Desktop/webserver/src/config/Config.cpp

CMakeFiles/webserver.dir/src/config/Config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/src/config/Config.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/uliakulikova/Desktop/webserver/src/config/Config.cpp > CMakeFiles/webserver.dir/src/config/Config.cpp.i

CMakeFiles/webserver.dir/src/config/Config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/src/config/Config.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/uliakulikova/Desktop/webserver/src/config/Config.cpp -o CMakeFiles/webserver.dir/src/config/Config.cpp.s

CMakeFiles/webserver.dir/src/config/LocationConfig.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/src/config/LocationConfig.cpp.o: ../src/config/LocationConfig.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/uliakulikova/Desktop/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/webserver.dir/src/config/LocationConfig.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/src/config/LocationConfig.cpp.o -c /Users/uliakulikova/Desktop/webserver/src/config/LocationConfig.cpp

CMakeFiles/webserver.dir/src/config/LocationConfig.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/src/config/LocationConfig.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/uliakulikova/Desktop/webserver/src/config/LocationConfig.cpp > CMakeFiles/webserver.dir/src/config/LocationConfig.cpp.i

CMakeFiles/webserver.dir/src/config/LocationConfig.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/src/config/LocationConfig.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/uliakulikova/Desktop/webserver/src/config/LocationConfig.cpp -o CMakeFiles/webserver.dir/src/config/LocationConfig.cpp.s

CMakeFiles/webserver.dir/src/config/ServerConfig.cpp.o: CMakeFiles/webserver.dir/flags.make
CMakeFiles/webserver.dir/src/config/ServerConfig.cpp.o: ../src/config/ServerConfig.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/uliakulikova/Desktop/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/webserver.dir/src/config/ServerConfig.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserver.dir/src/config/ServerConfig.cpp.o -c /Users/uliakulikova/Desktop/webserver/src/config/ServerConfig.cpp

CMakeFiles/webserver.dir/src/config/ServerConfig.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserver.dir/src/config/ServerConfig.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/uliakulikova/Desktop/webserver/src/config/ServerConfig.cpp > CMakeFiles/webserver.dir/src/config/ServerConfig.cpp.i

CMakeFiles/webserver.dir/src/config/ServerConfig.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserver.dir/src/config/ServerConfig.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/uliakulikova/Desktop/webserver/src/config/ServerConfig.cpp -o CMakeFiles/webserver.dir/src/config/ServerConfig.cpp.s

# Object files for target webserver
webserver_OBJECTS = \
"CMakeFiles/webserver.dir/main.cpp.o" \
"CMakeFiles/webserver.dir/src/http/Request.cpp.o" \
"CMakeFiles/webserver.dir/src/http/Status.cpp.o" \
"CMakeFiles/webserver.dir/src/http/Utils.cpp.o" \
"CMakeFiles/webserver.dir/src/http/Response.cpp.o" \
"CMakeFiles/webserver.dir/src/config/Config.cpp.o" \
"CMakeFiles/webserver.dir/src/config/LocationConfig.cpp.o" \
"CMakeFiles/webserver.dir/src/config/ServerConfig.cpp.o"

# External object files for target webserver
webserver_EXTERNAL_OBJECTS =

webserver: CMakeFiles/webserver.dir/main.cpp.o
webserver: CMakeFiles/webserver.dir/src/http/Request.cpp.o
webserver: CMakeFiles/webserver.dir/src/http/Status.cpp.o
webserver: CMakeFiles/webserver.dir/src/http/Utils.cpp.o
webserver: CMakeFiles/webserver.dir/src/http/Response.cpp.o
webserver: CMakeFiles/webserver.dir/src/config/Config.cpp.o
webserver: CMakeFiles/webserver.dir/src/config/LocationConfig.cpp.o
webserver: CMakeFiles/webserver.dir/src/config/ServerConfig.cpp.o
webserver: CMakeFiles/webserver.dir/build.make
webserver: CMakeFiles/webserver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/uliakulikova/Desktop/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable webserver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/webserver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/webserver.dir/build: webserver
.PHONY : CMakeFiles/webserver.dir/build

CMakeFiles/webserver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/webserver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/webserver.dir/clean

CMakeFiles/webserver.dir/depend:
	cd /Users/uliakulikova/Desktop/webserver/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/uliakulikova/Desktop/webserver /Users/uliakulikova/Desktop/webserver /Users/uliakulikova/Desktop/webserver/cmake-build-debug /Users/uliakulikova/Desktop/webserver/cmake-build-debug /Users/uliakulikova/Desktop/webserver/cmake-build-debug/CMakeFiles/webserver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/webserver.dir/depend

