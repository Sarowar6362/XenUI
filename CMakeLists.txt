cmake_minimum_required(VERSION 3.10)
project(XenUI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use pkg-config to find Xft and Freetype
find_package(PkgConfig REQUIRED)
pkg_check_modules(XFT REQUIRED xft)          # Finds Xft headers/libs
pkg_check_modules(FREETYPE REQUIRED freetype2) # Finds Freetype headers/libs

# Find X11 and OpenGL
find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)

# Include directories
include_directories(
    ${X11_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
    ${XFT_INCLUDE_DIRS}
    /usr/include/X11  # Explicitly add X11 headers
)


# Source files
file(GLOB_RECURSE SOURCES
    src/platform/linux/*.cpp
    src/ui/*.cpp
    src/core/*.cpp
    src/graphics/opengl/*.cpp
)

# Create executable
add_executable(XenUI ${SOURCES})

# Link libraries
target_link_libraries(XenUI
    ${X11_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${FREETYPE_LIBRARIES}  # Freetype library
    ${XFT_LIBRARIES}       # Xft library
)