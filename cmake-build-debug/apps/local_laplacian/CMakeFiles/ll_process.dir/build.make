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
include apps/local_laplacian/CMakeFiles/ll_process.dir/depend.make

# Include the progress variables for this target.
include apps/local_laplacian/CMakeFiles/ll_process.dir/progress.make

# Include the compile flags for this target's objects.
include apps/local_laplacian/CMakeFiles/ll_process.dir/flags.make

apps/local_laplacian/CMakeFiles/ll_process.dir/process.cpp.o: apps/local_laplacian/CMakeFiles/ll_process.dir/flags.make
apps/local_laplacian/CMakeFiles/ll_process.dir/process.cpp.o: ../apps/local_laplacian/process.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object apps/local_laplacian/CMakeFiles/ll_process.dir/process.cpp.o"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/local_laplacian && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ll_process.dir/process.cpp.o -c /Users/were/Halide-SDSoC/apps/local_laplacian/process.cpp

apps/local_laplacian/CMakeFiles/ll_process.dir/process.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ll_process.dir/process.cpp.i"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/local_laplacian && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/were/Halide-SDSoC/apps/local_laplacian/process.cpp > CMakeFiles/ll_process.dir/process.cpp.i

apps/local_laplacian/CMakeFiles/ll_process.dir/process.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ll_process.dir/process.cpp.s"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/local_laplacian && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/were/Halide-SDSoC/apps/local_laplacian/process.cpp -o CMakeFiles/ll_process.dir/process.cpp.s

apps/local_laplacian/CMakeFiles/ll_process.dir/process.cpp.o.requires:

.PHONY : apps/local_laplacian/CMakeFiles/ll_process.dir/process.cpp.o.requires

apps/local_laplacian/CMakeFiles/ll_process.dir/process.cpp.o.provides: apps/local_laplacian/CMakeFiles/ll_process.dir/process.cpp.o.requires
	$(MAKE) -f apps/local_laplacian/CMakeFiles/ll_process.dir/build.make apps/local_laplacian/CMakeFiles/ll_process.dir/process.cpp.o.provides.build
.PHONY : apps/local_laplacian/CMakeFiles/ll_process.dir/process.cpp.o.provides

apps/local_laplacian/CMakeFiles/ll_process.dir/process.cpp.o.provides.build: apps/local_laplacian/CMakeFiles/ll_process.dir/process.cpp.o


# Object files for target ll_process
ll_process_OBJECTS = \
"CMakeFiles/ll_process.dir/process.cpp.o"

# External object files for target ll_process
ll_process_EXTERNAL_OBJECTS =

bin/ll_process: apps/local_laplacian/CMakeFiles/ll_process.dir/process.cpp.o
bin/ll_process: apps/local_laplacian/CMakeFiles/ll_process.dir/build.make
bin/ll_process: generator_genfiles/local_laplacian/local_laplacian.a
bin/ll_process: /usr/local/lib/libpng.dylib
bin/ll_process: /usr/lib/libz.dylib
bin/ll_process: /usr/local/lib/libjpeg.dylib
bin/ll_process: apps/local_laplacian/CMakeFiles/ll_process.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/ll_process"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/local_laplacian && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ll_process.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/local_laplacian/CMakeFiles/ll_process.dir/build: bin/ll_process

.PHONY : apps/local_laplacian/CMakeFiles/ll_process.dir/build

apps/local_laplacian/CMakeFiles/ll_process.dir/requires: apps/local_laplacian/CMakeFiles/ll_process.dir/process.cpp.o.requires

.PHONY : apps/local_laplacian/CMakeFiles/ll_process.dir/requires

apps/local_laplacian/CMakeFiles/ll_process.dir/clean:
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/local_laplacian && $(CMAKE_COMMAND) -P CMakeFiles/ll_process.dir/cmake_clean.cmake
.PHONY : apps/local_laplacian/CMakeFiles/ll_process.dir/clean

apps/local_laplacian/CMakeFiles/ll_process.dir/depend:
	cd /Users/were/Halide-SDSoC/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/were/Halide-SDSoC /Users/were/Halide-SDSoC/apps/local_laplacian /Users/were/Halide-SDSoC/cmake-build-debug /Users/were/Halide-SDSoC/cmake-build-debug/apps/local_laplacian /Users/were/Halide-SDSoC/cmake-build-debug/apps/local_laplacian/CMakeFiles/ll_process.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/local_laplacian/CMakeFiles/ll_process.dir/depend

