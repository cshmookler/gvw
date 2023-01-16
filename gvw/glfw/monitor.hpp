#pragma once

// Standard includes
#include <optional>
#include <vector>

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Local includes
#include "../common/const.hpp"
#include "init.hpp"

namespace gvw {

class monitor
{
    GLFWmonitor* monitorId_ = MONITOR_ID_NULL;
    GLFWvidmode videoMode_;

  public:
    // Constructors
    monitor();
    monitor(GLFWmonitor* monitorId);

    // Destructor
    ~monitor();

    inline void Init(GLFWmonitor* monitorId);

    // Checks if the monitor is initialized.
    // If it is not, an error is sent to the GLFW error callback.
    bool AssertInitialization();

    GLFWmonitor* Id();
    const GLFWvidmode* VideoModeInScreenCoordinates();
    GLFWvidmode* VideoMode(window& associatedWindow);
    size<int> PhysicalSize();
    coordinate<float> ContentScale();
    coordinate<int> VirtualPosition(window& associatedWindow);
    coordinate<int> WorkAreaPosition(window& associatedWindow);
    size<int> WorkAreaSize(window& associatedWindow);
    const char* Name();
    const GLFWgammaramp* GammaRamp();

    // Set gamma ramp
    void SetGammaRamp(const GLFWgammaramp& gammaRamp);

    // Reset gamma ramp with a default value
    void ResetGammaRamp();
};

// Returns a 'Monitor' object for the primary monitor
monitor PrimaryMonitor();

// Returns a vector containing a 'Monitor' object for each physical monitor
std::vector<monitor> Monitors();

} // namespace gvw