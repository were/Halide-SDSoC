# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/were/Halide-SDSoC

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/were/Halide-SDSoC/cmake-build-debug

# Include any dependencies generated for this target.
include tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/depend.make

# Include the progress variables for this target.
include tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/progress.make

# Include the compile flags for this target's objects.
include tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/flags.make

tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.o: tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/flags.make
tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.o: ../tutorial/lesson_10_aot_compilation_run.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.o"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/tutorial && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.o -c /Users/were/Halide-SDSoC/tutorial/lesson_10_aot_compilation_run.cpp

tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.i"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/tutorial && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/were/Halide-SDSoC/tutorial/lesson_10_aot_compilation_run.cpp > CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.i

tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.s"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/tutorial && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/were/Halide-SDSoC/tutorial/lesson_10_aot_compilation_run.cpp -o CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.s

tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.o.requires:

.PHONY : tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.o.requires

tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.o.provides: tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.o.requires
	$(MAKE) -f tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/build.make tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.o.provides.build
.PHONY : tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.o.provides

tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.o.provides.build: tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.o


# Object files for target lesson_10_aot_compilation_run
lesson_10_aot_compilation_run_OBJECTS = \
"CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.o"

# External object files for target lesson_10_aot_compilation_run
lesson_10_aot_compilation_run_EXTERNAL_OBJECTS =

bin/lesson_10_aot_compilation_run: tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.o
bin/lesson_10_aot_compilation_run: tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/build.make
bin/lesson_10_aot_compilation_run: tutorial/lesson_10_halide.a
bin/lesson_10_aot_compilation_run: tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/lesson_10_aot_compilation_run"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/tutorial && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lesson_10_aot_compilation_run.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/build: bin/lesson_10_aot_compilation_run

.PHONY : tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/build

tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/requires: tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/lesson_10_aot_compilation_run.cpp.o.requires

.PHONY : tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/requires

tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/clean:
	cd /Users/were/Halide-SDSoC/cmake-build-debug/tutorial && $(CMAKE_COMMAND) -P CMakeFiles/lesson_10_aot_compilation_run.dir/cmake_clean.cmake
.PHONY : tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/clean

tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/depend:
	cd /Users/were/Halide-SDSoC/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/were/Halide-SDSoC /Users/were/Halide-SDSoC/tutorial /Users/were/Halide-SDSoC/cmake-build-debug /Users/were/Halide-SDSoC/cmake-build-debug/tutorial /Users/were/Halide-SDSoC/cmake-build-debug/tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tutorial/CMakeFiles/lesson_10_aot_compilation_run.dir/depend

