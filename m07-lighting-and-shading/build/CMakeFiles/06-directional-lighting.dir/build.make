# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\bsetz\OneDrive - Kennesaw State University\courses\4722fa21\4722fa21_code\example_stream\m07-lighting-and-shading"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\bsetz\OneDrive - Kennesaw State University\courses\4722fa21\4722fa21_code\example_stream\m07-lighting-and-shading\build"

# Include any dependencies generated for this target.
include CMakeFiles/06-directional-lighting.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/06-directional-lighting.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/06-directional-lighting.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/06-directional-lighting.dir/flags.make

CMakeFiles/06-directional-lighting.dir/06-directional-lighting/directional_lighting.cpp.obj: CMakeFiles/06-directional-lighting.dir/flags.make
CMakeFiles/06-directional-lighting.dir/06-directional-lighting/directional_lighting.cpp.obj: CMakeFiles/06-directional-lighting.dir/includes_CXX.rsp
CMakeFiles/06-directional-lighting.dir/06-directional-lighting/directional_lighting.cpp.obj: ../06-directional-lighting/directional_lighting.cpp
CMakeFiles/06-directional-lighting.dir/06-directional-lighting/directional_lighting.cpp.obj: CMakeFiles/06-directional-lighting.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\bsetz\OneDrive - Kennesaw State University\courses\4722fa21\4722fa21_code\example_stream\m07-lighting-and-shading\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/06-directional-lighting.dir/06-directional-lighting/directional_lighting.cpp.obj"
	c:\mingw\bin\mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/06-directional-lighting.dir/06-directional-lighting/directional_lighting.cpp.obj -MF CMakeFiles\06-directional-lighting.dir\06-directional-lighting\directional_lighting.cpp.obj.d -o CMakeFiles\06-directional-lighting.dir\06-directional-lighting\directional_lighting.cpp.obj -c "C:\Users\bsetz\OneDrive - Kennesaw State University\courses\4722fa21\4722fa21_code\example_stream\m07-lighting-and-shading\06-directional-lighting\directional_lighting.cpp"

CMakeFiles/06-directional-lighting.dir/06-directional-lighting/directional_lighting.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/06-directional-lighting.dir/06-directional-lighting/directional_lighting.cpp.i"
	c:\mingw\bin\mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\bsetz\OneDrive - Kennesaw State University\courses\4722fa21\4722fa21_code\example_stream\m07-lighting-and-shading\06-directional-lighting\directional_lighting.cpp" > CMakeFiles\06-directional-lighting.dir\06-directional-lighting\directional_lighting.cpp.i

CMakeFiles/06-directional-lighting.dir/06-directional-lighting/directional_lighting.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/06-directional-lighting.dir/06-directional-lighting/directional_lighting.cpp.s"
	c:\mingw\bin\mingw32-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\bsetz\OneDrive - Kennesaw State University\courses\4722fa21\4722fa21_code\example_stream\m07-lighting-and-shading\06-directional-lighting\directional_lighting.cpp" -o CMakeFiles\06-directional-lighting.dir\06-directional-lighting\directional_lighting.cpp.s

# Object files for target 06-directional-lighting
06__directional__lighting_OBJECTS = \
"CMakeFiles/06-directional-lighting.dir/06-directional-lighting/directional_lighting.cpp.obj"

# External object files for target 06-directional-lighting
06__directional__lighting_EXTERNAL_OBJECTS =

06-directional-lighting.exe: CMakeFiles/06-directional-lighting.dir/06-directional-lighting/directional_lighting.cpp.obj
06-directional-lighting.exe: CMakeFiles/06-directional-lighting.dir/build.make
06-directional-lighting.exe: CMakeFiles/06-directional-lighting.dir/linklibs.rsp
06-directional-lighting.exe: CMakeFiles/06-directional-lighting.dir/objects1.rsp
06-directional-lighting.exe: CMakeFiles/06-directional-lighting.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\bsetz\OneDrive - Kennesaw State University\courses\4722fa21\4722fa21_code\example_stream\m07-lighting-and-shading\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable 06-directional-lighting.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\06-directional-lighting.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/06-directional-lighting.dir/build: 06-directional-lighting.exe
.PHONY : CMakeFiles/06-directional-lighting.dir/build

CMakeFiles/06-directional-lighting.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\06-directional-lighting.dir\cmake_clean.cmake
.PHONY : CMakeFiles/06-directional-lighting.dir/clean

CMakeFiles/06-directional-lighting.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\bsetz\OneDrive - Kennesaw State University\courses\4722fa21\4722fa21_code\example_stream\m07-lighting-and-shading" "C:\Users\bsetz\OneDrive - Kennesaw State University\courses\4722fa21\4722fa21_code\example_stream\m07-lighting-and-shading" "C:\Users\bsetz\OneDrive - Kennesaw State University\courses\4722fa21\4722fa21_code\example_stream\m07-lighting-and-shading\build" "C:\Users\bsetz\OneDrive - Kennesaw State University\courses\4722fa21\4722fa21_code\example_stream\m07-lighting-and-shading\build" "C:\Users\bsetz\OneDrive - Kennesaw State University\courses\4722fa21\4722fa21_code\example_stream\m07-lighting-and-shading\build\CMakeFiles\06-directional-lighting.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/06-directional-lighting.dir/depend

