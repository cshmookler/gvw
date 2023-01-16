#include "init.tpp"

// Standard includes
#include <iostream>
#include <stdexcept>

// External includes
#include <boost/dll.hpp>

// Local includes
#include "../common/const.hpp"
#include "../common/global.hpp"

namespace gvw {

void ErrorCallback(int errorCode, const char* description)
{
    if (errorCode != GLFW_NO_ERROR) {
        if (THROW_ON_GLFW_ERROR) {
            throw std::runtime_error(description);
        }

        std::cout << "GLFW runtime error: " << description << std::endl;
    }
}

version RuntimeVersion()
{
    version version;
    glfwGetVersion(&version.major, &version.minor, &version.revision);
    return version;
}

int Init(GLFWerrorfun errorCallback)
{
    // Set the GLFW error callback
    glfwSetErrorCallback(errorCallback);
    ERROR_CALLBACK = errorCallback;

    // Set the working directory to the executable directory
    boost::filesystem::path absolutePathToBinary =
        boost::dll::program_location();
    std::cout << absolutePathToBinary << std::endl;
    absolutePathToBinary.remove_filename();
    boost::filesystem::current_path(absolutePathToBinary);

#ifdef _DEBUG
    // Print compiletime version and other information
    std::cout << "GLFW " << glfwGetVersionString() << " ";

    // Print runtime version
    version runtimeVersion = RuntimeVersion();
    std::cout << "(runtime version: " << runtimeVersion.major << "."
              << runtimeVersion.minor << "." << runtimeVersion.revision << ")"
              << std::endl;
#endif

    // Shared init hints
    glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_TRUE);

#ifdef __APPLE__
    // MacOS specific init hints
    glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_TRUE);
    glfwInitHint(GLFW_COCOA_MENUBAR, GLFW_TRUE);
#endif

    // Initialize the GLFW library
    int initialized = glfwInit();

    // Check Vulkan support
    if (glfwVulkanSupported() != GLFW_TRUE) {
        ERROR_CALLBACK(ERROR_VULKAN_LOADER_NOT_FOUND,
                       ERROR_MESSAGE_VULKAN_LOADER_NOT_FOUND);
    }

    return initialized;
}

void Destroy()
{
    glfwTerminate();
}

GLFWvkproc GetInstanceProcessAddress(VkInstance vulkanInstance,
                                     const char* processName)
{
    return glfwGetInstanceProcAddress(vulkanInstance, processName);
}

std::vector<const char*> GetRequiredInstanceExtensions()
{
    uint32_t extensionCount;
    const char** extensions =
        glfwGetRequiredInstanceExtensions(&extensionCount);
    std::vector<const char*> requiredInstanceExtensions(extensionCount);
    for (size_t extensionIndex = 0; extensionIndex < extensionCount;
         extensionIndex++) {
        requiredInstanceExtensions[extensionIndex] = extensions[extensionCount];
    }
    return requiredInstanceExtensions;
}

bool PhysicalDeviceHasPresentationSupport(VkInstance instance,
                                          VkPhysicalDevice physicalDevice,
                                          uint32_t queueFamily)
{
    return bool(glfwGetPhysicalDevicePresentationSupport(
        instance, physicalDevice, queueFamily));
}

} // namespace gvw