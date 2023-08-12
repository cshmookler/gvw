// Local includes
#include "monitor.hpp"

gvw::monitor::monitor(ptr GVW, GLFWmonitor* Monitor_Handle) noexcept
    : gvw(std::move(GVW))
    , monitorHandle(Monitor_Handle)
{
}

GLFWmonitor* gvw::monitor::Handle() const noexcept
{
    return this->monitorHandle;
}

void gvw::monitor::Handle(GLFWmonitor* Handle) noexcept
{
    this->monitorHandle = Handle;
}

const GLFWvidmode* gvw::monitor::VideoMode()
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    return glfwGetVideoMode(this->monitorHandle);
}

std::vector<const GLFWvidmode*> gvw::monitor::SupportedVideoModes() const
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
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

gvw::area<int> gvw::monitor::PhysicalSize() const
{
    area<int> physicalSize = { 0, 0 };
    this->gvw->glfwMutex.lock();
    glfwGetMonitorPhysicalSize(
        this->monitorHandle, &physicalSize.width, &physicalSize.height);
    this->gvw->glfwMutex.unlock();
    return physicalSize;
}

gvw::coordinate<float> gvw::monitor::ContentScale() const
{
    coordinate<float> contentScale = { 0.0F, 0.0F };
    this->gvw->glfwMutex.lock();
    glfwGetMonitorContentScale(
        this->monitorHandle, &contentScale.x, &contentScale.y);
    this->gvw->glfwMutex.unlock();
    return contentScale;
}

gvw::coordinate<int> gvw::monitor::VirtualPosition() const
{
    coordinate<int> virtualPosition = { 0, 0 };
    this->gvw->glfwMutex.lock();
    glfwGetMonitorPos(
        this->monitorHandle, &virtualPosition.x, &virtualPosition.y);
    this->gvw->glfwMutex.unlock();
    return virtualPosition;
}

gvw::coordinate<int> gvw::monitor::WorkAreaPosition() const
{
    coordinate<int> workAreaPosition = { 0, 0 };
    this->gvw->glfwMutex.lock();
    glfwGetMonitorWorkarea(this->monitorHandle,
                           &workAreaPosition.x,
                           &workAreaPosition.y,
                           nullptr,
                           nullptr);
    this->gvw->glfwMutex.unlock();
    return workAreaPosition;
}

gvw::area<int> gvw::monitor::WorkAreaSize() const
{
    area<int> workAreaSize = { 0, 0 };
    this->gvw->glfwMutex.lock();
    glfwGetMonitorWorkarea(this->monitorHandle,
                           nullptr,
                           nullptr,
                           &workAreaSize.width,
                           &workAreaSize.height);
    this->gvw->glfwMutex.unlock();
    return workAreaSize;
}

const char* gvw::monitor::Name() const
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    return glfwGetMonitorName(this->monitorHandle);
}

const GLFWgammaramp* gvw::monitor::GammaRamp() const
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    return glfwGetGammaRamp(this->monitorHandle);
}

void gvw::monitor::GammaRamp(const GLFWgammaramp& Gamma_Ramp) const
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    glfwSetGammaRamp(this->monitorHandle, &Gamma_Ramp);
}

void gvw::monitor::ResetGammaRamp() const
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    glfwSetGamma(this->monitorHandle, DEFAULT_MONITOR_GAMMA);
}