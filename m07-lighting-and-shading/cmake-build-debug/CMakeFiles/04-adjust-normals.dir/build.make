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
include CMakeFiles/04-adjust-normals.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/04-adjust-normals.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/04-adjust-normals.dir/flags.make

CMakeFiles/04-adjust-normals.dir/04-adjust-normals/adjust_normals.cpp.obj: CMakeFiles/04-adjust-normals.dir/flags.make
CMakeFiles/04-adjust-normals.dir/04-adjust-normals/adjust_normals.cpp.obj: CMakeFiles/04-adjust-normals.dir/includes_CXX.rsp
CMakeFiles/04-adjust-normals.dir/04-adjust-normals/adjust_normals.cpp.obj: ../04-adjust-normals/adjust_normals.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/04-adjust-normals.dir/04-adjust-normals/adjust_normals.cpp.obj"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\04-adjust-normals.dir\04-adjust-normals\adjust_normals.cpp.obj -c "D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading\04-adjust-normals\adjust_normals.cpp"

CMakeFiles/04-adjust-normals.dir/04-adjust-normals/adjust_normals.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/04-adjust-normals.dir/04-adjust-normals/adjust_normals.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading\04-adjust-normals\adjust_normals.cpp" > CMakeFiles\04-adjust-normals.dir\04-adjust-normals\adjust_normals.cpp.i

CMakeFiles/04-adjust-normals.dir/04-adjust-normals/adjust_normals.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/04-adjust-normals.dir/04-adjust-normals/adjust_normals.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading\04-adjust-normals\adjust_normals.cpp" -o CMakeFiles\04-adjust-normals.dir\04-adjust-normals\adjust_normals.cpp.s

# Object files for target 04-adjust-normals
04__adjust__normals_OBJECTS = \
"CMakeFiles/04-adjust-normals.dir/04-adjust-normals/adjust_normals.cpp.obj"

# External object files for target 04-adjust-normals
04__adjust__normals_EXTERNAL_OBJECTS =

04-adjust-normals.exe: CMakeFiles/04-adjust-normals.dir/04-adjust-normals/adjust_normals.cpp.obj
04-adjust-normals.exe: CMakeFiles/04-adjust-normals.dir/build.make
04-adjust-normals.exe: CMakeFiles/04-adjust-normals.dir/linklibs.rsp
04-adjust-normals.exe: CMakeFiles/04-adjust-normals.dir/objects1.rsp
04-adjust-normals.exe: CMakeFiles/04-adjust-normals.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable 04-adjust-normals.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\04-adjust-normals.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/04-adjust-normals.dir/build: 04-adjust-normals.exe
.PHONY : CMakeFiles/04-adjust-normals.dir/build

CMakeFiles/04-adjust-normals.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\04-adjust-normals.dir\cmake_clean.cmake
.PHONY : CMakeFiles/04-adjust-normals.dir/clean

CMakeFiles/04-adjust-normals.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading" "D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading" "D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading\cmake-build-debug" "D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading\cmake-build-debug" "D:\data\drive\College\Forth Year\Fall Semester\Computer Graphics and Multimedia\examples\m07-lighting-and-shading\cmake-build-debug\CMakeFiles\04-adjust-normals.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/04-adjust-normals.dir/depend

