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
include test/CMakeFiles/performance_sort.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/performance_sort.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/performance_sort.dir/flags.make

test/CMakeFiles/performance_sort.dir/performance/sort.cpp.o: test/CMakeFiles/performance_sort.dir/flags.make
test/CMakeFiles/performance_sort.dir/performance/sort.cpp.o: ../test/performance/sort.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/performance_sort.dir/performance/sort.cpp.o"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/performance_sort.dir/performance/sort.cpp.o -c /Users/were/Halide-SDSoC/test/performance/sort.cpp

test/CMakeFiles/performance_sort.dir/performance/sort.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/performance_sort.dir/performance/sort.cpp.i"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/were/Halide-SDSoC/test/performance/sort.cpp > CMakeFiles/performance_sort.dir/performance/sort.cpp.i

test/CMakeFiles/performance_sort.dir/performance/sort.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/performance_sort.dir/performance/sort.cpp.s"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/were/Halide-SDSoC/test/performance/sort.cpp -o CMakeFiles/performance_sort.dir/performance/sort.cpp.s

test/CMakeFiles/performance_sort.dir/performance/sort.cpp.o.requires:

.PHONY : test/CMakeFiles/performance_sort.dir/performance/sort.cpp.o.requires

test/CMakeFiles/performance_sort.dir/performance/sort.cpp.o.provides: test/CMakeFiles/performance_sort.dir/performance/sort.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/performance_sort.dir/build.make test/CMakeFiles/performance_sort.dir/performance/sort.cpp.o.provides.build
.PHONY : test/CMakeFiles/performance_sort.dir/performance/sort.cpp.o.provides

test/CMakeFiles/performance_sort.dir/performance/sort.cpp.o.provides.build: test/CMakeFiles/performance_sort.dir/performance/sort.cpp.o


# Object files for target performance_sort
performance_sort_OBJECTS = \
"CMakeFiles/performance_sort.dir/performance/sort.cpp.o"

# External object files for target performance_sort
performance_sort_EXTERNAL_OBJECTS =

bin/performance_sort: test/CMakeFiles/performance_sort.dir/performance/sort.cpp.o
bin/performance_sort: test/CMakeFiles/performance_sort.dir/build.make
bin/performance_sort: lib/libHalide.dylib
bin/performance_sort: test/CMakeFiles/performance_sort.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/performance_sort"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/performance_sort.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/performance_sort.dir/build: bin/performance_sort

.PHONY : test/CMakeFiles/performance_sort.dir/build

test/CMakeFiles/performance_sort.dir/requires: test/CMakeFiles/performance_sort.dir/performance/sort.cpp.o.requires

.PHONY : test/CMakeFiles/performance_sort.dir/requires

test/CMakeFiles/performance_sort.dir/clean:
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && $(CMAKE_COMMAND) -P CMakeFiles/performance_sort.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/performance_sort.dir/clean

test/CMakeFiles/performance_sort.dir/depend:
	cd /Users/were/Halide-SDSoC/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/were/Halide-SDSoC /Users/were/Halide-SDSoC/test /Users/were/Halide-SDSoC/cmake-build-debug /Users/were/Halide-SDSoC/cmake-build-debug/test /Users/were/Halide-SDSoC/cmake-build-debug/test/CMakeFiles/performance_sort.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/performance_sort.dir/depend

