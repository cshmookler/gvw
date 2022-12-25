#pragma once

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>
#include <vector>

// Local includes
#include "glfw.hpp"
#include "glfw_constants.hpp"

namespace glfw {

class Monitor
{
    template<typename T>
    struct Size
    {
        T width;
        T height;
    };

    template<typename T>
    struct Coordinate
    {
        T x;
        T y;
    };

    using PhysicalSize = Size<int>;
    using ContentScale = Coordinate<float>;
    using VirtualPosition = Coordinate<int>;

    struct WorkArea
        : Coordinate<int>
        , Size<int>
    {};

    GLFWmonitor* monitorId_ = MONITOR_ID_DEFAULT;
    const GLFWvidmode* videoMode_;
    PhysicalSize physicalSize_;
    ContentScale contentScale_;
    VirtualPosition virtualPosition_;
    WorkArea workArea_;
    const char* name_;
    const GLFWgammaramp* gammaRamp_;

    inline void GetVideoMode_()
    {
        this->videoMode_ = glfwGetVideoMode(this->monitorId_);
    }

    inline void GetPhysicalSize_()
    {
        glfwGetMonitorPhysicalSize(this->monitorId_,
                                   &this->physicalSize_.width,
                                   &this->physicalSize_.height);
    }

    inline void GetContentScale_()
    {
        glfwGetMonitorContentScale(
            this->monitorId_, &this->contentScale_.x, &this->contentScale_.y);
    }

    inline void GetVirtualPosition_()
    {
        glfwGetMonitorPos(this->monitorId_,
                          &this->virtualPosition_.x,
                          &this->virtualPosition_.y);
    }

    inline void GetWorkArea_()
    {
        glfwGetMonitorWorkarea(this->monitorId_,
                               &this->workArea_.x,
                               &this->workArea_.y,
                               &this->workArea_.width,
                               &this->workArea_.height);
    }

    inline void GetName_()
    {
        this->name_ = glfwGetMonitorName(this->monitorId_);
    }

    inline void GetGammaRamp_()
    {
        this->gammaRamp_ = glfwGetGammaRamp(this->monitorId_);
    }

  public:
    // Constructors
    Monitor();
    Monitor(GLFWmonitor* monitorId);

    // Destructor
    ~Monitor();

    // Checks if the monitor is initialized.
    // If it is not, an error is sent to the GLFW error callback.
    void AssertInitialization();

    // Initialize monitor values
    void Init(GLFWmonitor* monitorId);

    // Return member variables
    inline GLFWmonitor* monitor_id() { return this->monitorId_; }
    inline const GLFWvidmode* video_mode() { return this->videoMode_; }
    inline PhysicalSize physical_size() { return this->physicalSize_; }
    inline ContentScale content_scale() { return this->contentScale_; }
    inline VirtualPosition virtual_position() { return this->virtualPosition_; }
    inline WorkArea work_area() { return this->workArea_; }
    inline const char* name() { return this->name_; }
    inline const GLFWgammaramp* gamma_ramp() { return this->gammaRamp_; }

    // Set gamma ramp
    inline void SetGammaRamp(const GLFWgammaramp& gammaRamp)
    {
        glfwSetGammaRamp(this->monitorId_, &gammaRamp);
        this->GetGammaRamp_();
    }

    // Reset gamma ramp with a default value
    inline void ResetGammaRamp()
    {
        glfwSetGamma(this->monitorId_, GAMMA_DEFAULT);
    }
};

// Returns a 'Monitor' object for the primary monitor
Monitor PrimaryMonitor();

// Returns a vector containing a 'Monitor' object for each physical monitor
std::vector<Monitor> Monitors();

} // namespace glfw