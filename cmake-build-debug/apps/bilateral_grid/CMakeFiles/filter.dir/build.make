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
include apps/bilateral_grid/CMakeFiles/filter.dir/depend.make

# Include the progress variables for this target.
include apps/bilateral_grid/CMakeFiles/filter.dir/progress.make

# Include the compile flags for this target's objects.
include apps/bilateral_grid/CMakeFiles/filter.dir/flags.make

apps/bilateral_grid/CMakeFiles/filter.dir/filter.cpp.o: apps/bilateral_grid/CMakeFiles/filter.dir/flags.make
apps/bilateral_grid/CMakeFiles/filter.dir/filter.cpp.o: ../apps/bilateral_grid/filter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object apps/bilateral_grid/CMakeFiles/filter.dir/filter.cpp.o"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/bilateral_grid && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/filter.dir/filter.cpp.o -c /Users/were/Halide-SDSoC/apps/bilateral_grid/filter.cpp

apps/bilateral_grid/CMakeFiles/filter.dir/filter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/filter.dir/filter.cpp.i"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/bilateral_grid && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/were/Halide-SDSoC/apps/bilateral_grid/filter.cpp > CMakeFiles/filter.dir/filter.cpp.i

apps/bilateral_grid/CMakeFiles/filter.dir/filter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/filter.dir/filter.cpp.s"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/bilateral_grid && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/were/Halide-SDSoC/apps/bilateral_grid/filter.cpp -o CMakeFiles/filter.dir/filter.cpp.s

apps/bilateral_grid/CMakeFiles/filter.dir/filter.cpp.o.requires:

.PHONY : apps/bilateral_grid/CMakeFiles/filter.dir/filter.cpp.o.requires

apps/bilateral_grid/CMakeFiles/filter.dir/filter.cpp.o.provides: apps/bilateral_grid/CMakeFiles/filter.dir/filter.cpp.o.requires
	$(MAKE) -f apps/bilateral_grid/CMakeFiles/filter.dir/build.make apps/bilateral_grid/CMakeFiles/filter.dir/filter.cpp.o.provides.build
.PHONY : apps/bilateral_grid/CMakeFiles/filter.dir/filter.cpp.o.provides

apps/bilateral_grid/CMakeFiles/filter.dir/filter.cpp.o.provides.build: apps/bilateral_grid/CMakeFiles/filter.dir/filter.cpp.o


# Object files for target filter
filter_OBJECTS = \
"CMakeFiles/filter.dir/filter.cpp.o"

# External object files for target filter
filter_EXTERNAL_OBJECTS =

bin/filter: apps/bilateral_grid/CMakeFiles/filter.dir/filter.cpp.o
bin/filter: apps/bilateral_grid/CMakeFiles/filter.dir/build.make
bin/filter: generator_genfiles/bilateral_grid/bilateral_grid.a
bin/filter: /usr/local/lib/libpng.dylib
bin/filter: /usr/lib/libz.dylib
bin/filter: /usr/local/lib/libjpeg.dylib
bin/filter: apps/bilateral_grid/CMakeFiles/filter.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/filter"
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/bilateral_grid && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/filter.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/bilateral_grid/CMakeFiles/filter.dir/build: bin/filter

.PHONY : apps/bilateral_grid/CMakeFiles/filter.dir/build

apps/bilateral_grid/CMakeFiles/filter.dir/requires: apps/bilateral_grid/CMakeFiles/filter.dir/filter.cpp.o.requires

.PHONY : apps/bilateral_grid/CMakeFiles/filter.dir/requires

apps/bilateral_grid/CMakeFiles/filter.dir/clean:
	cd /Users/were/Halide-SDSoC/cmake-build-debug/apps/bilateral_grid && $(CMAKE_COMMAND) -P CMakeFiles/filter.dir/cmake_clean.cmake
.PHONY : apps/bilateral_grid/CMakeFiles/filter.dir/clean

apps/bilateral_grid/CMakeFiles/filter.dir/depend:
	cd /Users/were/Halide-SDSoC/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/were/Halide-SDSoC /Users/were/Halide-SDSoC/apps/bilateral_grid /Users/were/Halide-SDSoC/cmake-build-debug /Users/were/Halide-SDSoC/cmake-build-debug/apps/bilateral_grid /Users/were/Halide-SDSoC/cmake-build-debug/apps/bilateral_grid/CMakeFiles/filter.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : apps/bilateral_grid/CMakeFiles/filter.dir/depend

