// Standard includes

// External includes

// Local includes
#include "monitor.hpp"
#include "init.hpp"

monitor_t::monitor_t(
    GLFWmonitor* Monitor_Handle,
    gvw::gvw_error_callback GVW_Error_Callback,
    GLFWerrorfun GLFW_Error_Callback,
    const gvw::glfw_shared_init_hints& Shared_Init_Hints,
    const gvw::glfw_macos_init_hints& MacOS_Init_Hints) noexcept
    : monitorHandle(Monitor_Handle)
{
    // Initialize GLFW if it hasn't been initialized yet.
    if (gvw::INSTANCE_COUNT == 0) {
        gvw::Init(GVW_Error_Callback,
                  GLFW_Error_Callback,
                  Shared_Init_Hints,
                  MacOS_Init_Hints);
    }
    gvw::INSTANCE_COUNT++;
}

monitor_t::~monitor_t()
{
    gvw::INSTANCE_COUNT--;

    // Terminate GVW if there aren't any instances left to manage.
    if (gvw::INSTANCE_COUNT == 0) {
        // Terminate GLFW
        glfwTerminate();
    }
}

GLFWmonitor* monitor_t::MonitorHandle() const noexcept
{
    return this->monitorHandle;
}

void monitor_t::MonitorHandle(GLFWmonitor* Monitor_Handle) noexcept
{
    this->monitorHandle = Monitor_Handle;
}

const GLFWvidmode* monitor_t::VideoMode() const
{
    return glfwGetVideoMode(this->monitorHandle);
}

std::vector<const GLFWvidmode*> monitor_t::SupportedVideoModes() const
{
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

gvw::area<int> monitor_t::PhysicalSize() const
{
    gvw::area<int> physicalSize = { 0, 0 };
    glfwGetMonitorPhysicalSize(
        this->monitorHandle, &physicalSize.width, &physicalSize.height);
    return physicalSize;
}

gvw::coordinate<float> monitor_t::ContentScale() const
{
    gvw::coordinate<float> contentScale = { 0.0F, 0.0F };
    glfwGetMonitorContentScale(
        this->monitorHandle, &contentScale.x, &contentScale.y);
    return contentScale;
}

gvw::coordinate<int> monitor_t::VirtualPosition() const
{
    gvw::coordinate<int> virtualPosition = { 0, 0 };
    glfwGetMonitorPos(
        this->monitorHandle, &virtualPosition.x, &virtualPosition.y);
    return virtualPosition;
}

gvw::coordinate<int> monitor_t::WorkAreaPosition() const
{
    gvw::coordinate<int> workAreaPosition = { 0, 0 };
    glfwGetMonitorWorkarea(this->monitorHandle,
                           &workAreaPosition.x,
                           &workAreaPosition.y,
                           nullptr,
                           nullptr);
    return workAreaPosition;
}

gvw::area<int> monitor_t::WorkAreaSize() const
{
    gvw::area<int> workAreaSize = { 0, 0 };
    glfwGetMonitorWorkarea(this->monitorHandle,
                           nullptr,
                           nullptr,
                           &workAreaSize.width,
                           &workAreaSize.height);
    return workAreaSize;
}

const char* monitor_t::Name() const
{
    return glfwGetMonitorName(this->monitorHandle);
}

const GLFWgammaramp* monitor_t::GammaRamp() const
{
    return glfwGetGammaRamp(this->monitorHandle);
}

void monitor_t::GammaRamp(const GLFWgammaramp& Gamma_Ramp) const
{
    glfwSetGammaRamp(this->monitorHandle, &Gamma_Ramp);
}

void monitor_t::ResetGammaRamp() const
{
    glfwSetGamma(this->monitorHandle, gvw::DEFAULT_GAMMA);
}

namespace gvw {

monitor_t PrimaryMonitor(gvw::gvw_error_callback GVW_Error_Callback,
                         GLFWerrorfun GLFW_Error_Callback,
                         const gvw::glfw_shared_init_hints& Shared_Init_Hints,
                         const gvw::glfw_macos_init_hints& MacOS_Init_Hints)
{
    // Initialize GVW if it hasn't been initialized yet.
    if (gvw::INSTANCE_COUNT == 0) {
        gvw::Init(GVW_Error_Callback,
                  GLFW_Error_Callback,
                  Shared_Init_Hints,
                  MacOS_Init_Hints);
    }

    return { glfwGetPrimaryMonitor() };
}

std::vector<monitor_t> AllMonitors(
    gvw::gvw_error_callback GVW_Error_Callback,
    GLFWerrorfun GLFW_Error_Callback,
    const gvw::glfw_shared_init_hints& Shared_Init_Hints,
    const gvw::glfw_macos_init_hints& MacOS_Init_Hints)
{
    // Initialize GVW if it hasn't been initialized yet.
    if (gvw::INSTANCE_COUNT == 0) {
        gvw::Init(GVW_Error_Callback,
                  GLFW_Error_Callback,
                  Shared_Init_Hints,
                  MacOS_Init_Hints);
    }

    int monitorCount = 0;
    GLFWmonitor** monitorPointerArray = glfwGetMonitors(&monitorCount);
    std::vector<monitor_t> monitors = { static_cast<size_t>(monitorCount),
                                        { nullptr } };
    for (int i = 0; i < monitorCount; ++i) {
        monitors.at(i).MonitorHandle(monitorPointerArray[i]); // NOLINT
    }
    return monitors;
}

} // namespace gvw