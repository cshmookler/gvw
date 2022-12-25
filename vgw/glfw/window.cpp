#include "window.hpp"

// External includes
#include <iostream>

// Local includes
#include "glfw.hpp"
#include "glfw_constants.hpp"

namespace glfw {

void Window::Create_(int windowWidth,
                     int windowHeight,
                     const char* windowTitle,
                     const char* windowClassName,
                     GLFWmonitor* fullScreenMonitor,
                     GLFWwindow* contextShareWindow)
{
    // Check if the window has already been created
    if (this->windowId_ != WINDOW_ID_DEFAULT) {
        ErrorCallback(ERROR_WINDOW_ALREADY_CREATED,
                      ERROR_MESSAGE_WINDOW_ALREADY_CREATED);
        return;
    }

    // Check if the window dimensions are valid
    if (windowWidth < MIN_WINDOW_WIDTH || windowHeight < MIN_WINDOW_HEIGHT) {
        ErrorCallback(ERROR_INVALID_DIMENSIONS,
                      ERROR_MESSAGE_INVALID_DIMENSIONS);
        return;
    }

    // Window related hints
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
    glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
    glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_FALSE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_FALSE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);

    /* ----- Changing these window hints is NOT recommended ----- */

    // Framebuffer related hints
    glfwWindowHint(GLFW_AUX_BUFFERS, GLFW_DONT_CARE);
    glfwWindowHint(GLFW_STEREO, GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, GLFW_DONT_CARE);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    // Monitor related hints
    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

    // Context related hints
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_ROBUSTNESS);
    glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_ANY_RELEASE_BEHAVIOR);
    glfwWindowHint(GLFW_CONTEXT_NO_ERROR, GLFW_FALSE);

#ifdef __APPLE__
    // MacOS specific window hints
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHintString(GLFW_COCOA_FRAME_NAME, windowTitle);
    glfwWindowHint(GLFW_COCOA_GRAPHICS_SWITCHING, GLFW_FALSE);
#elif __linux__
    // Linux specific window hints
    glfwWindowHintString(GLFW_X11_CLASS_NAME, windowClassName);
    glfwWindowHintString(GLFW_X11_INSTANCE_NAME, windowTitle);
#endif

    /* ---------------------------------------------------------- */

    // Create the window
    this->windowId_ = glfwCreateWindow(windowWidth,
                                       windowHeight,
                                       windowTitle,
                                       fullScreenMonitor,
                                       contextShareWindow);
}

Window::Window() {}

void Window::Destroy()
{
    if (this->windowId_ != GLFW_FALSE) {
        glfwDestroyWindow(this->windowId_);
        this->windowId_ = GLFW_FALSE;
    }
}

Window::~Window()
{
    this->Destroy();
}

void Window::AssertCreation()
{
    if (this->windowId_ == WINDOW_ID_DEFAULT) {
        ErrorCallback(ERROR_PARENT_WINDOW_NOT_CREATED,
                      ERROR_MESSAGE_PARENT_WINDOW_NOT_CREATED);
        return;
    }
}

void Window::Create(int windowWidth,
                    int windowHeight,
                    const char* windowTitle,
                    const char* windowClassName,
                    Monitor& fullScreenMonitor)
{
    fullScreenMonitor.AssertInitialization();

    this->Create_(windowWidth,
                  windowHeight,
                  windowTitle,
                  windowClassName,
                  fullScreenMonitor.monitor_id(),
                  NULL);
}

void Window::Create(int windowWidth,
                    int windowHeight,
                    const char* windowTitle,
                    const char* windowClassName,
                    Window& contextShareWindow)
{
    contextShareWindow.AssertCreation();

    this->Create_(windowWidth,
                  windowHeight,
                  windowTitle,
                  windowClassName,
                  NULL,
                  contextShareWindow.windowId_);
}

void Window::Create(int windowWidth,
                    int windowHeight,
                    const char* windowTitle,
                    const char* windowClassName,
                    Monitor& fullScreenMonitor,
                    Window& contextShareWindow)
{
    fullScreenMonitor.AssertInitialization();
    contextShareWindow.AssertCreation();

    this->Create_(windowWidth,
                  windowHeight,
                  windowTitle,
                  windowClassName,
                  fullScreenMonitor.monitor_id(),
                  contextShareWindow.windowId_);
}

// void FullScreen(Monitor& fullScreenMonitor)
// {
//     fullScreenMonitor.AssertInitialization();

//     glfwSetWindowMonitor(this->windowId_,
//                          fullScreenMonitor.monitor_id(),
//                          x,
//                          y,
//                          width,
//                          height,
//                          refreshRate);
// }

// void ExitFullScreen();

} // namespace glfw