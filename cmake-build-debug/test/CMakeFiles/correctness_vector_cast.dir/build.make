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
include test/CMakeFiles/correctness_vector_cast.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/correctness_vector_cast.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/correctness_vector_cast.dir/flags.make

test/CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.o: test/CMakeFiles/correctness_vector_cast.dir/flags.make
test/CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.o: ../test/correctness/vector_cast.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.o"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.o -c /Users/were/Halide-SDSoC/test/correctness/vector_cast.cpp

test/CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.i"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/were/Halide-SDSoC/test/correctness/vector_cast.cpp > CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.i

test/CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.s"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/were/Halide-SDSoC/test/correctness/vector_cast.cpp -o CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.s

test/CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.o.requires:

.PHONY : test/CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.o.requires

test/CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.o.provides: test/CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/correctness_vector_cast.dir/build.make test/CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.o.provides.build
.PHONY : test/CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.o.provides

test/CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.o.provides.build: test/CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.o


# Object files for target correctness_vector_cast
correctness_vector_cast_OBJECTS = \
"CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.o"

# External object files for target correctness_vector_cast
correctness_vector_cast_EXTERNAL_OBJECTS =

bin/correctness_vector_cast: test/CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.o
bin/correctness_vector_cast: test/CMakeFiles/correctness_vector_cast.dir/build.make
bin/correctness_vector_cast: lib/libHalide.dylib
bin/correctness_vector_cast: test/CMakeFiles/correctness_vector_cast.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/correctness_vector_cast"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/correctness_vector_cast.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/correctness_vector_cast.dir/build: bin/correctness_vector_cast

.PHONY : test/CMakeFiles/correctness_vector_cast.dir/build

test/CMakeFiles/correctness_vector_cast.dir/requires: test/CMakeFiles/correctness_vector_cast.dir/correctness/vector_cast.cpp.o.requires

.PHONY : test/CMakeFiles/correctness_vector_cast.dir/requires

test/CMakeFiles/correctness_vector_cast.dir/clean:
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && $(CMAKE_COMMAND) -P CMakeFiles/correctness_vector_cast.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/correctness_vector_cast.dir/clean

test/CMakeFiles/correctness_vector_cast.dir/depend:
	cd /Users/were/Halide-SDSoC/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/were/Halide-SDSoC /Users/were/Halide-SDSoC/test /Users/were/Halide-SDSoC/cmake-build-debug /Users/were/Halide-SDSoC/cmake-build-debug/test /Users/were/Halide-SDSoC/cmake-build-debug/test/CMakeFiles/correctness_vector_cast.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/correctness_vector_cast.dir/depend

