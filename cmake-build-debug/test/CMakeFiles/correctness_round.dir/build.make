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
include test/CMakeFiles/correctness_round.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/correctness_round.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/correctness_round.dir/flags.make

test/CMakeFiles/correctness_round.dir/correctness/round.cpp.o: test/CMakeFiles/correctness_round.dir/flags.make
test/CMakeFiles/correctness_round.dir/correctness/round.cpp.o: ../test/correctness/round.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/correctness_round.dir/correctness/round.cpp.o"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/correctness_round.dir/correctness/round.cpp.o -c /Users/were/Halide-SDSoC/test/correctness/round.cpp

test/CMakeFiles/correctness_round.dir/correctness/round.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/correctness_round.dir/correctness/round.cpp.i"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/were/Halide-SDSoC/test/correctness/round.cpp > CMakeFiles/correctness_round.dir/correctness/round.cpp.i

test/CMakeFiles/correctness_round.dir/correctness/round.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/correctness_round.dir/correctness/round.cpp.s"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/were/Halide-SDSoC/test/correctness/round.cpp -o CMakeFiles/correctness_round.dir/correctness/round.cpp.s

test/CMakeFiles/correctness_round.dir/correctness/round.cpp.o.requires:

.PHONY : test/CMakeFiles/correctness_round.dir/correctness/round.cpp.o.requires

test/CMakeFiles/correctness_round.dir/correctness/round.cpp.o.provides: test/CMakeFiles/correctness_round.dir/correctness/round.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/correctness_round.dir/build.make test/CMakeFiles/correctness_round.dir/correctness/round.cpp.o.provides.build
.PHONY : test/CMakeFiles/correctness_round.dir/correctness/round.cpp.o.provides

test/CMakeFiles/correctness_round.dir/correctness/round.cpp.o.provides.build: test/CMakeFiles/correctness_round.dir/correctness/round.cpp.o


# Object files for target correctness_round
correctness_round_OBJECTS = \
"CMakeFiles/correctness_round.dir/correctness/round.cpp.o"

# External object files for target correctness_round
correctness_round_EXTERNAL_OBJECTS =

bin/correctness_round: test/CMakeFiles/correctness_round.dir/correctness/round.cpp.o
bin/correctness_round: test/CMakeFiles/correctness_round.dir/build.make
bin/correctness_round: lib/libHalide.dylib
bin/correctness_round: test/CMakeFiles/correctness_round.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/correctness_round"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/correctness_round.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/correctness_round.dir/build: bin/correctness_round

.PHONY : test/CMakeFiles/correctness_round.dir/build

test/CMakeFiles/correctness_round.dir/requires: test/CMakeFiles/correctness_round.dir/correctness/round.cpp.o.requires

.PHONY : test/CMakeFiles/correctness_round.dir/requires

test/CMakeFiles/correctness_round.dir/clean:
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && $(CMAKE_COMMAND) -P CMakeFiles/correctness_round.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/correctness_round.dir/clean

test/CMakeFiles/correctness_round.dir/depend:
	cd /Users/were/Halide-SDSoC/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/were/Halide-SDSoC /Users/were/Halide-SDSoC/test /Users/were/Halide-SDSoC/cmake-build-debug /Users/were/Halide-SDSoC/cmake-build-debug/test /Users/were/Halide-SDSoC/cmake-build-debug/test/CMakeFiles/correctness_round.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/correctness_round.dir/depend

