// Standard includes
#include <stdexcept>

// External includes

// Local includes
#include "init.hpp"

namespace gvw {

const error ERROR_GLFW_INIT_FAILED = { 0x00100001,
                                       "\"GLFW failed to initialize\"" };
const error ERROR_VULKAN_NOT_SUPPORTED = {
    0x00100002,
    "\"Vulkan is not supported on this system\""
};

const glfw_shared_init_hints DEFAULT_GLFW_SHARED_INIT_HINTS = {};
const glfw_macos_init_hints DEFAULT_GLFW_MACOS_INIT_HINTS = {};

const glfw_general_hints DEFAULT_GLFW_GENERAL_HINTS = {};
const glfw_framebuffer_hints DEFAULT_GLFW_FRAMEBUFFER_HINTS = {};
const glfw_monitor_hints DEFAULT_GLFW_MONITOR_HINTS = {};
const glfw_context_hints DEFAULT_GLFW_CONTEXT_HINTS = {};
const glfw_macos_window_hints DEFAULT_GLFW_MACOS_WINDOW_HINTS = {};
const glfw_linux_window_hints DEFAULT_GLFW_LINUX_WINDOW_HINTS = {};

const area<int> DEFAULT_WINDOW_SIZE = { 640, 360 };
const char* const DEFAULT_WINDOW_TITLE = "Untitled window";
const area<int> DEFAULT_MINIMUM_WINDOW_SIZE = { GLFW_DONT_CARE,
                                                GLFW_DONT_CARE };
const area<int> DEFAULT_MAXIMUM_WINDOW_SIZE = { GLFW_DONT_CARE,
                                                GLFW_DONT_CARE };

// NOLINTBEGIN
size_t INSTANCE_COUNT = 0;
gvw_error_callback GVW_ERROR_CALLBACK = DefaultGvwErrorCallback;
std::vector<joystick_event> JOYSTICK_EVENTS;
// NOLINTEND

void DefaultGvwErrorCallback(error GVW_Error)
{
    throw std::runtime_error("\033[1;31mGVW error [" +
                             std::to_string(GVW_Error.code) +
                             "]: " + GVW_Error.description + "\033[0m");
}

void DefaultGlfwErrorCallback(int Error_Code, const char* Description)
{
    throw std::runtime_error("\033[1;31mGLFW error [" +
                             std::to_string(Error_Code) + "]: " + Description +
                             "\033[0m");
}

version GetGlfwRuntimeVersion() noexcept
{
    version glfwVersion = { 0, 0, 0 };
    glfwGetVersion(
        &glfwVersion.major, &glfwVersion.minor, &glfwVersion.revision);
    return glfwVersion;
}

version GetGlfwCompiletimeVersion() noexcept
{
    return version{ GLFW_VERSION_MAJOR,
                    GLFW_VERSION_MINOR,
                    GLFW_VERSION_REVISION };
}

void SetGvwErrorCallback(gvw_error_callback GVW_Error_Callback) noexcept
{
    GVW_ERROR_CALLBACK = GVW_Error_Callback;
}

void SetGlfwErrorCallback(GLFWerrorfun GLFW_Error_Callback) noexcept
{
    glfwSetErrorCallback(GLFW_Error_Callback);
}

void JoystickCallback(int JID, int Event)
{
    JOYSTICK_EVENTS.emplace_back(JID, Event);
}

std::vector<joystick_event>& JoystickEvents()
{
    return JOYSTICK_EVENTS;
}

void ClearJoystickEvents()
{
    JOYSTICK_EVENTS.clear();
}

void Init(gvw_error_callback GVW_Error_Callback,
          GLFWerrorfun GLFW_Error_Callback,
          const glfw_shared_init_hints& Shared_Init_Hints,
          const glfw_macos_init_hints& MacOS_Init_Hints)
{
    // Set error callbacks
    gvw::SetGvwErrorCallback(GVW_Error_Callback);
    gvw::SetGlfwErrorCallback(GLFW_Error_Callback);

    // Apply GLFW initialization hints
    Shared_Init_Hints.Apply();
    MacOS_Init_Hints.Apply();

    // Initialize the GLFW library
    if (glfwInit() == GLFW_FALSE) {
        gvw::GVW_ERROR_CALLBACK(gvw::ERROR_GLFW_INIT_FAILED);
    }

    // Check for Vulkan support
    if (glfwVulkanSupported() == GLFW_FALSE) {
        gvw::GVW_ERROR_CALLBACK(gvw::ERROR_VULKAN_NOT_SUPPORTED);
    }
}

void PollEvents()
{
    glfwPollEvents();
}

void WaitThenPollEvents()
{
    glfwWaitEvents();
}

void WaitThenPollEvents(double Timeout)
{
    glfwWaitEventsTimeout(Timeout);
}

void PostEmptyEvent()
{
    glfwPostEmptyEvent();
}

} // namespace gvw