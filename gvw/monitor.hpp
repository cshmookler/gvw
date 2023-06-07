#pragma once

/**
 * @file monitor.hpp
 * @author Caden Shmookler (cadenshmookler@gmail.com)
 * @brief Monitor management.
 * @date 2023-05-28
 */

// Standard includes
#include <vector>

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Local includes
#include "init.hpp"
#include "types.tpp"

/// @brief Accesses monitor properties.
class monitor_t // NOLINT
{
  private:
    //////////////////////////////////////////////////
    //               Private Variables              //
    //////////////////////////////////////////////////

    GLFWmonitor* monitorHandle = nullptr;

  public:
    //////////////////////////////////////////////////
    //               Public Functions               //
    //////////////////////////////////////////////////

    /// @brief Initializes the monitor object.
    /// @remark The last 4 parameters are for GVW initialization. If GVW is
    /// already initialized (this function has been run once before), then
    /// arguments passed to these parameters are ignored.
    /// @param Monitor_Handle The monitor handle.
    /// @param GVW_Error_Callback The GVW error callback. The default GVW error
    /// callback throws all errors.
    /// @param GLFW_Error_Callback The GLFW error callback. The default GLFW
    /// error callback throws all errors.
    /// @param Shared_Init_Hints Shared initialization hints for GLFW. Ignored
    /// if GLFW is already initialized.
    /// @param Macos_Init_Hints MacOS specific initialization hints for GLFW.
    /// Ignored if GLFW is already initialized.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Can be called from any thread.
    monitor_t(GLFWmonitor* Monitor_Handle,
              gvw::gvw_error_callback GVW_Error_Callback =
                  gvw::DefaultGvwErrorCallback,
              GLFWerrorfun GLFW_Error_Callback = gvw::DefaultGlfwErrorCallback,
              const gvw::glfw_shared_init_hints& Shared_Init_Hints =
                  gvw::DEFAULT_GLFW_SHARED_INIT_HINTS,
              const gvw::glfw_macos_init_hints& MacOS_Init_Hints =
                  gvw::DEFAULT_GLFW_MACOS_INIT_HINTS) noexcept;

    /// @todo Make this function thread safe!
    /// @brief The destructor is public so as to allow explicit destruction
    /// using the delete operator.
    ~monitor_t();

    /// @brief Returns the handle to the underlying GLFW monitor object.
    /// @returns A pointer to a `GLFWmonitor` object.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Can be called from any thread.
    [[nodiscard]] GLFWmonitor* MonitorHandle() const noexcept;

    /// @brief Sets the handle to the underlying GLFW monitor object.
    /// @param Monitor_Handle THe monitor handle.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Can be called from any thread.
    void MonitorHandle(GLFWmonitor* Monitor_Handle) noexcept;

    /// @brief Returns the video mode of the monitor.
    /// @returns A pointer to a `GLFWvidmode` object.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] const GLFWvidmode* VideoMode() const;

    /// @brief Returns a vector of video modes supported by the monitor.
    /// @returns A vector of pointers to `const GLFWvidmode` objects.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] std::vector<const GLFWvidmode*> SupportedVideoModes() const;

    /// @brief Returns the physical size of the monitor in millimeters.
    /// @returns An `area` object of type int.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] gvw::area<int> PhysicalSize() const;

    /// @brief Returns the content scale of the monitor.
    /// @remark The content scale of the monitor is the ratio between the
    /// current DPI and the platform's default DPI.
    /// @returns A `coordinate` object of type float.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] gvw::coordinate<float> ContentScale() const;

    /// @brief Returns the position of the upper-left corner of the monitor on
    /// the virtual screen in screen coordinates.
    /// @returns A `coordinate` object of type int.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] gvw::coordinate<int> VirtualPosition() const;

    /// @brief Returns the origin of the work area in screen coordinates.
    /// @remark The position of the work area may differ from the monitor
    /// virtual position if a toolbar is placed on top or on the left of the
    /// monitor.
    /// @returns A `coordinate` object of type int.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] gvw::coordinate<int> WorkAreaPosition() const;

    /// @brief Returns the size of the work area in screen coordinates.
    /// @remark The size of the work area may not be the same as the size of the
    /// monitor if there are toolbars present.
    /// @returns An `area` object of type int.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] gvw::area<int> WorkAreaSize() const;

    /// @brief Returns the name of the monitor.
    /// @returns A C-style string.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] const char* Name() const;

    /// @brief Returns the gamma ramp of the monitor.
    /// @returns A pointer to a `const GLFWgammaramp` object.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] const GLFWgammaramp* GammaRamp() const;

    /// @brief Sets the gamma ramp of the monitor.
    /// @param Gamma_Ramp The new gamma ramp of the monitor.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void GammaRamp(const GLFWgammaramp& Gamma_Ramp) const;

    /// @brief Resets the gamma ramp of the monitor.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_VALUE`, or
    /// `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void ResetGammaRamp() const;
};

namespace gvw {

//////////////////////////////////////////////////
//                Const Variables               //
//////////////////////////////////////////////////

const float DEFAULT_GAMMA = 1.0F;

//////////////////////////////////////////////////
//                   Functions                  //
//////////////////////////////////////////////////

/// @brief Returns a `monitor` object cooresponding to the primary monitor.
/// @remark The primary monitor, on most operating systems, contains the
/// taskbar.
/// @remark The 4 parameters are for GVW initialization. If GVW is
/// already initialized (this function has been run once before), then
/// arguments passed to these parameters are ignored.
/// @param GVW_Error_Callback The GVW error callback. The default GVW error
/// callback throws all errors.
/// @param GLFW_Error_Callback The GLFW error callback. The default GLFW
/// error callback throws all errors.
/// @param Shared_Init_Hints Shared initialization hints for GLFW. Ignored
/// if GLFW is already initialized.
/// @param Macos_Init_Hints MacOS specific initialization hints for GLFW.
/// Ignored if GLFW is already initialized.
/// @returns A `monitor_t` object.
/// @gvw_errors None.
/// @glfw_errors `GLFW_NOT_INITIALIZED`.
/// @thread_safety Must be called from the main thread.
[[nodiscard]] monitor_t PrimaryMonitor(
    gvw::gvw_error_callback GVW_Error_Callback = gvw::DefaultGvwErrorCallback,
    GLFWerrorfun GLFW_Error_Callback = gvw::DefaultGlfwErrorCallback,
    const gvw::glfw_shared_init_hints& Shared_Init_Hints =
        gvw::DEFAULT_GLFW_SHARED_INIT_HINTS,
    const gvw::glfw_macos_init_hints& MacOS_Init_Hints =
        gvw::DEFAULT_GLFW_MACOS_INIT_HINTS);

/// @brief Returns a vector of `monitor` objects for each physical monitor.
/// @remark The 4 parameters are for GVW initialization. If GVW is
/// already initialized (this function has been run once before), then
/// arguments passed to these parameters are ignored.
/// @param GVW_Error_Callback The GVW error callback. The default GVW error
/// callback throws all errors.
/// @param GLFW_Error_Callback The GLFW error callback. The default GLFW
/// error callback throws all errors.
/// @param Shared_Init_Hints Shared initialization hints for GLFW. Ignored
/// if GLFW is already initialized.
/// @param Macos_Init_Hints MacOS specific initialization hints for GLFW.
/// Ignored if GLFW is already initialized.
/// @returns A vector of `monitor` objects.
/// @gvw_errors None.
/// @glfw_errors `GLFW_NOT_INITIALIZED`.
/// @thread_safety Must be called from the main thread.
[[nodiscard]] std::vector<monitor_t> AllMonitors(
    gvw::gvw_error_callback GVW_Error_Callback = gvw::DefaultGvwErrorCallback,
    GLFWerrorfun GLFW_Error_Callback = gvw::DefaultGlfwErrorCallback,
    const gvw::glfw_shared_init_hints& Shared_Init_Hints =
        gvw::DEFAULT_GLFW_SHARED_INIT_HINTS,
    const gvw::glfw_macos_init_hints& MacOS_Init_Hints =
        gvw::DEFAULT_GLFW_MACOS_INIT_HINTS);

} // namespace gvw