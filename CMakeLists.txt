cmake_minimum_required(VERSION 3.10)
project(Chip8Emulator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/libs/SDL2/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/SDL2_mixer/include)

link_directories(${PROJECT_SOURCE_DIR}/libs/SDL2/lib)
link_directories(${PROJECT_SOURCE_DIR}/libs/SDL2_mixer/lib)

add_executable(Chip8Emulator
    src/main.cpp
    src/cpu/Chip8.cpp
    src/graphics/Renderer.cpp
    src/input/InputHandler.cpp
)

target_link_libraries(Chip8Emulator SDL2 SDL2_mixer)