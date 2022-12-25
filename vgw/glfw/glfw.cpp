#include "glfw.hpp"

// Standard includes
#include <iostream>
#include <stdexcept>

// Local includes
#include "glfw_constants.hpp"

namespace glfw {

void ErrorCallback(int errorCode, const char* description)
{
    if (errorCode != GLFW_NO_ERROR) {
        if (THROW_ON_GLFW_ERROR == true) {
            throw std::runtime_error(description);
        } else {
            std::cout << "GLFW runtime error: " << description << std::endl;
        }
    }
}

Version RuntimeVersion()
{
    Version version;
    glfwGetVersion(&version.major, &version.minor, &version.revision);
    return version;
}

int Init()
{
    // Set the GLFW error callback
    glfwSetErrorCallback(ErrorCallback);

#ifdef _DEBUG
    // Print compiletime version and other information
    std::cout << "GLFW " << glfwGetVersionString() << " ";

    // Print runtime version
    Version runtimeVersion = RuntimeVersion();
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
    return glfwInit();
}

void Destroy()
{
    glfwTerminate();
}

} // namespace glfw