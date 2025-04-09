cmake_minimum_required(VERSION 3.10)
project(XenUI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find all required packages
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(glm REQUIRED)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
)

# Collect source files (explicit is better than GLOB for production)
set(SOURCES
    
    src/graphics/sdl2/TextRenderer.cpp  # Assuming SDL2-based TextRenderer implementation
    src/platform/linux/linux.cpp
    src/ui/Label.cpp
    src/utils/Anchor.cpp
    src/utils/WindowUtil.cpp
    src/ui/Shape.cpp
    src/ui/Button.cpp
    # Add other source files as needed
)

# Create executable
add_executable(XenUI ${SOURCES})

# Configure modern OpenGL and link libraries
target_compile_definitions(XenUI PRIVATE 
    FREETYPE_GL_USE_VAO
)

target_link_libraries(XenUI PRIVATE
    ${OPENGL_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    glm::glm
    Freetype::Freetype
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
    dl
    pthread
)

# Specify the source font file location
set(SOURCE_FONT_PATH "${CMAKE_SOURCE_DIR}/third_party/dejavu-fonts-ttf-2.37/ttf/DejaVuSans.ttf")

# Specify the destination directory relative to the executable in the build folder
set(DESTINATION_FONT_DIR "${CMAKE_BINARY_DIR}/assets/fonts") # CMAKE_BINARY_DIR is your build dir

# Ensure the destination directory exists before trying to copy
file(MAKE_DIRECTORY "${DESTINATION_FONT_DIR}")

# Add a command to copy the font after the executable is built
add_custom_command(
    TARGET XenUI POST_BUILD # Execute after building the XenUI target
    COMMAND ${CMAKE_COMMAND} -E copy_if_different # Use CMake's copy tool
            "${SOURCE_FONT_PATH}"                   # Source file
            "${DESTINATION_FONT_DIR}/DejaVuSans.ttf" # Destination file (with filename)
    COMMENT "Copying DejaVuSans.ttf to assets/fonts"
    VERBATIM
)

# Install target (optional)
install(TARGETS XenUI DESTINATION bin)

# Print configuration summary
message(STATUS "Build configuration:")
message(STATUS "  - OpenGL Libraries: ${OPENGL_LIBRARIES}")
message(STATUS "  - Freetype Includes: ${FREETYPE_INCLUDE_DIRS}")
message(STATUS "  - GLEW Version: ${GLEW_VERSION}")
