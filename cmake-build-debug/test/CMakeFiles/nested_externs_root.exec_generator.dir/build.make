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

# Utility rule file for nested_externs_root.exec_generator.

# Include the progress variables for this target.
include test/CMakeFiles/nested_externs_root.exec_generator.dir/progress.make

test/CMakeFiles/nested_externs_root.exec_generator: generator_genfiles/nested_externs_root/nested_externs_root.a
test/CMakeFiles/nested_externs_root.exec_generator: generator_genfiles/nested_externs_root/nested_externs_root.h


generator_genfiles/nested_externs_root/nested_externs_root.a: bin/nested_externs.generator
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/were/Halide-SDSoC/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Executing Generator nested_externs.generator with args -o;/Users/were/Halide-SDSoC/cmake-build-debug/generator_genfiles/nested_externs_root;-f;nested_externs_root;-g;nested_externs_root;target=host..."
	cd /Users/were/Halide-SDSoC/cmake-build-debug/generator_genfiles/nested_externs_root && /Users/were/Halide-SDSoC/cmake-build-debug/bin/nested_externs.generator -o /Users/were/Halide-SDSoC/cmake-build-debug/generator_genfiles/nested_externs_root -f nested_externs_root -g nested_externs_root target=host

generator_genfiles/nested_externs_root/nested_externs_root.h: generator_genfiles/nested_externs_root/nested_externs_root.a
	@$(CMAKE_COMMAND) -E touch_nocreate generator_genfiles/nested_externs_root/nested_externs_root.h

nested_externs_root.exec_generator: test/CMakeFiles/nested_externs_root.exec_generator
nested_externs_root.exec_generator: generator_genfiles/nested_externs_root/nested_externs_root.a
nested_externs_root.exec_generator: generator_genfiles/nested_externs_root/nested_externs_root.h
nested_externs_root.exec_generator: test/CMakeFiles/nested_externs_root.exec_generator.dir/build.make

.PHONY : nested_externs_root.exec_generator

# Rule to build all files generated by this target.
test/CMakeFiles/nested_externs_root.exec_generator.dir/build: nested_externs_root.exec_generator

.PHONY : test/CMakeFiles/nested_externs_root.exec_generator.dir/build

test/CMakeFiles/nested_externs_root.exec_generator.dir/clean:
	cd /Users/were/Halide-SDSoC/cmake-build-debug/test && $(CMAKE_COMMAND) -P CMakeFiles/nested_externs_root.exec_generator.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/nested_externs_root.exec_generator.dir/clean

test/CMakeFiles/nested_externs_root.exec_generator.dir/depend:
	cd /Users/were/Halide-SDSoC/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/were/Halide-SDSoC /Users/were/Halide-SDSoC/test /Users/were/Halide-SDSoC/cmake-build-debug /Users/were/Halide-SDSoC/cmake-build-debug/test /Users/were/Halide-SDSoC/cmake-build-debug/test/CMakeFiles/nested_externs_root.exec_generator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/nested_externs_root.exec_generator.dir/depend

