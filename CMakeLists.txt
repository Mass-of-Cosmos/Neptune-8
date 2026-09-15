cmake_minimum_required(VERSION 4.3)
project(Neptune_8)

set(CMAKE_CXX_STANDARD 23)

add_executable(Neptune_8
        src/Core/emulator.cpp
        src/Core/emulator.h
        "src/Core/chip8.h"
        src/main.cpp
        src/Core/chip8.cpp
        src/Frontend/SDLWindow.cpp
        src/Frontend/SDLWindow.h
)

include(FetchContent)

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        src/Frontend
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.8
)

FetchContent_MakeAvailable(src/Frontend)

target_link_libraries(Neptune_8 PRIVATE SDL3::SDL3-static)