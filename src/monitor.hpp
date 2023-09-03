#pragma once

/**
 * @file monitor.hpp
 * @author Caden Shmookler (cadenshmookler@gmail.com)
 * @brief Monitor management.
 * @date 2023-05-28
 */

// Local includes
#include "gvw.hpp"

namespace gvw {

class monitor : internal::uncopyable_unmovable // NOLINT
{
    friend internal::monitor_public_constructor;

    ////////////////////////////////////////////////////////////////////////////
    ///                Constructors, Operators, and Destructor               ///
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Initializes the monitor object.
    /// @remark This constructor is made private to prevent if from being called
    /// from outside of GVW.
    monitor(GLFWmonitor* Monitor_Handle);

  public:
    /// @brief The destructor is public so as to allow explicit destruction
    /// using the delete operator.
    ~monitor() = default;

  private:
    ////////////////////////////////////////////////////////////////////////////
    ///                           Private Variables                          ///
    ////////////////////////////////////////////////////////////////////////////

    instance_ptr gvwInstance;

    /// @brief The underlying GLFW monitor handle.
    GLFWmonitor* monitorHandle = nullptr;
    std::mutex monitorMutex;

  public:
    ////////////////////////////////////////////////////////////////////////////
    ///                        Public Member Functions                       ///
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Returns the handle to the underlying GLFW monitor object.
    [[nodiscard]] GLFWmonitor* GetHandle() const noexcept;

    /// @brief Returns the video mode of the monitor.
    [[nodiscard]] const GLFWvidmode* GetVideoMode();

    /// @brief Returns a vector of video modes supported by the monitor.
    [[nodiscard]] std::vector<const GLFWvidmode*> GetSupportedVideoModes()
        const;

    /// @brief Returns the physical size of the monitor in millimeters.
    [[nodiscard]] area<int> GetPhysicalSize() const;

    /// @brief Returns the content scale of the monitor.
    /// @remark The content scale of the monitor is the ratio between the
    /// current DPI and the platform's default DPI.
    [[nodiscard]] coordinate<float> GetContentScale() const;

    /// @brief Returns the position of the upper-left corner of the monitor on
    /// the virtual screen in screen coordinates.
    [[nodiscard]] coordinate<int> GetVirtualPosition() const;

    /// @brief Returns the origin of the work area in screen coordinates.
    /// @remark The position of the work area may differ from the monitor
    /// virtual position if a toolbar is placed on top or on the left of the
    /// monitor.
    [[nodiscard]] coordinate<int> GetWorkAreaPosition() const;

    /// @brief Returns the size of the work area in screen coordinates.
    /// @remark The size of the work area may not be the same as the size of the
    /// monitor if there are toolbars present.
    [[nodiscard]] area<int> GetWorkAreaSize() const;

    /// @brief Returns the name of the monitor.
    [[nodiscard]] const char* GetName() const;

    /// @brief Returns the gamma ramp of the monitor.
    [[nodiscard]] const GLFWgammaramp* GetGammaRamp() const;

    /// @brief Sets the gamma ramp of the monitor.
    /// @param Gamma_Ramp The new gamma ramp of the monitor.
    void SetGammaRamp(const GLFWgammaramp& Gamma_Ramp) const;

    /// @brief Sets the gamma of the monitor.
    void SetGamma(const monitor_gamma& Gamma) const;

    /// @brief Resets the gamma of the monitor.
    void ResetGamma() const;
};

} // namespace gvw