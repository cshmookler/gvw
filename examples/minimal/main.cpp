#include <iostream>
#include <thread>
#include <variant>
#include "../../gvw/gvw.hpp"

int main()
{
    // Constants
    const char* title = "Minimal";

    // Create the window.
    gvw::ptr gvw =
        gvw::Get({ .applicationInfo = { .pApplicationName = title,
                                        .applicationVersion =
                                            VK_MAKE_VERSION(1, 0, 0) } });

    std::vector<gvw::vertex> vertices = {
        { { -0.2F, -0.2F }, { 0.0F, 0.0F, 1.0F } },
        { { 0.2F, -0.2F }, { 1.0F, 0.0F, 0.0F } },
        { { -0.2F, 0.2F }, { 0.0F, 1.0F, 0.0F } },
        { { -0.2F, 0.2F }, { 0.0F, 1.0F, 0.0F } },
        { { 0.2F, -0.2F }, { 1.0F, 0.0F, 0.0F } },
        { { 0.2F, 0.2F }, { 0.0F, 0.0F, 1.0F } }
    };

    gvw::window_event_callbacks eventCallbacks = gvw::NO_WINDOW_EVENT_CALLBACKS;
    // eventCallbacks.keyCallback = gvw::AppendToKeyEventBuffer;

    gvw::window_ptr window = gvw->CreateWindow(
        { .title = title,
          .eventCallbacks = eventCallbacks,
          .deviceInfo = { .presentModes = gvw::present_modes::FIFO },
          .sizeOfVerticesInBytes = (sizeof(gvw::vertex) * vertices.size()) });

    float playerSpeed = 0.01F;

    std::chrono::nanoseconds deltaTime{ 0 };
    std::chrono::nanoseconds timeElapsed{ 0 };
    int fps = 0;
    std::chrono::time_point<std::chrono::system_clock> timeAtStartOfLastFrame =
        std::chrono::system_clock::now();

    while (!window->ShouldClose()) {

        deltaTime = (std::chrono::system_clock::now() - timeAtStartOfLastFrame);
        timeElapsed += deltaTime;
        if (std::chrono::duration_cast<std::chrono::seconds>(timeElapsed) >=
            std::chrono::seconds(1)) {
            std::cout << fps << std::endl;
            fps = 0;
            timeElapsed = std::chrono::nanoseconds(0);
        }
        fps++;
        timeAtStartOfLastFrame = std::chrono::system_clock::now();

        if (window->KeyState(GLFW_KEY_W) == GLFW_PRESS) {
            vertices.at(0).position[1] -= playerSpeed;
            vertices.at(1).position[1] -= playerSpeed;
            vertices.at(2).position[1] -= playerSpeed;
            vertices.at(3).position[1] -= playerSpeed;
            vertices.at(4).position[1] -= playerSpeed;
            vertices.at(5).position[1] -= playerSpeed;
        }
        if (window->KeyState(GLFW_KEY_A) == GLFW_PRESS) {
            vertices.at(0).position[0] -= playerSpeed;
            vertices.at(1).position[0] -= playerSpeed;
            vertices.at(2).position[0] -= playerSpeed;
            vertices.at(3).position[0] -= playerSpeed;
            vertices.at(4).position[0] -= playerSpeed;
            vertices.at(5).position[0] -= playerSpeed;
        }
        if (window->KeyState(GLFW_KEY_S) == GLFW_PRESS) {
            vertices.at(0).position[1] += playerSpeed;
            vertices.at(1).position[1] += playerSpeed;
            vertices.at(2).position[1] += playerSpeed;
            vertices.at(3).position[1] += playerSpeed;
            vertices.at(4).position[1] += playerSpeed;
            vertices.at(5).position[1] += playerSpeed;
        }
        if (window->KeyState(GLFW_KEY_D) == GLFW_PRESS) {
            vertices.at(0).position[0] += playerSpeed;
            vertices.at(1).position[0] += playerSpeed;
            vertices.at(2).position[0] += playerSpeed;
            vertices.at(3).position[0] += playerSpeed;
            vertices.at(4).position[0] += playerSpeed;
            vertices.at(5).position[0] += playerSpeed;
        }

        window->DrawFrame(vertices);

        // Wait until a window event is received.
        gvw->PollEvents();
        // gvw->WaitThenPollEvents();
    }

    window->logicalDevice.waitIdle();
}