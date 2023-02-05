#pragma once

/**
 * @file init.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief Functions for initializing GLFW and Vulkan.
 * @date 2023-02-03
 */

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

/// @brief Default error callback for GLFW and Vulkan.
/// @param errorCode
/// @param description
void ErrorCallback(int errorCode, const char* description);

// Return version information

/// @brief Returns GLFW version information. Can be called before `gvw::Init`.
/// @return `gvw::version` object.
version RuntimeVersion();

// Initialize GLFW

/// @brief Initializes GLFW. Must be called once at the beginning of your
/// program before most other GLFW functions.
///
/// Sets the glfw and vulkan error callback. Changes the working directory to
/// the parent directory of the executable (so all paths are relative to it). If
/// running in debug mode, the GLFW compile-time and runtime version are printed
/// to the console. GLFW initialization hints are set and the `glfwInit`
/// function is called.
///
/// @param errorCallback
/// @return GLFW_TRUE if initialization succeeded.<br> GLFW_FALSE if
/// initialization failed.
int Init(GLFWerrorfun errorCallback = ErrorCallback);

/// @brief Uninitializes GLFW.
void Destroy();

/// @brief Returns the process name associated with a Vulkan instance.
/// @param instance
/// @param procname
/// @return A `GLFWvkproc` object.
GLFWvkproc GetInstanceProcessAddress(VkInstance instance, const char* procname);

/// @brief Returns the required vulkan instance extensions.
/// @return A vector of `const char*`.
std::vector<const char*> GetRequiredInstanceExtensions();

/// @brief Returns the presentation support of a physical device.
/// @param instance
/// @param physicalDevice
/// @param queueFamily
/// @return True if the physical device has presentation support.<br> False if
/// the physical device does not have presentation support.
bool PhysicalDeviceHasPresentationSupport(VkInstance instance,
                                          VkPhysicalDevice physicalDevice,
                                          uint32_t queueFamily);

} // namespace gvw