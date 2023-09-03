// Standard includes
#include <iostream>

// Local includes
#include "gvw.hpp"

namespace gvw {

monitor::monitor(GLFWmonitor* Monitor_Handle)
    : gvwInstance(internal::global::GVW_INSTANCE)
    , monitorHandle(Monitor_Handle)
{
    internal::AssertInitialization();
}

GLFWmonitor* monitor::GetHandle() const noexcept
{
    return this->monitorHandle;
}

const GLFWvidmode* monitor::GetVideoMode()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    return glfwGetVideoMode(this->monitorHandle);
}

std::vector<const GLFWvidmode*> monitor::GetSupportedVideoModes() const
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    int videoModeCount = 0;
    const GLFWvidmode* videoModePointerArray =
        glfwGetVideoModes(this->monitorHandle, &videoModeCount);
    std::vector<const GLFWvidmode*> videoModes = {
        static_cast<size_t>(videoModeCount), nullptr
    };
    for (int i = 0; i < videoModeCount; ++i) {
        videoModes.at(i) = &videoModePointerArray[i]; // NOLINT
    }
    return videoModes;
}

gvw::area<int> monitor::GetPhysicalSize() const
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    area<int> physicalSize = { 0, 0 };
    glfwGetMonitorPhysicalSize(
        this->monitorHandle, &physicalSize.width, &physicalSize.height);
    return physicalSize;
}

gvw::coordinate<float> monitor::GetContentScale() const
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    coordinate<float> contentScale = { 0.0F, 0.0F };
    glfwGetMonitorContentScale(
        this->monitorHandle, &contentScale.x, &contentScale.y);
    return contentScale;
}

gvw::coordinate<int> monitor::GetVirtualPosition() const
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    coordinate<int> virtualPosition = { 0, 0 };
    glfwGetMonitorPos(
        this->monitorHandle, &virtualPosition.x, &virtualPosition.y);
    return virtualPosition;
}

gvw::coordinate<int> monitor::GetWorkAreaPosition() const
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    coordinate<int> workAreaPosition = { 0, 0 };
    glfwGetMonitorWorkarea(this->monitorHandle,
                           &workAreaPosition.x,
                           &workAreaPosition.y,
                           nullptr,
                           nullptr);
    return workAreaPosition;
}

gvw::area<int> monitor::GetWorkAreaSize() const
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    area<int> workAreaSize = { 0, 0 };
    glfwGetMonitorWorkarea(this->monitorHandle,
                           nullptr,
                           nullptr,
                           &workAreaSize.width,
                           &workAreaSize.height);
    return workAreaSize;
}

const char* monitor::GetName() const
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    return glfwGetMonitorName(this->monitorHandle);
}

const GLFWgammaramp* monitor::GetGammaRamp() const
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    return glfwGetGammaRamp(this->monitorHandle);
}

void monitor::SetGammaRamp(const GLFWgammaramp& Gamma_Ramp) const
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetGammaRamp(this->monitorHandle, &Gamma_Ramp);
}

void monitor::SetGamma(const monitor_gamma& Gamma) const
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetGamma(this->monitorHandle, Gamma);
}

void monitor::ResetGamma() const
{
    this->SetGamma(monitor_gamma_config::DEFAULT);
}

} // namespace gvw