# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12"

# Include any dependencies generated for this target.
include src/CMakeFiles/dolphin.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/dolphin.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/dolphin.dir/flags.make

src/CMakeFiles/dolphin.dir/dolphin_dummy.cpp.o: src/CMakeFiles/dolphin.dir/flags.make
src/CMakeFiles/dolphin.dir/dolphin_dummy.cpp.o: src/dolphin_dummy.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/dolphin.dir/dolphin_dummy.cpp.o"
	cd "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/src" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dolphin.dir/dolphin_dummy.cpp.o -c "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/src/dolphin_dummy.cpp"

src/CMakeFiles/dolphin.dir/dolphin_dummy.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dolphin.dir/dolphin_dummy.cpp.i"
	cd "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/src/dolphin_dummy.cpp" > CMakeFiles/dolphin.dir/dolphin_dummy.cpp.i

src/CMakeFiles/dolphin.dir/dolphin_dummy.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dolphin.dir/dolphin_dummy.cpp.s"
	cd "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/src/dolphin_dummy.cpp" -o CMakeFiles/dolphin.dir/dolphin_dummy.cpp.s

src/CMakeFiles/dolphin.dir/dolphin_dummy.cpp.o.requires:

.PHONY : src/CMakeFiles/dolphin.dir/dolphin_dummy.cpp.o.requires

src/CMakeFiles/dolphin.dir/dolphin_dummy.cpp.o.provides: src/CMakeFiles/dolphin.dir/dolphin_dummy.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/dolphin.dir/build.make src/CMakeFiles/dolphin.dir/dolphin_dummy.cpp.o.provides.build
.PHONY : src/CMakeFiles/dolphin.dir/dolphin_dummy.cpp.o.provides

src/CMakeFiles/dolphin.dir/dolphin_dummy.cpp.o.provides.build: src/CMakeFiles/dolphin.dir/dolphin_dummy.cpp.o


src/CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.o: src/CMakeFiles/dolphin.dir/flags.make
src/CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.o: src/dolphin_autogen/moc_compilation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.o"
	cd "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/src" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.o -c "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/src/dolphin_autogen/moc_compilation.cpp"

src/CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.i"
	cd "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/src/dolphin_autogen/moc_compilation.cpp" > CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.i

src/CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.s"
	cd "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/src/dolphin_autogen/moc_compilation.cpp" -o CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.s

src/CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.o.requires:

.PHONY : src/CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.o.requires

src/CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.o.provides: src/CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/dolphin.dir/build.make src/CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.o.provides.build
.PHONY : src/CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.o.provides

src/CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.o.provides.build: src/CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.o


# Object files for target dolphin
dolphin_OBJECTS = \
"CMakeFiles/dolphin.dir/dolphin_dummy.cpp.o" \
"CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.o"

# External object files for target dolphin
dolphin_EXTERNAL_OBJECTS =

src/dolphin: src/CMakeFiles/dolphin.dir/dolphin_dummy.cpp.o
src/dolphin: src/CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.o
src/dolphin: src/CMakeFiles/dolphin.dir/build.make
src/dolphin: src/libdolphinstatic.a
src/dolphin: src/CMakeFiles/dolphin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable dolphin"
	cd "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/src" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dolphin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/dolphin.dir/build: src/dolphin

.PHONY : src/CMakeFiles/dolphin.dir/build

src/CMakeFiles/dolphin.dir/requires: src/CMakeFiles/dolphin.dir/dolphin_dummy.cpp.o.requires
src/CMakeFiles/dolphin.dir/requires: src/CMakeFiles/dolphin.dir/dolphin_autogen/moc_compilation.cpp.o.requires

.PHONY : src/CMakeFiles/dolphin.dir/requires

src/CMakeFiles/dolphin.dir/clean:
	cd "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/src" && $(CMAKE_COMMAND) -P CMakeFiles/dolphin.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/dolphin.dir/clean

src/CMakeFiles/dolphin.dir/depend:
	cd "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager" "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/src" "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12" "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/src" "/home/kyan/Ember Library/Ember satellite projects/wreathe-file-manager/cmake-2017jan12/src/CMakeFiles/dolphin.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : src/CMakeFiles/dolphin.dir/depend

