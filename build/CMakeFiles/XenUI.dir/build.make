# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sarowar/Documents/XenonUI

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sarowar/Documents/XenonUI/build

# Include any dependencies generated for this target.
include CMakeFiles/XenUI.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/XenUI.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/XenUI.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/XenUI.dir/flags.make

CMakeFiles/XenUI.dir/src/graphics/sdl2/TextRenderer.cpp.o: CMakeFiles/XenUI.dir/flags.make
CMakeFiles/XenUI.dir/src/graphics/sdl2/TextRenderer.cpp.o: /home/sarowar/Documents/XenonUI/src/graphics/sdl2/TextRenderer.cpp
CMakeFiles/XenUI.dir/src/graphics/sdl2/TextRenderer.cpp.o: CMakeFiles/XenUI.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/sarowar/Documents/XenonUI/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/XenUI.dir/src/graphics/sdl2/TextRenderer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/XenUI.dir/src/graphics/sdl2/TextRenderer.cpp.o -MF CMakeFiles/XenUI.dir/src/graphics/sdl2/TextRenderer.cpp.o.d -o CMakeFiles/XenUI.dir/src/graphics/sdl2/TextRenderer.cpp.o -c /home/sarowar/Documents/XenonUI/src/graphics/sdl2/TextRenderer.cpp

CMakeFiles/XenUI.dir/src/graphics/sdl2/TextRenderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/XenUI.dir/src/graphics/sdl2/TextRenderer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sarowar/Documents/XenonUI/src/graphics/sdl2/TextRenderer.cpp > CMakeFiles/XenUI.dir/src/graphics/sdl2/TextRenderer.cpp.i

CMakeFiles/XenUI.dir/src/graphics/sdl2/TextRenderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/XenUI.dir/src/graphics/sdl2/TextRenderer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sarowar/Documents/XenonUI/src/graphics/sdl2/TextRenderer.cpp -o CMakeFiles/XenUI.dir/src/graphics/sdl2/TextRenderer.cpp.s

CMakeFiles/XenUI.dir/src/platform/linux/linux.cpp.o: CMakeFiles/XenUI.dir/flags.make
CMakeFiles/XenUI.dir/src/platform/linux/linux.cpp.o: /home/sarowar/Documents/XenonUI/src/platform/linux/linux.cpp
CMakeFiles/XenUI.dir/src/platform/linux/linux.cpp.o: CMakeFiles/XenUI.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/sarowar/Documents/XenonUI/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/XenUI.dir/src/platform/linux/linux.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/XenUI.dir/src/platform/linux/linux.cpp.o -MF CMakeFiles/XenUI.dir/src/platform/linux/linux.cpp.o.d -o CMakeFiles/XenUI.dir/src/platform/linux/linux.cpp.o -c /home/sarowar/Documents/XenonUI/src/platform/linux/linux.cpp

CMakeFiles/XenUI.dir/src/platform/linux/linux.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/XenUI.dir/src/platform/linux/linux.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sarowar/Documents/XenonUI/src/platform/linux/linux.cpp > CMakeFiles/XenUI.dir/src/platform/linux/linux.cpp.i

CMakeFiles/XenUI.dir/src/platform/linux/linux.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/XenUI.dir/src/platform/linux/linux.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sarowar/Documents/XenonUI/src/platform/linux/linux.cpp -o CMakeFiles/XenUI.dir/src/platform/linux/linux.cpp.s

CMakeFiles/XenUI.dir/src/ui/Label.cpp.o: CMakeFiles/XenUI.dir/flags.make
CMakeFiles/XenUI.dir/src/ui/Label.cpp.o: /home/sarowar/Documents/XenonUI/src/ui/Label.cpp
CMakeFiles/XenUI.dir/src/ui/Label.cpp.o: CMakeFiles/XenUI.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/sarowar/Documents/XenonUI/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/XenUI.dir/src/ui/Label.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/XenUI.dir/src/ui/Label.cpp.o -MF CMakeFiles/XenUI.dir/src/ui/Label.cpp.o.d -o CMakeFiles/XenUI.dir/src/ui/Label.cpp.o -c /home/sarowar/Documents/XenonUI/src/ui/Label.cpp

CMakeFiles/XenUI.dir/src/ui/Label.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/XenUI.dir/src/ui/Label.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sarowar/Documents/XenonUI/src/ui/Label.cpp > CMakeFiles/XenUI.dir/src/ui/Label.cpp.i

CMakeFiles/XenUI.dir/src/ui/Label.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/XenUI.dir/src/ui/Label.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sarowar/Documents/XenonUI/src/ui/Label.cpp -o CMakeFiles/XenUI.dir/src/ui/Label.cpp.s

CMakeFiles/XenUI.dir/src/ui/Button.cpp.o: CMakeFiles/XenUI.dir/flags.make
CMakeFiles/XenUI.dir/src/ui/Button.cpp.o: /home/sarowar/Documents/XenonUI/src/ui/Button.cpp
CMakeFiles/XenUI.dir/src/ui/Button.cpp.o: CMakeFiles/XenUI.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/sarowar/Documents/XenonUI/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/XenUI.dir/src/ui/Button.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/XenUI.dir/src/ui/Button.cpp.o -MF CMakeFiles/XenUI.dir/src/ui/Button.cpp.o.d -o CMakeFiles/XenUI.dir/src/ui/Button.cpp.o -c /home/sarowar/Documents/XenonUI/src/ui/Button.cpp

CMakeFiles/XenUI.dir/src/ui/Button.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/XenUI.dir/src/ui/Button.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sarowar/Documents/XenonUI/src/ui/Button.cpp > CMakeFiles/XenUI.dir/src/ui/Button.cpp.i

CMakeFiles/XenUI.dir/src/ui/Button.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/XenUI.dir/src/ui/Button.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sarowar/Documents/XenonUI/src/ui/Button.cpp -o CMakeFiles/XenUI.dir/src/ui/Button.cpp.s

# Object files for target XenUI
XenUI_OBJECTS = \
"CMakeFiles/XenUI.dir/src/graphics/sdl2/TextRenderer.cpp.o" \
"CMakeFiles/XenUI.dir/src/platform/linux/linux.cpp.o" \
"CMakeFiles/XenUI.dir/src/ui/Label.cpp.o" \
"CMakeFiles/XenUI.dir/src/ui/Button.cpp.o"

# External object files for target XenUI
XenUI_EXTERNAL_OBJECTS =

XenUI: CMakeFiles/XenUI.dir/src/graphics/sdl2/TextRenderer.cpp.o
XenUI: CMakeFiles/XenUI.dir/src/platform/linux/linux.cpp.o
XenUI: CMakeFiles/XenUI.dir/src/ui/Label.cpp.o
XenUI: CMakeFiles/XenUI.dir/src/ui/Button.cpp.o
XenUI: CMakeFiles/XenUI.dir/build.make
XenUI: /usr/lib/x86_64-linux-gnu/libGL.so
XenUI: /usr/lib/x86_64-linux-gnu/libGLU.so
XenUI: /usr/lib/x86_64-linux-gnu/libfreetype.so
XenUI: /usr/lib/x86_64-linux-gnu/libfreetype.so
XenUI: /usr/lib/x86_64-linux-gnu/libSDL2.so
XenUI: /usr/lib/x86_64-linux-gnu/libSDL2_ttf.so
XenUI: CMakeFiles/XenUI.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/sarowar/Documents/XenonUI/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable XenUI"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/XenUI.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/XenUI.dir/build: XenUI
.PHONY : CMakeFiles/XenUI.dir/build

CMakeFiles/XenUI.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/XenUI.dir/cmake_clean.cmake
.PHONY : CMakeFiles/XenUI.dir/clean

CMakeFiles/XenUI.dir/depend:
	cd /home/sarowar/Documents/XenonUI/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sarowar/Documents/XenonUI /home/sarowar/Documents/XenonUI /home/sarowar/Documents/XenonUI/build /home/sarowar/Documents/XenonUI/build /home/sarowar/Documents/XenonUI/build/CMakeFiles/XenUI.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/XenUI.dir/depend

