#include "glfw.hpp"

// Standard includes
#include <iostream>
#include <stdexcept>

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Local includes
#include "constants.hpp"

namespace vgw {

Glfw::Glfw() {}

Glfw::~Glfw()
{
    glfwTerminate();
}

int Glfw::Init(void (*errorCallback)(int, const char*))
{
    // Set the GLFW error callback
    glfwSetErrorCallback(errorCallback);

    // Initialize the GLFW library
    return glfwInit();
}

void Glfw::GlfwErrorCallback_(int errorCode, const char* description)
{
    if (errorCode != GLFW_NO_ERROR) {
        if (THROW_ON_GLFW_ERROR == true) {
            throw std::runtime_error(description);
        } else {
            std::cout << "GLFW runtime error: " << description << std::endl;
        }
    }
}

} // namespace vgw