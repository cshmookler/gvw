#include "monitor.tpp"

// Local includes
#include "../common/global.hpp"
#include "init.tpp"

namespace gvw {

bool monitor::AssertInitialization_()
{
    if (this->monitorId_ == con::MONITOR_ID_NULL) {
        global::ERROR_CALLBACK(con::ERROR_MONITOR_ID_NOT_INITIALIZED,
                               con::ERROR_MESSAGE_MONITOR_ID_NOT_INITIALIZED);
        return con::ASSERT_FAILURE;
    }

    return con::ASSERT_SUCCESS;
}

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

GLFWmonitor* monitor::Id()
{
    if (this->AssertInitialization_() == con::ASSERT_FAILURE) {
        return nullptr;
    }
    return this->monitorId_;
}

const GLFWvidmode* monitor::VideoMode()
{
    if (this->AssertInitialization_() == con::ASSERT_FAILURE) {
        return nullptr;
    }

    return glfwGetVideoMode(this->monitorId_);
}

size<int> monitor::PhysicalSize()
{
    size<int> physicalSize = { -1, -1 };
    if (this->AssertInitialization_() == con::ASSERT_FAILURE) {
        return physicalSize;
    }

    glfwGetMonitorPhysicalSize(
        this->monitorId_, &physicalSize.width, &physicalSize.height);

    return physicalSize;
}

coordinate<float> monitor::ContentScale()
{
    coordinate<float> contentScale = { -1.0F, -1.0F };
    if (this->AssertInitialization_() == con::ASSERT_FAILURE) {
        return contentScale;
    }

    glfwGetMonitorContentScale(
        this->monitorId_, &contentScale.x, &contentScale.y);

    return contentScale;
}

coordinate<int> monitor::VirtualPosition()
{
    coordinate<int> virtualPosition = { -1, -1 };
    if (this->AssertInitialization_() == con::ASSERT_FAILURE) {
        return virtualPosition;
    }

    glfwGetMonitorPos(this->monitorId_, &virtualPosition.x, &virtualPosition.y);

    return virtualPosition;
}

coordinate<int> monitor::WorkAreaPosition()
{
    coordinate<int> workAreaPosition;
    if (this->AssertInitialization_() == con::ASSERT_FAILURE) {
        return workAreaPosition;
    }

    glfwGetMonitorWorkarea(this->monitorId_,
                           &workAreaPosition.x,
                           &workAreaPosition.y,
                           nullptr,
                           nullptr);

    return workAreaPosition;
}

size<int> monitor::WorkAreaSize()
{
    size<int> workAreaSize;
    if (this->AssertInitialization_() == con::ASSERT_FAILURE) {
        return workAreaSize;
    }

    glfwGetMonitorWorkarea(this->monitorId_,
                           nullptr,
                           nullptr,
                           &workAreaSize.width,
                           &workAreaSize.height);

    return workAreaSize;
}

const char* monitor::Name()
{
    if (this->AssertInitialization_() == con::ASSERT_FAILURE) {
        return nullptr;
    }
    return glfwGetMonitorName(this->monitorId_);
}

const GLFWgammaramp* monitor::GammaRamp()
{
    if (this->AssertInitialization_() == con::ASSERT_FAILURE) {
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
    glfwSetGamma(this->monitorId_, con::GAMMA_DEFAULT);
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

} // namespace gvw