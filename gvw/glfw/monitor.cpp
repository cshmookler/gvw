#include "monitor.tpp"

// Local includes
#include "init.tpp"
#include "window.hpp"
#include "window.tpp"

namespace glfw {

monitor::monitor() = default;

monitor::monitor(GLFWmonitor* monitorId)
{
    this->Init(monitorId);
}

monitor::~monitor() = default;

void monitor::Init(GLFWmonitor* monitorId)
{
    this->monitorId_ = monitorId;
}

bool monitor::AssertInitialization()
{
    if (this->monitorId_ == MONITOR_ID_NULL) {
        ERROR_CALLBACK(ERROR_MONITOR_ID_NOT_INITIALIZED,
                       ERROR_MESSAGE_MONITOR_ID_NOT_INITIALIZED);
        return ASSERT_FAILURE;
    }

    return ASSERT_SUCCESS;
}

GLFWmonitor* monitor::Id()
{
    if (this->AssertInitialization() == ASSERT_FAILURE) {
        return nullptr;
    }
    return this->monitorId_;
}

const GLFWvidmode* monitor::VideoModeInScreenCoordinates()
{
    if (this->AssertInitialization() == ASSERT_FAILURE) {
        return nullptr;
    }

    return glfwGetVideoMode(this->monitorId_);
}

GLFWvidmode* monitor::VideoMode(window& associatedWindow)
{
    this->videoMode_ = *this->VideoModeInScreenCoordinates();
    associatedWindow.ScreenCoordinateToPixel(this->videoMode_.width,
                                             this->videoMode_.height,
                                             this->videoMode_.width,
                                             this->videoMode_.height);

    return &this->videoMode_;
}

size<int> monitor::PhysicalSize()
{
    size<int> physicalSize = { -1, -1 };
    if (this->AssertInitialization() == ASSERT_FAILURE) {
        return physicalSize;
    }

    glfwGetMonitorPhysicalSize(
        this->monitorId_, &physicalSize.width, &physicalSize.height);

    return physicalSize;
}

coordinate<float> monitor::ContentScale()
{
    coordinate<float> contentScale = { -1.0F, -1.0F };
    if (this->AssertInitialization() == ASSERT_FAILURE) {
        return contentScale;
    }

    glfwGetMonitorContentScale(
        this->monitorId_, &contentScale.x, &contentScale.y);

    return contentScale;
}

coordinate<int> monitor::VirtualPosition(window& associatedWindow)
{
    coordinate<int> virtualPosition = { -1, -1 };
    if (this->AssertInitialization() == ASSERT_FAILURE) {
        return virtualPosition;
    }

    glfwGetMonitorPos(this->monitorId_, &virtualPosition.x, &virtualPosition.y);
    associatedWindow.ScreenCoordinateToPixel(virtualPosition.x,
                                             virtualPosition.y,
                                             virtualPosition.x,
                                             virtualPosition.y);

    return virtualPosition;
}

coordinate<int> monitor::WorkAreaPosition(window& associatedWindow)
{
    coordinate<int> workAreaPosition;
    if (this->AssertInitialization() == ASSERT_FAILURE) {
        return workAreaPosition;
    }

    glfwGetMonitorWorkarea(this->monitorId_,
                           &workAreaPosition.x,
                           &workAreaPosition.y,
                           nullptr,
                           nullptr);
    associatedWindow.ScreenCoordinateToPixel(workAreaPosition.x,
                                             workAreaPosition.y,
                                             workAreaPosition.x,
                                             workAreaPosition.y);

    return workAreaPosition;
}

size<int> monitor::WorkAreaSize(window& associatedWindow)
{
    size<int> workAreaSize;
    if (this->AssertInitialization() == ASSERT_FAILURE) {
        return workAreaSize;
    }

    glfwGetMonitorWorkarea(this->monitorId_,
                           nullptr,
                           nullptr,
                           &workAreaSize.width,
                           &workAreaSize.height);
    associatedWindow.ScreenCoordinateToPixel(workAreaSize.width,
                                             workAreaSize.height,
                                             workAreaSize.width,
                                             workAreaSize.height);

    return workAreaSize;
}

const char* monitor::Name()
{
    if (this->AssertInitialization() == ASSERT_FAILURE) {
        return nullptr;
    }
    return glfwGetMonitorName(this->monitorId_);
}

const GLFWgammaramp* monitor::GammaRamp()
{
    if (this->AssertInitialization() == ASSERT_FAILURE) {
        return nullptr;
    }
    return glfwGetGammaRamp(this->monitorId_);
}

void monitor::SetGammaRamp(const GLFWgammaramp& gammaRamp)
{
    glfwSetGammaRamp(this->monitorId_, &gammaRamp);
}

void monitor::ResetGammaRamp()
{
    glfwSetGamma(this->monitorId_, GAMMA_DEFAULT);
}

monitor PrimaryMonitor()
{
    monitor monitor(glfwGetPrimaryMonitor());
    return monitor;
}

std::vector<monitor> Monitors()
{
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    std::vector<monitor> result(count);
    for (int index = 0; index < count; index++) {
        result[index].Init(monitors[index]);
    }
    return result;
}

} // namespace glfw