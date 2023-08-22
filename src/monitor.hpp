#pragma once

/**
 * @file monitor.hpp
 * @author Caden Shmookler (cadenshmookler@gmail.com)
 * @brief Monitor management.
 * @date 2023-05-28
 */

// Local includes
#include "gvw.hpp"

class gvw::monitor
{
    ////////////////////////////////////////////////////////////
    ///                   Private Variables                  ///
    ////////////////////////////////////////////////////////////

    /// @brief The GVW instance.
    ptr gvw;

    /// @brief The underlying GLFW monitor handle.
    std::atomic<GLFWmonitor*> monitorHandle = nullptr;

    ////////////////////////////////////////////////////////////
    ///       Constructors, Operators, and Destructors       ///
    ////////////////////////////////////////////////////////////

    /// @brief Initializes the monitor object.
    /// @remark This constructor is made private to prevent if from being called
    /// from outside of GVW.
    monitor(ptr GVW, GLFWmonitor* Monitor_Handle) noexcept;

    // Allow the private constructor to be called by the parent class.
    friend class gvw;

  public:
    // Delete the copy constructor, move constructor, copy assignment operator,
    // and move assignment operator. It should not be possible to copy or move
    // this object.
    monitor(const monitor&) = delete;
    monitor(monitor&&) noexcept = delete;
    monitor& operator=(const monitor&) = delete;
    monitor& operator=(monitor&&) = delete;

    /// @brief The destructor is public so as to allow explicit destruction
    /// using the delete operator.
    ~monitor() = default;

    ////////////////////////////////////////////////////////////
    ///                Public Member Functions               ///
    ////////////////////////////////////////////////////////////

    /// @brief Returns the handle to the underlying GLFW monitor object.
    [[nodiscard]] GLFWmonitor* Handle() const noexcept;

    /// @brief Returns the handle to the underlying GLFW monitor object.
    void Handle(GLFWmonitor* Handle) noexcept;

    /// @brief Returns the video mode of the monitor.
    [[nodiscard]] const GLFWvidmode* VideoMode();

    /// @brief Returns a vector of video modes supported by the monitor.
    [[nodiscard]] std::vector<const GLFWvidmode*> SupportedVideoModes() const;

    /// @brief Returns the physical size of the monitor in millimeters.
    [[nodiscard]] area<int> PhysicalSize() const;

    /// @brief Returns the content scale of the monitor.
    /// @remark The content scale of the monitor is the ratio between the
    /// current DPI and the platform's default DPI.
    [[nodiscard]] coordinate<float> ContentScale() const;

    /// @brief Returns the position of the upper-left corner of the monitor on
    /// the virtual screen in screen coordinates.
    [[nodiscard]] coordinate<int> VirtualPosition() const;

    /// @brief Returns the origin of the work area in screen coordinates.
    /// @remark The position of the work area may differ from the monitor
    /// virtual position if a toolbar is placed on top or on the left of the
    /// monitor.
    [[nodiscard]] coordinate<int> WorkAreaPosition() const;

    /// @brief Returns the size of the work area in screen coordinates.
    /// @remark The size of the work area may not be the same as the size of the
    /// monitor if there are toolbars present.
    [[nodiscard]] area<int> WorkAreaSize() const;

    /// @brief Returns the name of the monitor.
    [[nodiscard]] const char* Name() const;

    /// @brief Returns the gamma ramp of the monitor.
    [[nodiscard]] const GLFWgammaramp* GammaRamp() const;

    /// @brief Sets the gamma ramp of the monitor.
    /// @param Gamma_Ramp The new gamma ramp of the monitor.
    void GammaRamp(const GLFWgammaramp& Gamma_Ramp) const;

    /// @brief Resets the gamma ramp of the monitor.
    void ResetGammaRamp() const;
};

class gvw::monitor_public_constructor : public monitor
{
  public:
    template<typename... Args>
    monitor_public_constructor(Args&&... Arguments)
        : monitor(std::forward<Args>(Arguments)...)
    {
    }
};