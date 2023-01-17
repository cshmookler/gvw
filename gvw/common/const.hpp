#pragma once

// Standard includes
#include <cstdint>

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace gvw {

// General constants
const bool ASSERT_SUCCESS = GLFW_FALSE;
const bool ASSERT_FAILURE = GLFW_TRUE;
const size_t INPUT_BUFFER_INITIAL_SIZE = 20;
const size_t INPUT_BUFFER_INCREMENT_SIZE = 50;

// General error codes
const int ERROR_VULKAN_LOADER_NOT_FOUND = 0x00011001;
// General error messages
const char* const ERROR_MESSAGE_VULKAN_LOADER_NOT_FOUND =
    "A Vulkan loader and/or a Vulkan compatible display driver were not found";

// Window specific constants
GLFWwindow* const WINDOW_ID_NULL = nullptr;
const int MIN_WINDOW_WIDTH = 1;
const int MIN_WINDOW_HEIGHT = 1;
// Window display modes
const int NOT_CREATED = 0;
const int WINDOWED = 1;
const int ICONIFIED = 2;
const int MAXIMIZED = 3;
const int FULL_SCREEN = 4;

// Window error codes
const int ERROR_WINDOW_ALREADY_CREATED = 0x00012001;
const int ERROR_INVALID_DIMENSIONS = 0x00012002;
const int ERROR_WINDOW_NOT_CREATED_BEFORE_OPERATION = 0x00012003;
const int ERROR_PARENT_WINDOW_NOT_CREATED = 0x00012004;
const int ERROR_ICON_FAILED_TO_LOAD = 0x00012005;
const int ERROR_NOT_ENOUGH_COMPONENTS_PER_PIXEL = 0x00012006;
const int ERROR_WINDOW_SURFACE_ALREADY_CREATED = 0x00012007;
// Window error messages
const char* const ERROR_MESSAGE_WINDOW_ALREADY_CREATED =
    "The same window cannot be created twice";
const char* const ERROR_MESSAGE_INVALID_DIMENSIONS =
    "Invalid window dimensions";
const char* const ERROR_MESSAGE_WINDOW_NOT_CREATED_BEFORE_OPERATION =
    "The operation could not be performed because the window has not been "
    "created";
const char* const ERROR_MESSAGE_PARENT_WINDOW_NOT_CREATED =
    "A parent window must be created before its child window";
const char* const ERROR_MESSAGE_ICON_FAILED_TO_LOAD =
    "The window icon failed to load";
const char* const ERROR_MESSAGE_NOT_ENOUGH_COMPONENTS_PER_PIXEL =
    "The selected image does not have enough color components per pixel to be "
    "properly displayed";
const char* const ERROR_MESSAGE_WINDOW_SURFACE_ALREADY_CREATED =
    "The window surface has already been created";

// Monitor specific constants
GLFWmonitor* const MONITOR_ID_NULL = nullptr;
const float GAMMA_DEFAULT = 1.0;

// Monitor error codes
const int ERROR_MONITOR_ID_NOT_INITIALIZED = 0x00013001;
// Monitor error messages
const char* const ERROR_MESSAGE_MONITOR_ID_NOT_INITIALIZED =
    "A monitor object must first be initialized before accessing or setting "
    "its members";

} // namespace gvw