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
include test/CMakeFiles/error_float16_t_implicit_downcast.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/error_float16_t_implicit_downcast.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/error_float16_t_implicit_downcast.dir/flags.make

test/CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.o: test/CMakeFiles/error_float16_t_implicit_downcast.dir/flags.make
test/CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.o: ../test/error/float16_t_implicit_downcast.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.o"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.o -c /Users/were/Halide-SDSoC/test/error/float16_t_implicit_downcast.cpp

test/CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.i"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/were/Halide-SDSoC/test/error/float16_t_implicit_downcast.cpp > CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.i

test/CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.s"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/were/Halide-SDSoC/test/error/float16_t_implicit_downcast.cpp -o CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.s

test/CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.o.requires:

.PHONY : test/CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.o.requires

test/CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.o.provides: test/CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/error_float16_t_implicit_downcast.dir/build.make test/CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.o.provides.build
.PHONY : test/CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.o.provides

test/CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.o.provides.build: test/CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.o


# Object files for target error_float16_t_implicit_downcast
error_float16_t_implicit_downcast_OBJECTS = \
"CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.o"

# External object files for target error_float16_t_implicit_downcast
error_float16_t_implicit_downcast_EXTERNAL_OBJECTS =

bin/error_float16_t_implicit_downcast: test/CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.o
bin/error_float16_t_implicit_downcast: test/CMakeFiles/error_float16_t_implicit_downcast.dir/build.make
bin/error_float16_t_implicit_downcast: lib/libHalide.dylib
bin/error_float16_t_implicit_downcast: test/CMakeFiles/error_float16_t_implicit_downcast.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/error_float16_t_implicit_downcast"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/error_float16_t_implicit_downcast.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/error_float16_t_implicit_downcast.dir/build: bin/error_float16_t_implicit_downcast

.PHONY : test/CMakeFiles/error_float16_t_implicit_downcast.dir/build

test/CMakeFiles/error_float16_t_implicit_downcast.dir/requires: test/CMakeFiles/error_float16_t_implicit_downcast.dir/error/float16_t_implicit_downcast.cpp.o.requires

.PHONY : test/CMakeFiles/error_float16_t_implicit_downcast.dir/requires

test/CMakeFiles/error_float16_t_implicit_downcast.dir/clean:
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && $(CMAKE_COMMAND) -P CMakeFiles/error_float16_t_implicit_downcast.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/error_float16_t_implicit_downcast.dir/clean

test/CMakeFiles/error_float16_t_implicit_downcast.dir/depend:
	cd /Users/were/Halide-SDSoC/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/were/Halide-SDSoC /Users/were/Halide-SDSoC/test /Users/were/Halide-SDSoC/cmake-build-debug /Users/were/Halide-SDSoC/cmake-build-debug/test /Users/were/Halide-SDSoC/cmake-build-debug/test/CMakeFiles/error_float16_t_implicit_downcast.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/error_float16_t_implicit_downcast.dir/depend

