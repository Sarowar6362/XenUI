cmake_minimum_required(VERSION 3.10)
project(XenUI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find all required packages
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(glm REQUIRED)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${X11_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIR}
)

# Collect source files (explicit is better than GLOB for production)
set(SOURCES
    src/core/App.cpp
    src/graphics/opengl/TextRenderer.cpp
    src/platform/linux/linux.cpp
    src/ui/Label.cpp
    # Add other source files as needed
)

# Create executable
add_executable(XenUI ${SOURCES})

# Configure modern OpenGL and link libraries
target_compile_definitions(XenUI PRIVATE 
    GLEW_STATIC
    FREETYPE_GL_USE_VAO
)

target_link_libraries(XenUI PRIVATE
    ${X11_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    glm::glm
    Freetype::Freetype
    GLEW::GLEW
    GLUT::GLUT
    dl
    pthread
)

# Handle different platforms and link options
if(UNIX AND NOT APPLE)
    target_link_libraries(XenUI PRIVATE
        Xext
        Xft
        Xrender
    )
endif()

# Install target (optional)
install(TARGETS XenUI DESTINATION bin)

# Print configuration summary
message(STATUS "Build configuration:")
message(STATUS "  - X11 Includes: ${X11_INCLUDE_DIR}")
message(STATUS "  - Freetype Includes: ${FREETYPE_INCLUDE_DIRS}")
message(STATUS "  - OpenGL Libraries: ${OPENGL_LIBRARIES}")
message(STATUS "  - GLEW Version: ${GLEW_VERSION}")