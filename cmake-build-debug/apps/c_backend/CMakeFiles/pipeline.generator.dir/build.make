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
include apps/c_backend/CMakeFiles/pipeline.generator.dir/depend.make

# Include the progress variables for this target.
include apps/c_backend/CMakeFiles/pipeline.generator.dir/progress.make

# Include the compile flags for this target's objects.
include apps/c_backend/CMakeFiles/pipeline.generator.dir/flags.make

apps/c_backend/CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.o: apps/c_backend/CMakeFiles/pipeline.generator.dir/flags.make
apps/c_backend/CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.o: ../tools/GenGen.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object apps/c_backend/CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.o"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/c_backend && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.o -c /Users/were/Halide-SDSoC/tools/GenGen.cpp

apps/c_backend/CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.i"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/c_backend && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/were/Halide-SDSoC/tools/GenGen.cpp > CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.i

apps/c_backend/CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.s"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/c_backend && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/were/Halide-SDSoC/tools/GenGen.cpp -o CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.s

apps/c_backend/CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.o.requires:

.PHONY : apps/c_backend/CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.o.requires

apps/c_backend/CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.o.provides: apps/c_backend/CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.o.requires
	$(MAKE) -f apps/c_backend/CMakeFiles/pipeline.generator.dir/build.make apps/c_backend/CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.o.provides.build
.PHONY : apps/c_backend/CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.o.provides

apps/c_backend/CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.o.provides.build: apps/c_backend/CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.o


# Object files for target pipeline.generator
pipeline_generator_OBJECTS = \
"CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.o"

# External object files for target pipeline.generator
pipeline_generator_EXTERNAL_OBJECTS = \
"/Users/were/Halide-SDSoC/cmake-build-debug/apps/c_backend/CMakeFiles/pipeline.generator.objlib.dir/pipeline_generator.cpp.o"

bin/pipeline.generator: apps/c_backend/CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.o
bin/pipeline.generator: apps/c_backend/CMakeFiles/pipeline.generator.objlib.dir/pipeline_generator.cpp.o
bin/pipeline.generator: apps/c_backend/CMakeFiles/pipeline.generator.dir/build.make
bin/pipeline.generator: lib/libHalide.dylib
bin/pipeline.generator: apps/c_backend/CMakeFiles/pipeline.generator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/pipeline.generator"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/c_backend && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pipeline.generator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/c_backend/CMakeFiles/pipeline.generator.dir/build: bin/pipeline.generator

.PHONY : apps/c_backend/CMakeFiles/pipeline.generator.dir/build

apps/c_backend/CMakeFiles/pipeline.generator.dir/requires: apps/c_backend/CMakeFiles/pipeline.generator.dir/__/__/tools/GenGen.cpp.o.requires

.PHONY : apps/c_backend/CMakeFiles/pipeline.generator.dir/requires

apps/c_backend/CMakeFiles/pipeline.generator.dir/clean:
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/c_backend && $(CMAKE_COMMAND) -P CMakeFiles/pipeline.generator.dir/cmake_clean.cmake
.PHONY : apps/c_backend/CMakeFiles/pipeline.generator.dir/clean

apps/c_backend/CMakeFiles/pipeline.generator.dir/depend:
	cd /Users/were/Halide-SDSoC/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/were/Halide-SDSoC /Users/were/Halide-SDSoC/apps/c_backend /Users/were/Halide-SDSoC/cmake-build-debug /Users/were/Halide-SDSoC/cmake-build-debug/apps/c_backend /Users/were/Halide-SDSoC/cmake-build-debug/apps/c_backend/CMakeFiles/pipeline.generator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/c_backend/CMakeFiles/pipeline.generator.dir/depend

