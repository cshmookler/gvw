#include "glfw.ipp"

// Standard includes
#include <iostream>
#include <stdexcept>

// External includes
#include <boost/dll.hpp>

// Local includes
#include "glfw_constants.hpp"

namespace glfw {

const bool THROW_ON_GLFW_ERROR = GLFW_FALSE;
GLFWerrorfun ERROR_CALLBACK = nullptr;

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
    return glfwInit();
}

void Destroy()
{
    glfwTerminate();
}

} // namespace glfw