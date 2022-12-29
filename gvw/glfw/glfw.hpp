#pragma once

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace glfw {

// Global variables
extern const bool THROW_ON_GLFW_ERROR;
extern GLFWerrorfun ERROR_CALLBACK;

// Structure for storing version information
struct version
{
    int major;
    int minor;
    int revision;
};

template<typename type>
struct size;

template<typename type>
struct coordinate
{
    type x;
    type y;
    coordinate<type>& operator=(const coordinate<type>& coordinate);
    coordinate<type>& operator=(const size<type>& area);
};

template<typename type>
struct size
{
    type width;
    type height;
    size<type>& operator=(const size<type>& area);
    size<type>& operator=(const coordinate<type>& coordinate);
};

// Handle GLFW errors
void ErrorCallback(int errorCode, const char* description);

// Return version information
version RuntimeVersion();

// Initialize GLFW
int Init(GLFWerrorfun errorCallback = ErrorCallback);

// Terminate GLFW
void Destroy();

} // namespace glfw