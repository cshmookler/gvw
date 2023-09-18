#pragma once

/**
 * @file instance.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief GVW instance singleton.
 * @date 2023-08-27
 */

// Standard includes
#include <atomic>

// Local includes
#include "gvw.hpp"
#include "gvw.ipp"
#include "internal.hpp"
#include "internal.ipp"
#include "monitor.hpp"
#include "window.hpp"
#include "device.hpp"

namespace gvw {

class instance : internal::uncopyable_unmovable // NOLINT
{
    friend internal::instance_public_constructor;

    friend class monitor;
    friend class window;
    friend class device;

    struct impl;
    /// @todo Consider wrapping this within std::experimental::propagate_const.
    std::shared_ptr<impl> pImpl;

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

  private:
    ////////////////////////////////////////////////////////////////////////////
    ///                           Private Variables                          ///
    ////////////////////////////////////////////////////////////////////////////

    /// @todo Create flag enum for tracking initialization success.

    // Constant after object construction, so these don't have to be atomic.
    bool initializedGlfw = false;
    bool vulkanSupported = false;
    bool requiredExtensionsSupported = false;
    bool selectedExtensionsSupported = false;
    bool selectedLayersSupported = false;

  public:
    ////////////////////////////////////////////////////////////////////////////
    ///                        Public Static Functions                       ///
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Deinitializes the GLFW library.
    static void TerminateGlfw();

  private:
    ////////////////////////////////////////////////////////////////////////////
    ///                        Private Member Functions                      ///
    ////////////////////////////////////////////////////////////////////////////

    [[nodiscard]] bool GlfwNotInitialized(
        const std::string& Function_Name) const;
    [[nodiscard]] bool VulkanNotSupported(
        const std::string& Function_Name) const;
    [[nodiscard]] bool RequiredExtensionsNotSupported(
        const std::string& Function_Name) const;
    [[nodiscard]] bool SelectedExtensionsNotSupported(
        const std::string& Function_Name) const;
    [[nodiscard]] bool SelectedLayersNotSupported(
        const std::string& Function_Name) const;

  public:
    ////////////////////////////////////////////////////////////////////////////
    ///                        Public Member Functions                       ///
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Sets the joystick event callback.
    void SetJoystickEventCallback(
        instance_joystick_event_callback Joystick_Event_Callback =
            instance_joystick_event_callback_config::NONE);

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
        const vk::SurfaceKHR* Window_Surface = nullptr);

    /// @brief Creates a monitor object.
    [[nodiscard]] monitor_ptr GetMonitor(
        const monitor_info& Monitor_Info = monitor_info_config::DEFAULT);

    /// @brief Returns the primary monitor.
    /// @remark The primary monitor, on most operating systems, contains the
    /// taskbar.
    [[nodiscard]] monitor_ptr GetPrimaryMonitor();

    /// @brief Returns all monitors.
    [[nodiscard]] std::vector<monitor_ptr> GetAllMonitors();

    [[nodiscard]] cursor_ptr CreateCursor(
        cursor_standard_shape Cursor_Standard_Shape =
            cursor_standard_shape::eArrow);
    [[nodiscard]] cursor_ptr CreateCursor(
        const cursor_custom_shape_info& Cursor_Custom_Shape_Info);

    [[nodiscard]] const char* GetClipboard();
    void SetClipboard(const char* Data);

    int GetKeyScancode(window_key Key);
    const char* GetKeyName(window_key Key, int Scancode);
};

} // namespace gvw