# Project Setup
cmake_minimum_required(VERSION 3.20)

project(
  frameio
  VERSION 0.1.0
  DESCRIPTION "A small C++ application framework."
  LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

# Source Files
file(GLOB_RECURSE SOURCES
    ./src/Core/*.cpp
    ./src/ImGui/*.cpp
    ./src/Renderer/*.cpp
    ./src/Platforms/OpenGL/*.cpp)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  file(GLOB_RECURSE PLATFORM_SOURCES ./src/Platforms/Windows/*.cpp)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  file(GLOB_RECURSE PLATFORM_SOURCES ./src/Platforms/Linux/*.cpp)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  file(GLOB_RECURSE PLATFORM_SOURCES ./src/Platforms/MacOS/*.cpp)
endif()

# Targets
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${PLATFORM_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ON)

# Crossplatform Compiler Defines/Options
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  add_compile_definitions(FR_PLATFORM_WINDOWS)
  # target_compile_options(${PROJECT_NAME} PRIVATE /W4)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_compile_definitions(FR_PLATFORM_LINUX)
  # target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  add_compile_definitions(FR_PLATFORM_MACOS)
endif()

# Includes
target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ./src
  PUBLIC ./libs/debugbreak
  PRIVATE ./libs/glfw/include
  PRIVATE ./libs/spdlog/include
  PRIVATE ./libs/glad/include)

# External Libraries
add_subdirectory(./libs/glad)
add_subdirectory(./libs/glfw)
add_subdirectory(./libs/glm)
add_subdirectory(./libs/imgui)
add_subdirectory(./libs/imnodes)
add_subdirectory(./libs/spdlog)
add_subdirectory(./libs/stb)

# Linking
target_link_libraries(${PROJECT_NAME} glad glfw glm imgui imnodes spdlog stb_image)

# Precompiled Headers
target_precompile_headers(${PROJECT_NAME} PRIVATE ./src/frpch.hpp)
