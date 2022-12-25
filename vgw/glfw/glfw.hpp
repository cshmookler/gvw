#pragma once

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace glfw {

// Structure for storing version information
struct Version
{
    int major;
    int minor;
    int revision;
};

// Handle GLFW errors
void ErrorCallback(int errorCode, const char* description);

// Return version information
Version RuntimeVersion();

// Initialize GLFW (must only be called from the main thread)
int Init();

// Free resources (calls the destructor)
void Destroy();

} // namespace glfw