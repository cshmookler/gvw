#include <iostream>
#include <thread>

#include "../../gvw/gvw.hpp"

// int main(int argc, char** argv)
int main()
{
    gvw::glfw_general_hints generalHints = { GLFW_FALSE, GLFW_TRUE,  GLFW_FALSE,
                                             GLFW_TRUE,  GLFW_FALSE, GLFW_TRUE,
                                             GLFW_FALSE, GLFW_FALSE, GLFW_FALSE,
                                             GLFW_TRUE,  GLFW_FALSE };

    auto monitor = gvw::PrimaryMonitor();

    window_t window(monitor.WorkAreaPosition(),
                    monitor.WorkAreaSize(),
                    "",
                    monitor.MonitorHandle(),
                    nullptr,
                    generalHints);

    while (!window.ShouldClose()) {
        window.ClearEvents();
        gvw::WaitThenPollEvents();
        window.ShouldClose(false);

        // if (glfwGetKey(window.WindowHandle(), GLFW_KEY_LEFT_SUPER) ==
        //     GLFW_PRESS) {
        //     glfwIconifyWindow(window.WindowHandle());
        // }

        auto keyEvents = window.KeyEvents();
        for (const auto& keyEvent : keyEvents) {
            if (keyEvent.key == GLFW_KEY_SPACE) {
                window.ShouldClose(true);
            } else if (keyEvent.key == GLFW_KEY_LEFT_SUPER ||
                       keyEvent.key == GLFW_KEY_RIGHT_SUPER) {
                window.Focus();
            }
        }
    }

    return 0;
}