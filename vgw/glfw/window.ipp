#include "window.hpp"

namespace glfw {

inline window::window(int windowWidth,
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

inline void window::Create(int windowWidth,
                           int windowHeight,
                           const char* windowTitle,
                           const char* windowClassName)
{
    this->Create_(windowWidth,
                  windowHeight,
                  windowTitle,
                  windowClassName,
                  nullptr,
                  nullptr);
}

inline void window::Create(int windowWidth,
                           int windowHeight,
                           const char* windowTitle,
                           monitor& fullScreenMonitor)
{
    this->Create(windowWidth, windowHeight, windowTitle, "", fullScreenMonitor);
}

inline void window::Create(int windowWidth,
                           int windowHeight,
                           const char* windowTitle,
                           window& contextShareWindow)
{
    this->Create(
        windowWidth, windowHeight, windowTitle, "", contextShareWindow);
}

inline void window::Create(int windowWidth,
                           int windowHeight,
                           const char* windowTitle,
                           monitor& fullScreenMonitor,
                           window& contextShareWindow)
{
    this->Create(windowWidth,
                 windowHeight,
                 windowTitle,
                 "",
                 fullScreenMonitor,
                 contextShareWindow);
}

inline void window::ExitFullScreen(coordinate<int> position, size<int> size)
{
    ExitFullScreen(position.x, position.y, size.width, size.height);
}

template<typename type>
void ScreenCoordinateToPixel(monitor& associatedMonitor,
                             window& referenceWindow,
                             type screenCoordinateX,
                             type screenCoordinateY,
                             type& pixelX,
                             type& pixelY)
{
    if ((associatedMonitor.AssertInitialization() == ASSERT_FAILURE) ||
        (referenceWindow.AssertCreation() == ASSERT_FAILURE)) {
        return;
    }

    size<int> windowSizeInScreenCoordinates;
    glfwGetWindowSize(referenceWindow.Id(),
                      &windowSizeInScreenCoordinates.width,
                      &windowSizeInScreenCoordinates.height);
    size<int> windowSizeInPixels;
    glfwGetFramebufferSize(referenceWindow.Id(),
                           &windowSizeInPixels.width,
                           &windowSizeInPixels.height);

    coordinate<double> pixelOverScreenCoordinateRatio;
    pixelOverScreenCoordinateRatio.x =
        double(windowSizeInPixels.width) /
        double(windowSizeInScreenCoordinates.width);
    pixelOverScreenCoordinateRatio.y =
        double(windowSizeInPixels.height) /
        double(windowSizeInScreenCoordinates.height);

    pixelX = type(double(pixelX) * pixelOverScreenCoordinateRatio.x);
    pixelY = type(double(pixelY) * pixelOverScreenCoordinateRatio.y);
}

template<typename type>
coordinate<type> ScreenCoordinateToPixel(monitor& associatedMonitor,
                                         window& referenceWindow,
                                         coordinate<type> screenCoordinate)
{
    coordinate<type> pixel;
    ScreenCoordinateToPixel(associatedMonitor,
                            referenceWindow,
                            screenCoordinate.x,
                            screenCoordinate.y,
                            pixel.x,
                            pixel.y);
    return pixel;
}

template<typename type>
size<type> ScreenCoordinateToPixel(monitor& associatedMonitor,
                                   window& referenceWindow,
                                   size<type> screenCoordinate)
{
    size<type> pixel;
    ScreenCoordinateToPixel(associatedMonitor,
                            referenceWindow,
                            screenCoordinate.width,
                            screenCoordinate.height,
                            pixel.width,
                            pixel.height);
    return pixel;
}

template<typename type>
void PixelToScreenCoordinate(monitor& associatedMonitor,
                             window& referenceWindow,
                             type pixelX,
                             type pixelY,
                             type& screenCoordinateX,
                             type& screenCoordinateY)
{
    if ((associatedMonitor.AssertInitialization() == ASSERT_FAILURE) ||
        (referenceWindow.AssertCreation() == ASSERT_FAILURE)) {
        return;
    }

    size<int> windowSizeInScreenCoordinates;
    glfwGetWindowSize(referenceWindow.Id(),
                      &windowSizeInScreenCoordinates.width,
                      &windowSizeInScreenCoordinates.height);
    size<int> windowSizeInPixels;
    glfwGetFramebufferSize(referenceWindow.Id(),
                           &windowSizeInPixels.width,
                           &windowSizeInPixels.height);

    coordinate<double> screenCoordinateOverPixelRatio;
    screenCoordinateOverPixelRatio.x =
        double(windowSizeInScreenCoordinates.width) /
        double(windowSizeInPixels.width);
    screenCoordinateOverPixelRatio.y =
        double(windowSizeInScreenCoordinates.height) /
        double(windowSizeInPixels.height);

    pixelX = type(double(pixelX) * screenCoordinateOverPixelRatio.x);
    pixelY = type(double(pixelY) * screenCoordinateOverPixelRatio.y);
}

template<typename type>
coordinate<type> PixelToScreenCoordinate(monitor& associatedMonitor,
                                         window& referenceWindow,
                                         coordinate<type> pixel)
{
    coordinate<type> screenCoordinate;
    PixelToScreenCoordinate(associatedMonitor,
                            referenceWindow,
                            pixel.x,
                            pixel.y,
                            screenCoordinate.x,
                            screenCoordinate.y);
    return screenCoordinate;
}

template<typename type>
size<type> PixelToScreenCoordinate(monitor& associatedMonitor,
                                   window& referenceWindow,
                                   size<type> pixel)
{
    size<type> screenCoordinate;
    PixelToScreenCoordinate(associatedMonitor,
                            referenceWindow,
                            pixel.width,
                            pixel.height,
                            screenCoordinate.width,
                            screenCoordinate.height);
    return screenCoordinate;
}

} // namespace glfw