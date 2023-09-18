// Local includes
#include "gvw.ipp"

namespace gvw::internal {

bool NotInitializedTemplate(bool Condition,
                            const std::string& If_False,
                            const std::string& Function_Name)
{
    if (Condition) {
        return false;
    }

    std::string message =
        "Error in function: \"" + Function_Name + "\". " + If_False;
    ErrorCallback(message.c_str());

    return true;
}

/*****************************    GVW Instance    *****************************/
bool NotInitialized(const std::string& Function_Name)
{
    return NotInitializedTemplate(global::GVW_INSTANCE != nullptr,
                                  "GVW is not initialized",
                                  Function_Name);
}

/********************************    Window    ********************************/

void* GetUserPointer(GLFWwindow* Window)
{
    /// @todo Verify that GVW and GLFW are initialized.
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