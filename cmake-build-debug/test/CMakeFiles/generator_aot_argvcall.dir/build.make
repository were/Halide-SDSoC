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
include test/CMakeFiles/generator_aot_argvcall.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/generator_aot_argvcall.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/generator_aot_argvcall.dir/flags.make

test/CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.o: test/CMakeFiles/generator_aot_argvcall.dir/flags.make
test/CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.o: ../test/generator/argvcall_aottest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.o"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.o -c /Users/were/Halide-SDSoC/test/generator/argvcall_aottest.cpp

test/CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.i"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/were/Halide-SDSoC/test/generator/argvcall_aottest.cpp > CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.i

test/CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.s"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/were/Halide-SDSoC/test/generator/argvcall_aottest.cpp -o CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.s

test/CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.o.requires:

.PHONY : test/CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.o.requires

test/CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.o.provides: test/CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/generator_aot_argvcall.dir/build.make test/CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.o.provides.build
.PHONY : test/CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.o.provides

test/CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.o.provides.build: test/CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.o


# Object files for target generator_aot_argvcall
generator_aot_argvcall_OBJECTS = \
"CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.o"

# External object files for target generator_aot_argvcall
generator_aot_argvcall_EXTERNAL_OBJECTS =

bin/generator_aot_argvcall: test/CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.o
bin/generator_aot_argvcall: test/CMakeFiles/generator_aot_argvcall.dir/build.make
bin/generator_aot_argvcall: generator_genfiles/argvcall/argvcall.a
bin/generator_aot_argvcall: test/CMakeFiles/generator_aot_argvcall.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/generator_aot_argvcall"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/generator_aot_argvcall.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/generator_aot_argvcall.dir/build: bin/generator_aot_argvcall

.PHONY : test/CMakeFiles/generator_aot_argvcall.dir/build

test/CMakeFiles/generator_aot_argvcall.dir/requires: test/CMakeFiles/generator_aot_argvcall.dir/generator/argvcall_aottest.cpp.o.requires

.PHONY : test/CMakeFiles/generator_aot_argvcall.dir/requires

test/CMakeFiles/generator_aot_argvcall.dir/clean:
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && $(CMAKE_COMMAND) -P CMakeFiles/generator_aot_argvcall.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/generator_aot_argvcall.dir/clean

test/CMakeFiles/generator_aot_argvcall.dir/depend:
	cd /Users/were/Halide-SDSoC/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/were/Halide-SDSoC /Users/were/Halide-SDSoC/test /Users/were/Halide-SDSoC/cmake-build-debug /Users/were/Halide-SDSoC/cmake-build-debug/test /Users/were/Halide-SDSoC/cmake-build-debug/test/CMakeFiles/generator_aot_argvcall.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/generator_aot_argvcall.dir/depend

