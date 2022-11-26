// Standard includes
#include <cstdint>
#include <iostream>

// Local includes
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

#include "utils.hpp"

int main(int argc, char** argv)
{
    utils::print_command_line_arguments(argc, argv);

    boost::basic_string_view test = "example";

#ifdef _DEBUG
    std::cout << "Running in Debug mode" << std::endl;
#else
    std::cout << "Running in Release mode" << std::endl;
#endif

    return 0;
}