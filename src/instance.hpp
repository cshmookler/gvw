#pragma once

/**
 * @file instance.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief GVW instance singleton.
 * @date 2023-08-27
 */

// Local includes
#include "gvw.hpp"

namespace gvw {

class instance : internal::uncopyable_unmovable // NOLINT
{
    friend internal::instance_public_constructor;

    friend class monitor;
    friend class window;
    friend class device;

    ////////////////////////////////////////////////////////////////////////////
    ///                Constructors, Operators, and Destructor               ///
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Initializes GVW.
    /// @remark This constructor is made private to prevent it from being called
    /// from outside of this class.
    instance(
        const instance_info& Instance_Info = instance_info_config::DEFAULT);

  public:
    // The destructor is public to allow explicit destruction.
    ~instance() = default;

    ////////////////////////////////////////////////////////////////////////////
    ///                        Public Static Functions                       ///
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Deinitializes the GLFW library.
    static void TerminateGlfw();

  private:
    ////////////////////////////////////////////////////////////////////////////
    ///                           Private Variables                          ///
    ////////////////////////////////////////////////////////////////////////////
    /// @todo Determine if these Vulkan related variables need to be
    /// thread-safe.

    std::unique_ptr<internal::terminator<>> glfwTerminator;

    instance_extensions vulkanInstanceExtensions;
    instance_layers vulkanInstanceLayers;

    vk::UniqueInstance vulkanInstance;
    vk::DispatchLoaderDynamic vulkanDispatchLoaderDynamic;
    vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic>
        vulkanDebugUtilsMessenger;

  public:
    ////////////////////////////////////////////////////////////////////////////
    ///                        Public Member Functions                       ///
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Sets the joystick event callback.
    void SetJoystickEventCallback(
        instance_joystick_event_callback Joystick_Event_Callback =
            instance_joystick_event_callback_config::NONE);

    /// @brief Returns the joystick event buffer.
    const std::vector<instance_joystick_event>& GetJoystickEvents();

    /// @brief Clears the joystick event buffer.
    void ClearJoystickEvents();

    /// @brief Polls events for all windows.
    void PollEvents();

    /// @brief Waits until a window event is received, then polls events for all
    /// windows.
    void WaitThenPollEvents();

    /// @brief Waits until either the timeout (measured in milliseconds) expires
    /// or a window event is received, then polls events for all windows.
    void WaitThenPollEvents(double Timeout);

    /// @brief Posts an empty event. Causes `WaitThenPollEvents` to poll events.
    void PostEmptyEvent();

    /// @brief Creates a window.
    [[nodiscard]] window_ptr CreateWindow(
        const window_info& Window_Info = window_info_config::DEFAULT);

    /// @brief Selects physical devices for graphics processing.
    [[nodiscard]] std::vector<gvw::device_ptr> SelectPhysicalDevices(
        const device_selection_info& Device_Info =
            device_selection_info_config::DEFAULT,
        const std::optional<vk::SurfaceKHR>& Window_Surface = std::nullopt);

    /// @brief Creates a monitor object.
    [[nodiscard]] monitor_ptr GetMonitor(
        const monitor_info& Monitor_Info = monitor_info_config::DEFAULT);

    /// @brief Returns the primary monitor.
    /// @remark The primary monitor, on most operating systems, contains the
    /// taskbar.
    [[nodiscard]] monitor_ptr GetPrimaryMonitor();

    /// @brief Returns all monitors.
    [[nodiscard]] std::vector<monitor_ptr> AllMonitors();

    /// @brief Creates a standard GLFW cursor.
    [[nodiscard]] cursor_ptr CreateCursor(
        const standard_cursor_info& Standard_Cursor_Info =
            standard_cursor_info_config::DEFAULT);

    /// @brief Creates a custom GLFW cursor.
    [[nodiscard]] cursor_ptr CreateCursor(
        const custom_cursor_info& Custom_Cursor_Info);
};

} // namespace gvw