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
include test/CMakeFiles/error_overflow_during_constant_folding.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/error_overflow_during_constant_folding.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/error_overflow_during_constant_folding.dir/flags.make

test/CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.o: test/CMakeFiles/error_overflow_during_constant_folding.dir/flags.make
test/CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.o: ../test/error/overflow_during_constant_folding.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.o"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.o -c /Users/were/Halide-SDSoC/test/error/overflow_during_constant_folding.cpp

test/CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.i"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/were/Halide-SDSoC/test/error/overflow_during_constant_folding.cpp > CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.i

test/CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.s"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/were/Halide-SDSoC/test/error/overflow_during_constant_folding.cpp -o CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.s

test/CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.o.requires:

.PHONY : test/CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.o.requires

test/CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.o.provides: test/CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/error_overflow_during_constant_folding.dir/build.make test/CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.o.provides.build
.PHONY : test/CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.o.provides

test/CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.o.provides.build: test/CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.o


# Object files for target error_overflow_during_constant_folding
error_overflow_during_constant_folding_OBJECTS = \
"CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.o"

# External object files for target error_overflow_during_constant_folding
error_overflow_during_constant_folding_EXTERNAL_OBJECTS =

bin/error_overflow_during_constant_folding: test/CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.o
bin/error_overflow_during_constant_folding: test/CMakeFiles/error_overflow_during_constant_folding.dir/build.make
bin/error_overflow_during_constant_folding: lib/libHalide.dylib
bin/error_overflow_during_constant_folding: test/CMakeFiles/error_overflow_during_constant_folding.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/error_overflow_during_constant_folding"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/error_overflow_during_constant_folding.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/error_overflow_during_constant_folding.dir/build: bin/error_overflow_during_constant_folding

.PHONY : test/CMakeFiles/error_overflow_during_constant_folding.dir/build

test/CMakeFiles/error_overflow_during_constant_folding.dir/requires: test/CMakeFiles/error_overflow_during_constant_folding.dir/error/overflow_during_constant_folding.cpp.o.requires

.PHONY : test/CMakeFiles/error_overflow_during_constant_folding.dir/requires

test/CMakeFiles/error_overflow_during_constant_folding.dir/clean:
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && $(CMAKE_COMMAND) -P CMakeFiles/error_overflow_during_constant_folding.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/error_overflow_during_constant_folding.dir/clean

test/CMakeFiles/error_overflow_during_constant_folding.dir/depend:
	cd /Users/were/Halide-SDSoC/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/were/Halide-SDSoC /Users/were/Halide-SDSoC/test /Users/were/Halide-SDSoC/cmake-build-debug /Users/were/Halide-SDSoC/cmake-build-debug/test /Users/were/Halide-SDSoC/cmake-build-debug/test/CMakeFiles/error_overflow_during_constant_folding.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/error_overflow_during_constant_folding.dir/depend

