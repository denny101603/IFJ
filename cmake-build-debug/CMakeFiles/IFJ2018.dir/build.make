# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.12

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2018.2.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2018.2.4\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\danbu\CLionProjects\IFJ\IFJ

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\danbu\CLionProjects\IFJ\IFJ\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/IFJ2018.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/IFJ2018.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/IFJ2018.dir/flags.make

CMakeFiles/IFJ2018.dir/main.c.obj: CMakeFiles/IFJ2018.dir/flags.make
CMakeFiles/IFJ2018.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\danbu\CLionProjects\IFJ\IFJ\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/IFJ2018.dir/main.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\IFJ2018.dir\main.c.obj   -c C:\Users\danbu\CLionProjects\IFJ\IFJ\main.c

CMakeFiles/IFJ2018.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/IFJ2018.dir/main.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\danbu\CLionProjects\IFJ\IFJ\main.c > CMakeFiles\IFJ2018.dir\main.c.i

CMakeFiles/IFJ2018.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/IFJ2018.dir/main.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\danbu\CLionProjects\IFJ\IFJ\main.c -o CMakeFiles\IFJ2018.dir\main.c.s

CMakeFiles/IFJ2018.dir/fsm.c.obj: CMakeFiles/IFJ2018.dir/flags.make
CMakeFiles/IFJ2018.dir/fsm.c.obj: ../fsm.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\danbu\CLionProjects\IFJ\IFJ\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/IFJ2018.dir/fsm.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\IFJ2018.dir\fsm.c.obj   -c C:\Users\danbu\CLionProjects\IFJ\IFJ\fsm.c

CMakeFiles/IFJ2018.dir/fsm.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/IFJ2018.dir/fsm.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\danbu\CLionProjects\IFJ\IFJ\fsm.c > CMakeFiles\IFJ2018.dir\fsm.c.i

CMakeFiles/IFJ2018.dir/fsm.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/IFJ2018.dir/fsm.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\danbu\CLionProjects\IFJ\IFJ\fsm.c -o CMakeFiles\IFJ2018.dir\fsm.c.s

# Object files for target IFJ2018
IFJ2018_OBJECTS = \
"CMakeFiles/IFJ2018.dir/main.c.obj" \
"CMakeFiles/IFJ2018.dir/fsm.c.obj"

# External object files for target IFJ2018
IFJ2018_EXTERNAL_OBJECTS =

IFJ2018.exe: CMakeFiles/IFJ2018.dir/main.c.obj
IFJ2018.exe: CMakeFiles/IFJ2018.dir/fsm.c.obj
IFJ2018.exe: CMakeFiles/IFJ2018.dir/build.make
IFJ2018.exe: CMakeFiles/IFJ2018.dir/linklibs.rsp
IFJ2018.exe: CMakeFiles/IFJ2018.dir/objects1.rsp
IFJ2018.exe: CMakeFiles/IFJ2018.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\danbu\CLionProjects\IFJ\IFJ\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable IFJ2018.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\IFJ2018.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/IFJ2018.dir/build: IFJ2018.exe

.PHONY : CMakeFiles/IFJ2018.dir/build

CMakeFiles/IFJ2018.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\IFJ2018.dir\cmake_clean.cmake
.PHONY : CMakeFiles/IFJ2018.dir/clean

CMakeFiles/IFJ2018.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\danbu\CLionProjects\IFJ\IFJ C:\Users\danbu\CLionProjects\IFJ\IFJ C:\Users\danbu\CLionProjects\IFJ\IFJ\cmake-build-debug C:\Users\danbu\CLionProjects\IFJ\IFJ\cmake-build-debug C:\Users\danbu\CLionProjects\IFJ\IFJ\cmake-build-debug\CMakeFiles\IFJ2018.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/IFJ2018.dir/depend

