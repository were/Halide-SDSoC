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
include test/CMakeFiles/opengl_internal.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/opengl_internal.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/opengl_internal.dir/flags.make

test/CMakeFiles/opengl_internal.dir/opengl/internal.cpp.o: test/CMakeFiles/opengl_internal.dir/flags.make
test/CMakeFiles/opengl_internal.dir/opengl/internal.cpp.o: ../test/opengl/internal.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/opengl_internal.dir/opengl/internal.cpp.o"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/opengl_internal.dir/opengl/internal.cpp.o -c /Users/were/Halide-SDSoC/test/opengl/internal.cpp

test/CMakeFiles/opengl_internal.dir/opengl/internal.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opengl_internal.dir/opengl/internal.cpp.i"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/were/Halide-SDSoC/test/opengl/internal.cpp > CMakeFiles/opengl_internal.dir/opengl/internal.cpp.i

test/CMakeFiles/opengl_internal.dir/opengl/internal.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opengl_internal.dir/opengl/internal.cpp.s"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/were/Halide-SDSoC/test/opengl/internal.cpp -o CMakeFiles/opengl_internal.dir/opengl/internal.cpp.s

test/CMakeFiles/opengl_internal.dir/opengl/internal.cpp.o.requires:

.PHONY : test/CMakeFiles/opengl_internal.dir/opengl/internal.cpp.o.requires

test/CMakeFiles/opengl_internal.dir/opengl/internal.cpp.o.provides: test/CMakeFiles/opengl_internal.dir/opengl/internal.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/opengl_internal.dir/build.make test/CMakeFiles/opengl_internal.dir/opengl/internal.cpp.o.provides.build
.PHONY : test/CMakeFiles/opengl_internal.dir/opengl/internal.cpp.o.provides

test/CMakeFiles/opengl_internal.dir/opengl/internal.cpp.o.provides.build: test/CMakeFiles/opengl_internal.dir/opengl/internal.cpp.o


# Object files for target opengl_internal
opengl_internal_OBJECTS = \
"CMakeFiles/opengl_internal.dir/opengl/internal.cpp.o"

# External object files for target opengl_internal
opengl_internal_EXTERNAL_OBJECTS =

bin/opengl_internal: test/CMakeFiles/opengl_internal.dir/opengl/internal.cpp.o
bin/opengl_internal: test/CMakeFiles/opengl_internal.dir/build.make
bin/opengl_internal: lib/libHalide.dylib
bin/opengl_internal: test/CMakeFiles/opengl_internal.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/opengl_internal"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/opengl_internal.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/opengl_internal.dir/build: bin/opengl_internal

.PHONY : test/CMakeFiles/opengl_internal.dir/build

test/CMakeFiles/opengl_internal.dir/requires: test/CMakeFiles/opengl_internal.dir/opengl/internal.cpp.o.requires

.PHONY : test/CMakeFiles/opengl_internal.dir/requires

test/CMakeFiles/opengl_internal.dir/clean:
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && $(CMAKE_COMMAND) -P CMakeFiles/opengl_internal.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/opengl_internal.dir/clean

test/CMakeFiles/opengl_internal.dir/depend:
	cd /Users/were/Halide-SDSoC/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/were/Halide-SDSoC /Users/were/Halide-SDSoC/test /Users/were/Halide-SDSoC/cmake-build-debug /Users/were/Halide-SDSoC/cmake-build-debug/test /Users/were/Halide-SDSoC/cmake-build-debug/test/CMakeFiles/opengl_internal.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/opengl_internal.dir/depend

