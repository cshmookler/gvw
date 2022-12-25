// Standard includes
#include <cstdint>
#include <iostream>

// External includes
// Include paths for packages installed by Conan.
// Not necessary for compilation, but provides declarations for intellisense.
// Clang-Tidy will incorrectly mark these as errors.
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

// Local includes
#include "../../vgw/vgw.hpp"
#include "utils.hpp"

int main(int argc, char** argv)
{
    int returnCode;

#ifdef _DEBUG
    std::cout << "Running in Debug mode\n" << std::endl;
    std::cout << "Command line arguments:\n";
    utils::PrintCommandLineArguments(argc, argv, '\t');
#else
    std::cout << "Running in Release mode\n" << std::endl;
#endif

    // Initialize GLFW
    returnCode = glfw::Init();
#ifdef _DEBUG
    std::cout << "glfw::Init() returned with: " << returnCode << std::endl;
#endif

    // Create the window
    glfw::Window window;
    window.Create(600, 450, "This is the window title");

    glfw::Monitor primaryMonitor = glfw::PrimaryMonitor();
    std::vector<glfw::Monitor> monitors = glfw::Monitors();
    std::cout << "monitor count: " << monitors.size() << std::endl;
    primaryMonitor.AssertInitialization();
    std::cout << "monitor name: " << primaryMonitor.name() << std::endl;

    // Window main loop
    while (window.ShouldClose() == GLFW_FALSE) {
        window.PollEvents();
    }

    // Destroy objects
    window.Destroy();
    glfw::Destroy();
}