#pragma once

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Local includes
#include "monitor.hpp"

namespace glfw {

class Window
{
    // Window ID
    GLFWwindow* windowId_ = GLFW_FALSE;

    // Create the window
    void Create_(int windowWidth,
                 int windowHeight,
                 const char* windowTitle,
                 const char* windowClassName,
                 GLFWmonitor* fullScreenMonitor,
                 GLFWwindow* contextShareWindow);

  public:
    // Constructors
    Window();
    inline Window(int windowWidth,
                  int windowHeight,
                  const char* windowTitle,
                  const char* windowClassName,
                  GLFWmonitor* fullScreenMonitor,
                  GLFWwindow* contextShareWindow)
    {
        this->Create_(windowWidth,
                      windowHeight,
                      windowTitle,
                      windowClassName,
                      fullScreenMonitor,
                      contextShareWindow);
    }

    // Immediately closes the window
    void Destroy();

    // Destructor (calls the 'Destroy' function)
    ~Window();

    // Delete the copy constructor and copy assignment operators
    // It should not be possible to copy this object
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Checks if the window has been created.
    // If it has not, an error is sent to the GLFW error callback.
    void AssertCreation();

    inline void Create(int windowWidth,
                       int windowHeight,
                       const char* windowTitle,
                       const char* windowClassName = "")
    {
        this->Create_(windowWidth,
                      windowHeight,
                      windowTitle,
                      windowClassName,
                      NULL,
                      NULL);
    }

    void Create(int windowWidth,
                int windowHeight,
                const char* windowTitle,
                const char* windowClassName,
                Monitor& fullScreenMonitor);

    void Create(int windowWidth,
                int windowHeight,
                const char* windowTitle,
                const char* windowClassName,
                Window& contextShareWindow);

    void Create(int windowWidth,
                int windowHeight,
                const char* windowTitle,
                const char* windowClassName,
                Monitor& fullScreenMonitor,
                Window& contextShareWindow);

    inline void Create(int windowWidth,
                       int windowHeight,
                       const char* windowTitle,
                       Monitor& fullScreenMonitor)
    {
        this->Create(
            windowWidth, windowHeight, windowTitle, "", fullScreenMonitor);
    }

    inline void Create(int windowWidth,
                       int windowHeight,
                       const char* windowTitle,
                       Window& contextShareWindow)
    {
        this->Create(
            windowWidth, windowHeight, windowTitle, "", contextShareWindow);
    }

    inline void Create(int windowWidth,
                       int windowHeight,
                       const char* windowTitle,
                       Monitor& fullScreenMonitor,
                       Window& contextShareWindow)
    {
        this->Create(windowWidth,
                     windowHeight,
                     windowTitle,
                     "",
                     fullScreenMonitor,
                     contextShareWindow);
    }

    // // Enters full screen on a specific monitor
    // void FullScreen(Monitor& fullScreenMonitor);

    // // Exits full screen
    // void ExitFullScreen();

    // Returns the state of the 'close' flag
    inline bool ShouldClose() { return glfwWindowShouldClose(this->windowId_); }

    // Set the 'close' flag to true
    // Note: The window does NOT immediately close
    inline void Close() { glfwSetWindowShouldClose(this->windowId_, true); }

    // Set the 'close' flag to false
    inline void CancelClose()
    {
        glfwSetWindowShouldClose(this->windowId_, false);
    }

    // Poll window Events
    inline void PollEvents() { glfwPollEvents(); }
};

} // namespace glfw