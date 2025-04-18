cmake_minimum_required(VERSION 3.10)
project(TestXenUI)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(
    /usr/local/include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

link_directories(/usr/local/lib)

add_executable(xenui_test linux.cpp)

target_link_libraries(xenui_test
    XenUI
    SDL2
    SDL2_ttf
    GL
)
