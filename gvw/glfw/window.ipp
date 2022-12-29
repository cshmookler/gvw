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

inline void window::SetMinimumAndMaximumSize(size<int> minimumSize,
                                             size<int> maximumSize)
{
    this->SetMinimumAndMaximumSize(minimumSize.width,
                                   minimumSize.height,
                                   maximumSize.width,
                                   maximumSize.height);
}

inline void window::SetMinimumSize(int minimumWidth, int minimumHeight)
{
    this->SetMinimumAndMaximumSize(
        minimumWidth, minimumHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);
}

inline void window::SetMinimumSize(size<int> minimumSize)
{
    this->SetMinimumAndMaximumSize(
        minimumSize.width, minimumSize.height, GLFW_DONT_CARE, GLFW_DONT_CARE);
}

inline void window::SetMaximumSize(int maximumWidth, int maximumHeight)
{
    this->SetMinimumAndMaximumSize(
        GLFW_DONT_CARE, GLFW_DONT_CARE, maximumWidth, maximumHeight);
}

inline void window::SetMaximumSize(size<int> maximumSize)
{
    this->SetMinimumAndMaximumSize(
        GLFW_DONT_CARE, GLFW_DONT_CARE, maximumSize.width, maximumSize.height);
}

inline void window::ExitFullScreen(coordinate<int> position, size<int> size)
{
    ExitFullScreen(position.x, position.y, size.width, size.height);
}

inline bool window::IsCursorHovering()
{
    return this->GetWindowAttribute_(GLFW_HOVERED);
}

inline bool window::IsResizable()
{
    return this->GetWindowAttribute_(GLFW_RESIZABLE);
}

inline void window::Decorate()
{
    this->SetWindowAttribute_(GLFW_DECORATED, GLFW_TRUE);
}

inline void window::Undecorate()
{
    this->SetWindowAttribute_(GLFW_DECORATED, GLFW_FALSE);
}

inline bool window::IsDecorated()
{
    return this->GetWindowAttribute_(GLFW_DECORATED);
}

inline void window::MinimizeOnFocusLoss()
{
    this->SetWindowAttribute_(GLFW_AUTO_ICONIFY, GLFW_TRUE);
}

inline void window::DontMinimizeOnFocusLoss()
{
    this->SetWindowAttribute_(GLFW_AUTO_ICONIFY, GLFW_FALSE);
}

inline bool window::IsMinimizedOnFocusLoss()
{
    return this->GetWindowAttribute_(GLFW_AUTO_ICONIFY);
}

inline void window::AlwaysOnTop()
{
    this->SetWindowAttribute_(GLFW_FLOATING, GLFW_TRUE);
}

inline void window::NotAlwaysOnTop()
{
    this->SetWindowAttribute_(GLFW_FLOATING, GLFW_FALSE);
}

inline bool window::IsAlwaysOnTop()
{
    return this->GetWindowAttribute_(GLFW_FLOATING);
}

inline bool window::IsMinimized()
{
    return this->GetWindowAttribute_(GLFW_ICONIFIED);
}

inline bool window::IsMaximized()
{
    return this->GetWindowAttribute_(GLFW_MAXIMIZED);
}

inline bool window::IsVisible()
{
    return this->GetWindowAttribute_(GLFW_VISIBLE);
}

bool window::IsFocused()
{
    return this->GetWindowAttribute_(GLFW_FOCUSED);
}

inline void window::FocusOnShow()
{
    this->SetWindowAttribute_(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
}

inline void window::DontFocusOnShow()
{
    this->SetWindowAttribute_(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
}

inline bool window::IsFocusedOnShow()
{
    return this->GetWindowAttribute_(GLFW_FOCUS_ON_SHOW);
}

inline bool window::IsTransparent()
{
    return this->GetWindowAttribute_(GLFW_TRANSPARENT_FRAMEBUFFER);
}

inline void SetSwapInterval(int interval)
{
    glfwSwapInterval(interval);
}

template<typename type>
void ScreenCoordinateToPixel(window& referenceWindow,
                             type screenCoordinateX,
                             type screenCoordinateY,
                             type& pixelX,
                             type& pixelY)
{
    if (referenceWindow.AssertCreation() == ASSERT_FAILURE) {
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
coordinate<type> ScreenCoordinateToPixel(window& referenceWindow,
                                         coordinate<type> screenCoordinate)
{
    coordinate<type> pixel;
    ScreenCoordinateToPixel(referenceWindow,
                            screenCoordinate.x,
                            screenCoordinate.y,
                            pixel.x,
                            pixel.y);
    return pixel;
}

template<typename type>
size<type> ScreenCoordinateToPixel(window& referenceWindow,
                                   size<type> screenCoordinate)
{
    size<type> pixel;
    ScreenCoordinateToPixel(referenceWindow,
                            screenCoordinate.width,
                            screenCoordinate.height,
                            pixel.width,
                            pixel.height);
    return pixel;
}

template<typename type>
void PixelToScreenCoordinate(window& referenceWindow,
                             type pixelX,
                             type pixelY,
                             type& screenCoordinateX,
                             type& screenCoordinateY)
{
    if (referenceWindow.AssertCreation() == ASSERT_FAILURE) {
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
coordinate<type> PixelToScreenCoordinate(window& referenceWindow,
                                         coordinate<type> pixel)
{
    coordinate<type> screenCoordinate;
    PixelToScreenCoordinate(referenceWindow,
                            pixel.x,
                            pixel.y,
                            screenCoordinate.x,
                            screenCoordinate.y);
    return screenCoordinate;
}

template<typename type>
size<type> PixelToScreenCoordinate(window& referenceWindow, size<type> pixel)
{
    size<type> screenCoordinate;
    PixelToScreenCoordinate(referenceWindow,
                            pixel.width,
                            pixel.height,
                            screenCoordinate.width,
                            screenCoordinate.height);
    return screenCoordinate;
}

} // namespace glfw