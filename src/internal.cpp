// Local includes
#include "gvw.hpp"

namespace gvw::internal {

/*****************************    GVW Instance    *****************************/
void AssertInitialization()
{
    if (global::GVW_INSTANCE != nullptr) {
        return;
    }
    const char* error = "GVW is not initialized.";
    if (global::ERROR_CALLBACK != nullptr) {
        ErrorCallback(error);
        return;
    }
    instance_error_callback_config::CERR(error);
}

/********************************    Window    ********************************/

void* GetUserPointer(GLFWwindow* Window)
{
    return glfwGetWindowUserPointer(Window);
}

/********************************    Global    ********************************/
namespace global {
instance_ptr GVW_INSTANCE = nullptr;
std::mutex GLFW_MUTEX;
std::mutex CONSOLE_MUTEX;
instance_verbose_callback VERBOSE_CALLBACK = nullptr;
std::mutex VERBOSE_CALLBACK_MUTEX;
instance_info_callback INFO_CALLBACK = nullptr;
std::mutex INFO_CALLBACK_MUTEX;
instance_warning_callback WARNING_CALLBACK = nullptr;
std::mutex WARNING_CALLBACK_MUTEX;
instance_error_callback ERROR_CALLBACK = nullptr;
std::mutex ERROR_CALLBACK_MUTEX;
instance_glfw_error_callback GLFW_ERROR_CALLBACK = nullptr;
std::vector<instance_joystick_event> JOYSTICK_EVENTS;
std::mutex JOYSTICK_EVENTS_MUTEX;
} // namespace global

} // namespace gvw::internal