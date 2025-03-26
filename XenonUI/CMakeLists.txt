cmake_minimum_required(VERSION 3.10)
project(XenUI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)

find_library(XFT_LIB Xft REQUIRED)




# Add FreeType include path
include_directories(/usr/include/freetype2)





# Include directories
include_directories(${X11_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} /usr/include/X11)

# Source files
file(GLOB_RECURSE SOURCES src/platform/linux/*.cpp src/ui/*.cpp src/core/*.cpp src/graphics/opengl/*.cpp)

# Create executable
add_executable(XenUI ${SOURCES})

# Link libraries
target_link_libraries(XenUI ${X11_LIBRARIES} ${OPENGL_LIBRARIES} ${XFT_LIB} GLX)
