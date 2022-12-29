#pragma once

// Standard includes
#include <vector>

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Local includes
#include "glfw.hpp"
#include "monitor.hpp"

namespace glfw {

inline void PollEvents()
{
    glfwPollEvents();
}

class window
{
    GLFWwindow* windowId_ = WINDOW_ID_NULL;
    coordinate<int> defaultPositionInScreenCoordinates_;
    size<int> defaultSizeInScreenCoordinates_;
    int displayMode_ = NOT_CREATED;

    // Create the window
    void Create_(int windowWidth,
                 int windowHeight,
                 const char* windowTitle,
                 const char* windowClassName,
                 GLFWmonitor* fullScreenMonitor,
                 GLFWwindow* contextShareWindow);

  public:
    // Constructors
    window();
    inline window(int windowWidth,
                  int windowHeight,
                  const char* windowTitle,
                  const char* windowClassName,
                  GLFWmonitor* fullScreenMonitor,
                  GLFWwindow* contextShareWindow);

    // Immediately closes the window
    void Destroy();

    // Destructor (calls the 'Destroy' function)
    ~window();

    // Delete the copy constructor and copy assignment operators
    // It should not be possible to copy this object
    window(const window&) = delete;
    window& operator=(const window&) = delete;

    // Checks if the window has been created.
    // If it has not, an error is sent to the GLFW error callback.
    bool AssertCreation(
        int errorCode = ERROR_WINDOW_NOT_CREATED_BEFORE_OPERATION,
        const char* errorMessage =
            ERROR_MESSAGE_WINDOW_NOT_CREATED_BEFORE_OPERATION);

    inline void Create(int windowWidth,
                       int windowHeight,
                       const char* windowTitle,
                       const char* windowClassName = "");

    void Create(int windowWidth,
                int windowHeight,
                const char* windowTitle,
                const char* windowClassName,
                monitor& fullScreenMonitor);
    void Create(int windowWidth,
                int windowHeight,
                const char* windowTitle,
                const char* windowClassName,
                window& contextShareWindow);
    void Create(int windowWidth,
                int windowHeight,
                const char* windowTitle,
                const char* windowClassName,
                monitor& fullScreenMonitor,
                window& contextShareWindow);
    inline void Create(int windowWidth,
                       int windowHeight,
                       const char* windowTitle,
                       monitor& fullScreenMonitor);
    inline void Create(int windowWidth,
                       int windowHeight,
                       const char* windowTitle,
                       window& contextShareWindow);
    inline void Create(int windowWidth,
                       int windowHeight,
                       const char* windowTitle,
                       monitor& fullScreenMonitor,
                       window& contextShareWindow);

    GLFWwindow* Id();

    size<int> GetSizeInScreenCoordinates();
    size<int> GetSize();
    void SetSize(size<int> windowSize);
    void SetSize(int width, int height);

    coordinate<float> GetContentScale();

    coordinate<int> GetPositionInScreenCoordinates();
    coordinate<int> GetPosition();
    void SetPosition(coordinate<int> windowPosition);
    void SetPosition(int xPos, int yPos);

    void FullScreen(monitor& fullScreenMonitor);
    void ExitFullScreen();
    void ExitFullScreen(int xPos, int yPos, int width, int height);
    inline void ExitFullScreen(coordinate<int> position, size<int> size);

    void SetTitle(const char* title);

    void SetIcon(const char* iconImagePath);
    void SetIcon(std::vector<const char*> candidateIconImagePaths);

    bool ShouldClose(); // Returns the state of the 'close' flag
    void Close();       // Set the 'close' flag to true
    void CancelClose(); // Set the 'close' flag to false

    void SwapBuffers();
};

template<typename type>
void ScreenCoordinateToPixel(monitor& associatedMonitor,
                             window& referenceWindow,
                             type screenCoordinateX,
                             type screenCoordinateY,
                             type& pixelX,
                             type& pixelY);
template<typename type>
coordinate<type> ScreenCoordinateToPixel(monitor& associatedMonitor,
                                         window& referenceWindow,
                                         coordinate<type> screenCoordinate);
template<typename type>
size<type> ScreenCoordinateToPixel(monitor& associatedMonitor,
                                   window& referenceWindow,
                                   size<type> screenCoordinate);

template<typename type>
void PixelToScreenCoordinate(monitor& associatedMonitor,
                             window& referenceWindow,
                             type pixelX,
                             type pixelY,
                             type& screenCoordinateX,
                             type& screenCoordinateY);
template<typename type>
coordinate<type> PixelToScreenCoordinate(monitor& associatedMonitor,
                                         window& referenceWindow,
                                         coordinate<type> pixel);
template<typename type>
size<type> PixelToScreenCoordinate(monitor& associatedMonitor,
                                   window& referenceWindow,
                                   size<type> pixel);

} // namespace glfw