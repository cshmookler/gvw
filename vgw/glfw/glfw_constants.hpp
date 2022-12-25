#pragma once

// Standard includes
#include <cstdint>

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace glfw {

// General constants
const bool THROW_ON_GLFW_ERROR = false;

// Window specific constants
GLFWwindow* const WINDOW_ID_DEFAULT = GLFW_FALSE;
const int MIN_WINDOW_WIDTH = 1;
const int MIN_WINDOW_HEIGHT = 1;

// Monitor specific constants
GLFWmonitor* const MONITOR_ID_DEFAULT = GLFW_FALSE;
const float GAMMA_DEFAULT = 1.0;

// Window error codes
const int ERROR_WINDOW_ALREADY_CREATED = 0x00020001;
const int ERROR_INVALID_DIMENSIONS = 0x00020002;
const int ERROR_PARENT_WINDOW_NOT_CREATED = 0x00020003;

// Window error messages
const char* const ERROR_MESSAGE_WINDOW_ALREADY_CREATED =
    "The same window cannot be created twice";
const char* const ERROR_MESSAGE_INVALID_DIMENSIONS =
    "Invalid window dimensions";
const char* const ERROR_MESSAGE_PARENT_WINDOW_NOT_CREATED =
    "Parent window not created before child";

// Monitor error codes
const int ERROR_MONITOR_ID_NOT_INITIALIZED = 0x00030001;

// Monitor error messages
const char* const ERROR_MESSAGE_MONITOR_ID_NOT_INITIALIZED =
    "A monitor ID for a monitor object was required but not set";

} // namespace glfw