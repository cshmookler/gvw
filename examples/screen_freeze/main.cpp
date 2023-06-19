#include <iostream>
#include <list>
#include <thread>
#include <vector>

#include "../../gvw/gvw.hpp"

int main() // NOLINT
{
    auto null = nullptr;
    auto def = std::nullopt;

    std::this_thread::sleep_for(std::chrono::seconds(3));

    gvw::glfw_general_hints generalHints = { GLFW_FALSE, GLFW_FALSE, GLFW_FALSE,
                                             GLFW_TRUE,  GLFW_FALSE, GLFW_TRUE,
                                             GLFW_FALSE, GLFW_FALSE, GLFW_FALSE,
                                             GLFW_TRUE,  GLFW_FALSE };

    const gvw::glfw_input_event_callbacks EVENT_CALLBACKS = { def,  null, null,
                                                              null, null, null,
                                                              null };

    std::vector<gvw::monitor_t> monitors = gvw::AllMonitors();
    std::list<gvw::window_t> windows;
    for (const auto& monitor : monitors) {
        windows.emplace_back(
            gvw::window_info{ .position = monitor.WorkAreaPosition(),
                              .size = monitor.WorkAreaSize(),
                              .title = " ",
                              .inputEventCallbacks = EVENT_CALLBACKS,
                              .fullScreenMonitor = monitor.Handle(),
                              .parentWindow = nullptr,
                              .generalHints = generalHints });
    }

    bool shouldClose = false;
    bool leftCtrlPressed = false;
    bool leftAltPressed = false;
    bool uPressed = false;

    while (true) {
        for (auto& window : windows) {
            window.ClearInputEvents();
        }

        for (auto& window : windows) {
            window.ShouldClose(false);
            auto keyEvents = window.KeyEvents();
            for (const auto& keyEvent : keyEvents) {
                if (keyEvent.key == GLFW_KEY_LEFT_CONTROL) {
                    if (keyEvent.action == GLFW_PRESS) {
                        leftCtrlPressed = true;
                    } else if (keyEvent.action == GLFW_RELEASE) {
                        leftCtrlPressed = false;
                    }
                } else if (keyEvent.key == GLFW_KEY_LEFT_ALT) {
                    if (keyEvent.action == GLFW_PRESS) {
                        leftAltPressed = true;
                    } else if (keyEvent.action == GLFW_RELEASE) {
                        leftAltPressed = false;
                    }
                } else if (keyEvent.key == GLFW_KEY_U) {
                    if (keyEvent.action == GLFW_PRESS) {
                        uPressed = true;
                    } else if (keyEvent.action == GLFW_RELEASE) {
                        uPressed = false;
                    }
                }

                if (leftCtrlPressed && leftAltPressed && uPressed) {
                    shouldClose = true;
                }
            }
        }

        if (shouldClose) {
            break;
        }
    }

    return 0;
}
