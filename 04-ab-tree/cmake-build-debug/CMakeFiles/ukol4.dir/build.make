# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /opt/clion/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jencmart/matfyz/sem-1/datovky/04-ab-tree

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jencmart/matfyz/sem-1/datovky/04-ab-tree/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ukol4.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ukol4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ukol4.dir/flags.make

CMakeFiles/ukol4.dir/ab_tree_test.cpp.o: CMakeFiles/ukol4.dir/flags.make
CMakeFiles/ukol4.dir/ab_tree_test.cpp.o: ../ab_tree_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jencmart/matfyz/sem-1/datovky/04-ab-tree/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ukol4.dir/ab_tree_test.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ukol4.dir/ab_tree_test.cpp.o -c /home/jencmart/matfyz/sem-1/datovky/04-ab-tree/ab_tree_test.cpp

CMakeFiles/ukol4.dir/ab_tree_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ukol4.dir/ab_tree_test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jencmart/matfyz/sem-1/datovky/04-ab-tree/ab_tree_test.cpp > CMakeFiles/ukol4.dir/ab_tree_test.cpp.i

CMakeFiles/ukol4.dir/ab_tree_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ukol4.dir/ab_tree_test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jencmart/matfyz/sem-1/datovky/04-ab-tree/ab_tree_test.cpp -o CMakeFiles/ukol4.dir/ab_tree_test.cpp.s

CMakeFiles/ukol4.dir/test_main.cpp.o: CMakeFiles/ukol4.dir/flags.make
CMakeFiles/ukol4.dir/test_main.cpp.o: ../test_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jencmart/matfyz/sem-1/datovky/04-ab-tree/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ukol4.dir/test_main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ukol4.dir/test_main.cpp.o -c /home/jencmart/matfyz/sem-1/datovky/04-ab-tree/test_main.cpp

CMakeFiles/ukol4.dir/test_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ukol4.dir/test_main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jencmart/matfyz/sem-1/datovky/04-ab-tree/test_main.cpp > CMakeFiles/ukol4.dir/test_main.cpp.i

CMakeFiles/ukol4.dir/test_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ukol4.dir/test_main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jencmart/matfyz/sem-1/datovky/04-ab-tree/test_main.cpp -o CMakeFiles/ukol4.dir/test_main.cpp.s

# Object files for target ukol4
ukol4_OBJECTS = \
"CMakeFiles/ukol4.dir/ab_tree_test.cpp.o" \
"CMakeFiles/ukol4.dir/test_main.cpp.o"

# External object files for target ukol4
ukol4_EXTERNAL_OBJECTS =

ukol4: CMakeFiles/ukol4.dir/ab_tree_test.cpp.o
ukol4: CMakeFiles/ukol4.dir/test_main.cpp.o
ukol4: CMakeFiles/ukol4.dir/build.make
ukol4: CMakeFiles/ukol4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jencmart/matfyz/sem-1/datovky/04-ab-tree/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ukol4"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ukol4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ukol4.dir/build: ukol4

.PHONY : CMakeFiles/ukol4.dir/build

CMakeFiles/ukol4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ukol4.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ukol4.dir/clean

CMakeFiles/ukol4.dir/depend:
	cd /home/jencmart/matfyz/sem-1/datovky/04-ab-tree/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jencmart/matfyz/sem-1/datovky/04-ab-tree /home/jencmart/matfyz/sem-1/datovky/04-ab-tree /home/jencmart/matfyz/sem-1/datovky/04-ab-tree/cmake-build-debug /home/jencmart/matfyz/sem-1/datovky/04-ab-tree/cmake-build-debug /home/jencmart/matfyz/sem-1/datovky/04-ab-tree/cmake-build-debug/CMakeFiles/ukol4.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ukol4.dir/depend
