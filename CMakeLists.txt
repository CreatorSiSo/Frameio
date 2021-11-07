# Project Setup
cmake_minimum_required(VERSION 3.20)

project(
  framio
  VERSION 0.1.0
  DESCRIPTION "A small C++ application framework."
  LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

# Source Files
set(SOURCES
    ./src/App.cpp
    ./src/Layer.cpp
    ./src/LayerStack.cpp
    ./src/Log.cpp
    ./src/ImGui/ImGuiBuild.cpp
    ./src/ImGui/ImGuiLayer.cpp
    ./src/Platforms/OpenGL/OpenGLContext.cpp
    ./src/Renderer/Shader.cpp)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(PLATFORM_SOURCES ./src/Platforms/Windows/WindowsInput.cpp
                       ./src/Platforms/Windows/WindowsWindow.cpp)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(PLATFORM_SOURCES ./src/Platforms/Linux/LinuxInput.cpp
                       ./src/Platforms/Linux/LinuxWindow.cpp)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(PLATFORM_SOURCES ./src/Platforms/MacOS/MacOSInput.cpp
                       ./src/Platforms/MacOS/MacOSWindow.cpp)
endif()

# Targets
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${PLATFORM_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ON)

# Crossplatform Compiler Defines/Options
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  add_compile_definitions(TX_PLATFORM_WINDOWS)
  # target_compile_options(${PROJECT_NAME} PRIVATE /W4)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_compile_definitions(TX_PLATFORM_LINUX)
  # target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  add_compile_definitions(TX_PLATFORM_MACOS)
endif()

# Includes
target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ./src
  PUBLIC ./libs/debugbreak
  PUBLIC ./libs/glfw/include
  PUBLIC ./libs/glm
  PUBLIC ./libs/imgui
  PUBLIC ./libs/spdlog/include
  PRIVATE ./libs/glad/include)

# External Libraries
add_subdirectory(./libs/glad)
add_subdirectory(./libs/glfw)
add_subdirectory(./libs/glm)
add_subdirectory(./libs/imgui)
add_subdirectory(./libs/spdlog)

# Linking
target_link_libraries(${PROJECT_NAME} glad glfw glm imgui spdlog)

# Precompiled Headers
target_precompile_headers(${PROJECT_NAME} PRIVATE ./src/frpch.hpp)
