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
include test/CMakeFiles/can_use_target.generator.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/can_use_target.generator.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/can_use_target.generator.dir/flags.make

test/CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.o: test/CMakeFiles/can_use_target.generator.dir/flags.make
test/CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.o: ../tools/GenGen.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.o"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.o -c /Users/were/Halide-SDSoC/tools/GenGen.cpp

test/CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.i"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/were/Halide-SDSoC/tools/GenGen.cpp > CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.i

test/CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.s"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/were/Halide-SDSoC/tools/GenGen.cpp -o CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.s

test/CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.o.requires:

.PHONY : test/CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.o.requires

test/CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.o.provides: test/CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.o.requires
	$(MAKE) -f test/CMakeFiles/can_use_target.generator.dir/build.make test/CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.o.provides.build
.PHONY : test/CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.o.provides

test/CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.o.provides.build: test/CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.o


# Object files for target can_use_target.generator
can_use_target_generator_OBJECTS = \
"CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.o"

# External object files for target can_use_target.generator
can_use_target_generator_EXTERNAL_OBJECTS = \
"/Users/were/Halide-SDSoC/cmake-build-debug/test/CMakeFiles/can_use_target.generator.objlib.dir/generator/can_use_target_generator.cpp.o"

bin/can_use_target.generator: test/CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.o
bin/can_use_target.generator: test/CMakeFiles/can_use_target.generator.objlib.dir/generator/can_use_target_generator.cpp.o
bin/can_use_target.generator: test/CMakeFiles/can_use_target.generator.dir/build.make
bin/can_use_target.generator: lib/libHalide.dylib
bin/can_use_target.generator: test/CMakeFiles/can_use_target.generator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/can_use_target.generator"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/can_use_target.generator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/can_use_target.generator.dir/build: bin/can_use_target.generator

.PHONY : test/CMakeFiles/can_use_target.generator.dir/build

test/CMakeFiles/can_use_target.generator.dir/requires: test/CMakeFiles/can_use_target.generator.dir/__/tools/GenGen.cpp.o.requires

.PHONY : test/CMakeFiles/can_use_target.generator.dir/requires

test/CMakeFiles/can_use_target.generator.dir/clean:
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && $(CMAKE_COMMAND) -P CMakeFiles/can_use_target.generator.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/can_use_target.generator.dir/clean

test/CMakeFiles/can_use_target.generator.dir/depend:
	cd /Users/were/Halide-SDSoC/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/were/Halide-SDSoC /Users/were/Halide-SDSoC/test /Users/were/Halide-SDSoC/cmake-build-debug /Users/were/Halide-SDSoC/cmake-build-debug/test /Users/were/Halide-SDSoC/cmake-build-debug/test/CMakeFiles/can_use_target.generator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/can_use_target.generator.dir/depend

