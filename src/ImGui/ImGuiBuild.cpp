#include "frpch.hpp"

#include <glad/glad.h>

// We use glad as a opengl loader so we dont need imguis custom one
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM

#include "backends/imgui_impl_glfw.cpp"
#include "backends/imgui_impl_opengl3.cpp"