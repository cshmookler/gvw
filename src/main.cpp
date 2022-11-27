// Standard includes
#include <cstdint>
#include <iostream>

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

// Include paths for packages installed by Conan.
// Not necessary for compilation, but provides declarations for intellisense.
// Clang-Tidy will incorrectly mark these as errors.
#include <boost/static_string.hpp>

// Local includes
#include "utils.hpp"
#include "vgw/vgw.hpp"

int main(int argc, char** argv)
{
    utils::PrintCommandLineArguments(argc, argv);

#ifdef _DEBUG
    std::cout << "Running in Debug mode" << std::endl;
#else
    std::cout << "Running in Release mode" << std::endl;
#endif

    // Initialize GLFW
    vgw::Glfw glfw;
    std::cout << "glfw.Init() returned with: " << glfw.Init() << std::endl;

    // Create the window
    vgw::Window window;

    return 0;
}