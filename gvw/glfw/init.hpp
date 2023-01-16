#pragma once

// Standard includes
#include <vector>

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Local includes
#include "../common/types.hpp"

namespace gvw {

// Forward declaration of the 'window' class
class window;

// Handle GLFW errors
void ErrorCallback(int errorCode, const char* description);

// Return version information
version RuntimeVersion();

// Initialize GLFW
int Init(GLFWerrorfun errorCallback = ErrorCallback);

// Terminate GLFW
void Destroy();

GLFWvkproc GetInstanceProcessAddress(VkInstance instance, const char* procname);

std::vector<const char*> GetRequiredInstanceExtensions();

bool PhysicalDeviceHasPresentationSupport(VkInstance instance,
                                          VkPhysicalDevice physicalDevice,
                                          uint32_t queueFamily);

void SetSwapInterval(int interval);

} // namespace gvw