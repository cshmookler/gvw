#pragma once

/**
 * @file monitor.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief The monitor class and other monitor related functions.
 * @date 2023-03-03
 */

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

/// @brief Accesses monitor information.
class monitor
{
    GLFWmonitor* monitorId_ = con::MONITOR_ID_NULL;
    GLFWvidmode videoMode_;

    // Checks if the monitor is initialized.
    // If it is not, an error is sent to the GLFW error callback.
    bool AssertInitialization_();

  public:
    /// @brief Creates a monitor object without assigning a monitor ID.
    monitor();

    /// @brief Assign a monitor ID on object creation.
    /// @param monitorId
    monitor(GLFWmonitor* monitorId);

    /// @brief Destroys the monitor object.
    ~monitor();

    /// @brief Assign a monitor ID if one wasn't already set during object
    /// construction. A monitor ID should not be assigned more than once. If the
    /// monitor ID was set during object construction, then this function should
    /// not be called.
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

    /// @brief Returns the content scale of the assigned monitor. The content
    /// scale is the ratio between the current DPI and the platform's default
    /// DPI. This can be used to scale text and UI elements so that they look
    /// appropriate on all devices.
    /// @return A coordinate object of type float.
    coordinate<float> ContentScale();

    /// @brief Returns the position of the assigned monitor on the virtual
    /// screen.
    /// @return A coordinate object of type int.
    coordinate<int> VirtualPosition();

    /// @brief Returns the origin of the work area in screen coordinates. The
    /// position of the work area may differ from the monitor virtual position
    /// if a toolbar is placed on top or on the left of the monitor.
    /// @return A coordinate object of type int.
    coordinate<int> WorkAreaPosition();

    /// @brief Returns the size of the work area in screen coordinates. The size
    /// of the work area may not be the same as the size of the monitor if there
    /// are toolbars present.
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

/// @brief Returns a `monitor` object cooresponding to the primary monitor. The
/// primary monitor, on most operating systems, contains the taskbar.
/// @return A `monitor` object.
monitor PrimaryMonitor();

/// @brief Returns a vector containing `monitor` objects for each physical
/// monitor.
/// @return A vector of `monitor` objects.
std::vector<monitor> Monitors();

} // namespace gvw