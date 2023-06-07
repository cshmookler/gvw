#include <iostream>
#include <thread>

#include "../../gvw/gvw.hpp"

int main(int argc, char** argv)
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

    window.ExitFullScreen();
    window.Decorate();
    window.Position({ 100, 100 });
    window.Size(window_t::DEFAULT_WINDOW_SIZE);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    window.EnterFullScreen(monitor);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    window.ExitFullScreen();

    while (!window.ShouldClose()) {
        window.ClearEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // NOLINT
        gvw::PollEvents();
        window.ShouldClose(false);

        auto keyEvents = window.KeyEvents();
        for (const auto& keyEvent : keyEvents) {
            if (keyEvent.key == GLFW_KEY_SPACE) {
                window.ShouldClose(true);
            } else if (keyEvent.key == GLFW_KEY_LEFT_SUPER ||
                       keyEvent.key == GLFW_KEY_RIGHT_SUPER) {
                std::cout << "focusing" << std::endl;
                window.Focus();
            }
        }
    }

    return 0;
}