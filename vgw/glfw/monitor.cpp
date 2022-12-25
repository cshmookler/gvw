#include "monitor.hpp"

namespace glfw {

Monitor::Monitor() {}

Monitor::Monitor(GLFWmonitor* monitorId)
{
    this->Init(monitorId);
}

Monitor::~Monitor() {}

void Monitor::AssertInitialization()
{
    if (this->monitorId_ == MONITOR_ID_DEFAULT) {
        ErrorCallback(ERROR_MONITOR_ID_NOT_INITIALIZED,
                      ERROR_MESSAGE_MONITOR_ID_NOT_INITIALIZED);
        return;
    }
}

void Monitor::Init(GLFWmonitor* monitor)
{
    this->monitorId_ = monitor;
    if (this->monitorId_ == GLFW_FALSE) {
        ErrorCallback(ERROR_MONITOR_ID_NOT_INITIALIZED,
                      ERROR_MESSAGE_MONITOR_ID_NOT_INITIALIZED);
        return;
    }

    this->GetVideoMode_();
    this->GetPhysicalSize_();
    this->GetContentScale_();
    this->GetVirtualPosition_();
    this->GetWorkArea_();
    this->GetName_();
    this->GetGammaRamp_();
}

Monitor PrimaryMonitor()
{
    Monitor monitor(glfwGetPrimaryMonitor());
    return monitor;
}

std::vector<Monitor> Monitors()
{
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    std::vector<Monitor> result(count);
    for (int index = 0; index < count; index++) {
        result[index].Init(monitors[index]);
    }
    return result;
}

} // namespace glfw