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
include apps/blur/CMakeFiles/halide_blur.generator.dir/depend.make

# Include the progress variables for this target.
include apps/blur/CMakeFiles/halide_blur.generator.dir/progress.make

# Include the compile flags for this target's objects.
include apps/blur/CMakeFiles/halide_blur.generator.dir/flags.make

apps/blur/CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.o: apps/blur/CMakeFiles/halide_blur.generator.dir/flags.make
apps/blur/CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.o: ../tools/GenGen.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object apps/blur/CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.o"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/blur && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.o -c /Users/were/Halide-SDSoC/tools/GenGen.cpp

apps/blur/CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.i"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/blur && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/were/Halide-SDSoC/tools/GenGen.cpp > CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.i

apps/blur/CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.s"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/blur && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/were/Halide-SDSoC/tools/GenGen.cpp -o CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.s

apps/blur/CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.o.requires:

.PHONY : apps/blur/CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.o.requires

apps/blur/CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.o.provides: apps/blur/CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.o.requires
	$(MAKE) -f apps/blur/CMakeFiles/halide_blur.generator.dir/build.make apps/blur/CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.o.provides.build
.PHONY : apps/blur/CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.o.provides

apps/blur/CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.o.provides.build: apps/blur/CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.o


# Object files for target halide_blur.generator
halide_blur_generator_OBJECTS = \
"CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.o"

# External object files for target halide_blur.generator
halide_blur_generator_EXTERNAL_OBJECTS = \
"/Users/were/Halide-SDSoC/cmake-build-debug/apps/blur/CMakeFiles/halide_blur.generator.objlib.dir/halide_blur_generator.cpp.o"

bin/halide_blur.generator: apps/blur/CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.o
bin/halide_blur.generator: apps/blur/CMakeFiles/halide_blur.generator.objlib.dir/halide_blur_generator.cpp.o
bin/halide_blur.generator: apps/blur/CMakeFiles/halide_blur.generator.dir/build.make
bin/halide_blur.generator: lib/libHalide.dylib
bin/halide_blur.generator: apps/blur/CMakeFiles/halide_blur.generator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/halide_blur.generator"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/blur && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/halide_blur.generator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/blur/CMakeFiles/halide_blur.generator.dir/build: bin/halide_blur.generator

.PHONY : apps/blur/CMakeFiles/halide_blur.generator.dir/build

apps/blur/CMakeFiles/halide_blur.generator.dir/requires: apps/blur/CMakeFiles/halide_blur.generator.dir/__/__/tools/GenGen.cpp.o.requires

.PHONY : apps/blur/CMakeFiles/halide_blur.generator.dir/requires

apps/blur/CMakeFiles/halide_blur.generator.dir/clean:
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/blur && $(CMAKE_COMMAND) -P CMakeFiles/halide_blur.generator.dir/cmake_clean.cmake
.PHONY : apps/blur/CMakeFiles/halide_blur.generator.dir/clean

apps/blur/CMakeFiles/halide_blur.generator.dir/depend:
	cd /Users/were/Halide-SDSoC/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/were/Halide-SDSoC /Users/were/Halide-SDSoC/apps/blur /Users/were/Halide-SDSoC/cmake-build-debug /Users/were/Halide-SDSoC/cmake-build-debug/apps/blur /Users/were/Halide-SDSoC/cmake-build-debug/apps/blur/CMakeFiles/halide_blur.generator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/blur/CMakeFiles/halide_blur.generator.dir/depend

