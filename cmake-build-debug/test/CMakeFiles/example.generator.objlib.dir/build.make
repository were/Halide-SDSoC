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
include test/CMakeFiles/example.generator.objlib.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/example.generator.objlib.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/example.generator.objlib.dir/flags.make

test/CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.o: test/CMakeFiles/example.generator.objlib.dir/flags.make
test/CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.o: ../test/generator/example_generator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.o"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.o -c /Users/were/Halide-SDSoC/test/generator/example_generator.cpp

test/CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.i"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/were/Halide-SDSoC/test/generator/example_generator.cpp > CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.i

test/CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.s"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/were/Halide-SDSoC/test/generator/example_generator.cpp -o CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.s

test/CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.o.requires:

.PHONY : test/CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.o.requires

test/CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.o.provides: test/CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/example.generator.objlib.dir/build.make test/CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.o.provides.build
.PHONY : test/CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.o.provides

test/CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.o.provides.build: test/CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.o


example.generator.objlib: test/CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.o
example.generator.objlib: test/CMakeFiles/example.generator.objlib.dir/build.make

.PHONY : example.generator.objlib

# Rule to build all files generated by this target.
test/CMakeFiles/example.generator.objlib.dir/build: example.generator.objlib

.PHONY : test/CMakeFiles/example.generator.objlib.dir/build

test/CMakeFiles/example.generator.objlib.dir/requires: test/CMakeFiles/example.generator.objlib.dir/generator/example_generator.cpp.o.requires

.PHONY : test/CMakeFiles/example.generator.objlib.dir/requires

test/CMakeFiles/example.generator.objlib.dir/clean:
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && $(CMAKE_COMMAND) -P CMakeFiles/example.generator.objlib.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/example.generator.objlib.dir/clean

test/CMakeFiles/example.generator.objlib.dir/depend:
	cd /Users/were/Halide-SDSoC/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/were/Halide-SDSoC /Users/were/Halide-SDSoC/test /Users/were/Halide-SDSoC/cmake-build-debug /Users/were/Halide-SDSoC/cmake-build-debug/test /Users/were/Halide-SDSoC/cmake-build-debug/test/CMakeFiles/example.generator.objlib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/example.generator.objlib.dir/depend

