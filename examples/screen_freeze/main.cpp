#include <iostream>
#include <list>
#include <thread>

#include "../../gvw/gvw.hpp"

int main() // NOLINT
{
    std::this_thread::sleep_for(std::chrono::seconds(3));

    gvw::instance_ptr gvw = gvw::CreateInstance();

    gvw::window_event_callbacks eventCallbacks = {
        .keyCallback =
            gvw::window_key_event_callback_config::APPEND_TO_KEY_EVENT_BUFFER
    };

    std::vector<gvw::xy_rgb> vertices = {
        { { -1.0F, -1.0F }, { 0.0F, 0.0F, 1.0F } },
        { { 1.0F, -1.0F }, { 1.0F, 0.0F, 0.0F } },
        { { -1.0F, 1.0F }, { 0.0F, 1.0F, 0.0F } },
        { { -1.0F, 1.0F }, { 0.0F, 1.0F, 0.0F } },
        { { 1.0F, -1.0F }, { 1.0F, 0.0F, 0.0F } },
        { { 1.0F, 1.0F }, { 0.0F, 0.0F, 1.0F } }
    };

    std::vector<gvw::monitor_ptr> monitors = gvw->AllMonitors();
    std::vector<gvw::window_ptr> windows(monitors.size());
    for (size_t i = 0; i < monitors.size(); ++i) {
        const auto& monitor = monitors.at(i);
        windows.at(i) = gvw->CreateWindow(
            { .position = monitor->GetWorkAreaPosition(),
              .size = monitor->GetWorkAreaSize(),
              .title = " ",
              .fullScreenMonitor = monitor,
              //   .creationHints = { { .resizable = GLFW_FALSE,
              //                        .visible = GLFW_FALSE,
              //                        .decorated = GLFW_FALSE,
              //                        .focused = GLFW_TRUE,
              //                        .autoIconify = GLFW_FALSE,
              //                        .floating = GLFW_TRUE,
              //                        .maximized = GLFW_FALSE,
              //                        .centerCursor = GLFW_FALSE,
              //                        .transparentFramebuffer = GLFW_FALSE,
              //                        .focusOnShow = GLFW_TRUE,
              //                        .scaleToMonitor = GLFW_FALSE } },
              .eventCallbacks = eventCallbacks,
              .staticVertices = vertices,
              .sizeOfDynamicDataVerticesInBytes =
                  (sizeof(gvw::xy_rgb) * vertices.size()) });
    }

    bool shouldClose = false;
    bool leftCtrlPressed = false;
    bool leftAltPressed = false;
    bool uPressed = false;

    while (true) {
        for (auto& window : windows) {
            window->ClearEvents();
        }

        gvw->WaitThenPollEvents();

        for (auto& window : windows) {
            auto keyEvents = window->GetKeyEvents();
            for (const auto& keyEvent : keyEvents) {
                if (keyEvent.key == gvw::window_key::eLeftControl) {
                    if (keyEvent.action == gvw::window_key_action::ePress) {
                        leftCtrlPressed = true;
                    } else if (keyEvent.action ==
                               gvw::window_key_action::eRelease) {
                        leftCtrlPressed = false;
                    }
                } else if (keyEvent.key == gvw::window_key::eLeftAlt) {
                    if (keyEvent.action == gvw::window_key_action::ePress) {
                        leftAltPressed = true;
                    } else if (keyEvent.action ==
                               gvw::window_key_action::eRelease) {
                        leftAltPressed = false;
                    }
                } else if (keyEvent.key == gvw::window_key::eU) {
                    if (keyEvent.action == gvw::window_key_action::ePress) {
                        uPressed = true;
                    } else if (keyEvent.action ==
                               gvw::window_key_action::eRelease) {
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