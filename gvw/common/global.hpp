#pragma once

/**
 * @file global.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief Global variables
 * @date 2023-02-02
 */

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace gvw {

/// @brief Used by the default error callback for GLFW. If set to GLFW_TRUE, an
/// error is thrown when GLFW encounters an error. If set to GLFW_FALSE, a
/// warning is printed to the console when GLFW encounters an error.
extern bool THROW_ON_GLFW_ERROR;

/// @brief The GLFW error callback. Set by `gvw::Init`.
extern GLFWerrorfun ERROR_CALLBACK_GLFW;

} // namespace gvw