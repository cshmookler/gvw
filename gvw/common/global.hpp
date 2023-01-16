#pragma once

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace gvw {

extern const bool THROW_ON_GLFW_ERROR;
extern GLFWerrorfun ERROR_CALLBACK;

} // namespace gvw