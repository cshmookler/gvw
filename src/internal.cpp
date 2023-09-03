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
    }
    instance_error_callback_config::CERR_THROW(error);
}

/********************************    Window    ********************************/
void* GetUserPointerNoMutex(GLFWwindow* Window)
{
    // The GLFW error callback is invoked if GLFW (GVW by proxy) is
    // uninitialized.
    return glfwGetWindowUserPointer(Window);
}

void* GetUserPointer(GLFWwindow* Window)
{
    std::scoped_lock lock(global::GLFW_MUTEX);
    return GetUserPointerNoMutex(Window);
}

/********************************    Global    ********************************/
namespace global {
instance_ptr GVW_INSTANCE = nullptr;
std::mutex GLFW_MUTEX;
std::mutex CONSOLE_MUTEX;
instance_warning_callback WARNING_CALLBACK = nullptr;
std::mutex WARNING_CALLBACK_MUTEX;
instance_error_callback ERROR_CALLBACK = nullptr;
std::mutex ERROR_CALLBACK_MUTEX;
instance_glfw_error_callback GLFW_ERROR_CALLBACK = nullptr;
std::vector<instance_joystick_event> JOYSTICK_EVENTS;
std::mutex JOYSTICK_EVENTS_MUTEX;
} // namespace global

} // namespace gvw::internal