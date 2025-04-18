cmake_minimum_required(VERSION 3.10)
project(XenUI VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(glm REQUIRED)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(Fontconfig REQUIRED)
    set(PLATFORM_SPECIFIC_LIBS Fontconfig::Fontconfig)
else()
    set(PLATFORM_SPECIFIC_LIBS "")
endif()

set(XENUI_PUBLIC_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(XENUI_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")

file(GLOB XENUI_PUBLIC_HEADERS
    "${XENUI_PUBLIC_INCLUDE_DIR}/XenUI/*.h"
)

add_library(XenUI SHARED
    ${XENUI_SOURCE_DIR}/graphics/sdl2/TextRenderer.cpp
    ${XENUI_SOURCE_DIR}/ui/Label.cpp
    ${XENUI_SOURCE_DIR}/utils/Anchor.cpp
    ${XENUI_SOURCE_DIR}/utils/WindowUtil.cpp
    ${XENUI_SOURCE_DIR}/ui/Shape.cpp
    ${XENUI_SOURCE_DIR}/ui/Button.cpp
    ${XENUI_SOURCE_DIR}/ui/InputBox.cpp
)

set_target_properties(XenUI PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "${XENUI_PUBLIC_HEADERS}"
)

target_include_directories(XenUI
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${XENUI_PUBLIC_INCLUDE_DIR}>
    PRIVATE
        ${XENUI_SOURCE_DIR}
        ${SDL2_INCLUDE_DIRS}
        ${FREETYPE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIR}
)

set(FALLBACK_FONT_SOURCE "${CMAKE_SOURCE_DIR}/third_party/dejavu-fonts-ttf-2.37/ttf/DejaVuSans.ttf")
include(GNUInstallDirs)
set(XENUI_FONT_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/fonts/XenUI")
set(FALLBACK_FONT_INSTALLED_PATH "${XENUI_FONT_INSTALL_DIR}/DejaVuSans.ttf")

target_compile_definitions(XenUI PRIVATE
    "XENUI_FALLBACK_FONT_PATH=\"${FALLBACK_FONT_INSTALLED_PATH}\""
)

target_link_libraries(XenUI PRIVATE
    OpenGL::GL
    Freetype::Freetype
    glm::glm
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
    ${PLATFORM_SPECIFIC_LIBS}
)

install(TARGETS XenUI
    EXPORT XenUITargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/XenUI
)

install(DIRECTORY ${XENUI_PUBLIC_INCLUDE_DIR}/XenUI/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/XenUI
)

install(FILES "${FALLBACK_FONT_SOURCE}"
    DESTINATION "${XENUI_FONT_INSTALL_DIR}"
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/XenUIConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/XenUIConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/XenUIConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/XenUI
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR XENUI_FONT_INSTALL_DIR
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/XenUIConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/XenUIConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/XenUI
)

install(EXPORT XenUITargets
    FILE XenUITargets.cmake
    NAMESPACE XenUI::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/XenUI
)

message(STATUS "Configured XenUI Framework Library ${PROJECT_VERSION}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Install Library Dir: ${CMAKE_INSTALL_LIBDIR}")
message(STATUS "  Install Include Dir: ${CMAKE_INSTALL_INCLUDEDIR}")
message(STATUS "  Install Font Dir: ${XENUI_FONT_INSTALL_DIR}")





# cmake_minimum_required(VERSION 3.10)
# project(XenUI)

# # Use C++20
# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Dependencies
# find_package(OpenGL REQUIRED)
# find_package(Freetype REQUIRED)
# find_package(SDL2 REQUIRED)
# find_package(SDL2_ttf REQUIRED)
# find_package(glm REQUIRED)
# find_package(Fontconfig REQUIRED)


# # Include dirs (only your own include folder needed here)
# include_directories(${PROJECT_SOURCE_DIR}/include)

# # Source groups
# set(GRAPHICS_SOURCES
#     src/graphics/sdl2/TextRenderer.cpp
# )
# set(PLATFORM_SOURCES
#     src/platform/linux/linux.cpp
# )
# set(UI_SOURCES
#     src/ui/Label.cpp
#     src/ui/Button.cpp
#     src/ui/Shape.cpp
# )
# set(UTILS_SOURCES
#     src/utils/Anchor.cpp
#     src/utils/WindowUtil.cpp
# )

# # All sources
# set(SOURCES
#     ${GRAPHICS_SOURCES}
#     ${PLATFORM_SOURCES}
#     ${UI_SOURCES}
#     ${UTILS_SOURCES}
# )

# # Executable
# add_executable(XenUI ${SOURCES})

# # Define if needed
# target_compile_definitions(XenUI PRIVATE 
#     FREETYPE_GL_USE_VAO
# )

# # Link libraries
# target_link_libraries(XenUI PRIVATE
#     OpenGL::GL
#     Freetype::Freetype
#     SDL2::SDL2
#     SDL2_ttf::SDL2_ttf
#     glm::glm
#     Fontconfig::Fontconfig
#     dl
#     pthread
# )

# # Copy font after build
# set(SOURCE_FONT_PATH "${CMAKE_SOURCE_DIR}/third_party/dejavu-fonts-ttf-2.37/ttf/DejaVuSans.ttf")
# set(DESTINATION_FONT_DIR "${CMAKE_BINARY_DIR}/assets/fonts")
# file(MAKE_DIRECTORY "${DESTINATION_FONT_DIR}")
# add_custom_command(
#     TARGET XenUI POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different 
#             "${SOURCE_FONT_PATH}" 
#             "${DESTINATION_FONT_DIR}/DejaVuSans.ttf"
#     COMMENT "Copying DejaVuSans.ttf to assets/fonts"
#     VERBATIM
# )

# # Install
# install(TARGETS XenUI DESTINATION bin)

# # Summary
# message(STATUS "Build configuration:")
# message(STATUS "  - OpenGL: ${OPENGL_LIBRARIES}")
# message(STATUS "  - Freetype: ${FREETYPE_INCLUDE_DIRS}")
