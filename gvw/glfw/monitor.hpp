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

/// @brief For retrieving monitor information.
class monitor
{
    GLFWmonitor* monitorId_ = con::MONITOR_ID_NULL;
    GLFWvidmode videoMode_;

    // Checks if the monitor is initialized.
    // If it is not, an error is sent to the GLFW error callback.
    bool AssertInitialization_();

  public:
    // Constructors
    monitor();

    /// @brief Assign a monitor ID on object creation.
    /// @param monitorId
    monitor(GLFWmonitor* monitorId);

    // Destructor
    ~monitor();

    /// @brief Assign a monitor ID if one wasn't already set during object
    /// construction.
    ///
    /// A monitor ID should not be assigned more than once. If the monitor ID
    /// was set during object construction, then this function should not be
    /// called.
    ///
    /// @param monitorId
    inline void Init(GLFWmonitor* monitorId);

    /// @brief Returns the ID of the assigned monitor.
    /// @return A pointer to a GLFWmonitor object.
    GLFWmonitor* Id();

    /// @brief Returns the video mode of the assigned monitor in screen
    /// coordinates.
    /// @return A pointer to a constant GLFWvidmode object.
    const GLFWvidmode* VideoMode();

    /// @brief Returns the physical size of the monitor in millimeters.
    /// @return A size object of type int.
    size<int> PhysicalSize();

    /// @brief Returns the content scale of the assigned monitor.
    ///
    /// The content scale is the ratio between the current DPI and the
    /// platform's default DPI. This can be used to scale text and UI elements
    /// so that they look appropriate on all devices.
    ///
    /// @return A coordinate object of type float.
    coordinate<float> ContentScale();

    /// @brief Returns the position of the assigned monitor on the virtual
    /// screen.
    /// @return A coordinate object of type int.
    coordinate<int> VirtualPosition();

    /// @brief Returns the origin of the work area in screen coordinates.
    ///
    /// The position of the work area may differ from the monitor virtual
    /// position if a toolbar is placed on top or on the left of the monitor.
    ///
    /// @return A coordinate object of type int.
    coordinate<int> WorkAreaPosition();

    /// @brief Returns the size of the work area in screen coordinates.
    ///
    /// The size of the work area may not be the same as the size of the monitor
    /// if there are toolbars present.
    ///
    /// @return A size object of type int.
    size<int> WorkAreaSize();

    /// @brief Returns the name of the assigned monitor.
    /// @return A C-style string (const char*).
    const char* Name();

    /// @brief Returns the gamma ramp of the assigned monitor.
    /// @return A pointer to a constant GLFWgammaramp object.
    const GLFWgammaramp* GammaRamp();

    /// @brief Sets the gamma ramp of the assigned monitor.
    /// @param gammaRamp
    void SetGammaRamp(const GLFWgammaramp& gammaRamp);

    /// @brief Resets the gamma ramp of the assigned monitor.
    void ResetGammaRamp();
};

// Returns a 'Monitor' object for the primary monitor
monitor PrimaryMonitor();

// Returns a vector containing a 'Monitor' object for each physical monitor
std::vector<monitor> Monitors();

} // namespace gvw