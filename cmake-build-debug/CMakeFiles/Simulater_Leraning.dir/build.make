# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\nijir\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\203.7717.62\bin\cmake\win\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\nijir\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\203.7717.62\bin\cmake\win\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\nijir\CLionProjects\Simulater_Leraning

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\nijir\CLionProjects\Simulater_Leraning\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Simulater_Leraning.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Simulater_Leraning.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Simulater_Leraning.dir/flags.make

CMakeFiles/Simulater_Leraning.dir/main.cpp.obj: CMakeFiles/Simulater_Leraning.dir/flags.make
CMakeFiles/Simulater_Leraning.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\nijir\CLionProjects\Simulater_Leraning\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Simulater_Leraning.dir/main.cpp.obj"
	C:\Users\nijir\scoop\apps\gcc\current\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Simulater_Leraning.dir\main.cpp.obj -c C:\Users\nijir\CLionProjects\Simulater_Leraning\main.cpp

CMakeFiles/Simulater_Leraning.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Simulater_Leraning.dir/main.cpp.i"
	C:\Users\nijir\scoop\apps\gcc\current\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\nijir\CLionProjects\Simulater_Leraning\main.cpp > CMakeFiles\Simulater_Leraning.dir\main.cpp.i

CMakeFiles/Simulater_Leraning.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Simulater_Leraning.dir/main.cpp.s"
	C:\Users\nijir\scoop\apps\gcc\current\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\nijir\CLionProjects\Simulater_Leraning\main.cpp -o CMakeFiles\Simulater_Leraning.dir\main.cpp.s

# Object files for target Simulater_Leraning
Simulater_Leraning_OBJECTS = \
"CMakeFiles/Simulater_Leraning.dir/main.cpp.obj"

# External object files for target Simulater_Leraning
Simulater_Leraning_EXTERNAL_OBJECTS =

Simulater_Leraning.exe: CMakeFiles/Simulater_Leraning.dir/main.cpp.obj
Simulater_Leraning.exe: CMakeFiles/Simulater_Leraning.dir/build.make
Simulater_Leraning.exe: CMakeFiles/Simulater_Leraning.dir/linklibs.rsp
Simulater_Leraning.exe: CMakeFiles/Simulater_Leraning.dir/objects1.rsp
Simulater_Leraning.exe: CMakeFiles/Simulater_Leraning.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\nijir\CLionProjects\Simulater_Leraning\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Simulater_Leraning.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Simulater_Leraning.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Simulater_Leraning.dir/build: Simulater_Leraning.exe

.PHONY : CMakeFiles/Simulater_Leraning.dir/build

CMakeFiles/Simulater_Leraning.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Simulater_Leraning.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Simulater_Leraning.dir/clean

CMakeFiles/Simulater_Leraning.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\nijir\CLionProjects\Simulater_Leraning C:\Users\nijir\CLionProjects\Simulater_Leraning C:\Users\nijir\CLionProjects\Simulater_Leraning\cmake-build-debug C:\Users\nijir\CLionProjects\Simulater_Leraning\cmake-build-debug C:\Users\nijir\CLionProjects\Simulater_Leraning\cmake-build-debug\CMakeFiles\Simulater_Leraning.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Simulater_Leraning.dir/depend

