# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.21

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.2\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/02-ambient-light.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/02-ambient-light.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/02-ambient-light.dir/flags.make

CMakeFiles/02-ambient-light.dir/02-ambient-light/ambient-light.cpp.obj: CMakeFiles/02-ambient-light.dir/flags.make
CMakeFiles/02-ambient-light.dir/02-ambient-light/ambient-light.cpp.obj: CMakeFiles/02-ambient-light.dir/includes_CXX.rsp
CMakeFiles/02-ambient-light.dir/02-ambient-light/ambient-light.cpp.obj: ../02-ambient-light/ambient-light.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/02-ambient-light.dir/02-ambient-light/ambient-light.cpp.obj"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\02-ambient-light.dir\02-ambient-light\ambient-light.cpp.obj -c "D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading\02-ambient-light\ambient-light.cpp"

CMakeFiles/02-ambient-light.dir/02-ambient-light/ambient-light.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/02-ambient-light.dir/02-ambient-light/ambient-light.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading\02-ambient-light\ambient-light.cpp" > CMakeFiles\02-ambient-light.dir\02-ambient-light\ambient-light.cpp.i

CMakeFiles/02-ambient-light.dir/02-ambient-light/ambient-light.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/02-ambient-light.dir/02-ambient-light/ambient-light.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading\02-ambient-light\ambient-light.cpp" -o CMakeFiles\02-ambient-light.dir\02-ambient-light\ambient-light.cpp.s

# Object files for target 02-ambient-light
02__ambient__light_OBJECTS = \
"CMakeFiles/02-ambient-light.dir/02-ambient-light/ambient-light.cpp.obj"

# External object files for target 02-ambient-light
02__ambient__light_EXTERNAL_OBJECTS =

02-ambient-light.exe: CMakeFiles/02-ambient-light.dir/02-ambient-light/ambient-light.cpp.obj
02-ambient-light.exe: CMakeFiles/02-ambient-light.dir/build.make
02-ambient-light.exe: CMakeFiles/02-ambient-light.dir/linklibs.rsp
02-ambient-light.exe: CMakeFiles/02-ambient-light.dir/objects1.rsp
02-ambient-light.exe: CMakeFiles/02-ambient-light.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable 02-ambient-light.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\02-ambient-light.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/02-ambient-light.dir/build: 02-ambient-light.exe
.PHONY : CMakeFiles/02-ambient-light.dir/build

CMakeFiles/02-ambient-light.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\02-ambient-light.dir\cmake_clean.cmake
.PHONY : CMakeFiles/02-ambient-light.dir/clean

CMakeFiles/02-ambient-light.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading" "D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading" "D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading\cmake-build-debug" "D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading\cmake-build-debug" "D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading\cmake-build-debug\CMakeFiles\02-ambient-light.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/02-ambient-light.dir/depend

