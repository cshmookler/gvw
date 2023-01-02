#include "window.ipp"

// Standard includes
#include <array>
#include <iostream>

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// Local includes
#include "glfw.hpp"
#include "glfw.ipp"
#include "glfw_constants.hpp"

namespace glfw {

void window::Create_(int windowWidth,
                     int windowHeight,
                     const char* windowTitle,
                     const char* windowClassName,
                     GLFWmonitor* fullScreenMonitor,
                     GLFWwindow* contextShareWindow)
{
    // Check if the window has already been created
    if (this->windowId_ != WINDOW_ID_NULL) {
        ERROR_CALLBACK(ERROR_WINDOW_ALREADY_CREATED,
                       ERROR_MESSAGE_WINDOW_ALREADY_CREATED);
        return;
    }

    // Check if the window dimensions are valid
    if (windowWidth < MIN_WINDOW_WIDTH || windowHeight < MIN_WINDOW_HEIGHT) {
        ERROR_CALLBACK(ERROR_INVALID_DIMENSIONS,
                       ERROR_MESSAGE_INVALID_DIMENSIONS);
        return;
    }

    if (fullScreenMonitor == nullptr) {
        this->displayMode_ = WINDOWED;
    } else {
        this->displayMode_ = FULL_SCREEN;
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
    glfwWindowHint(GLFW_SCALE_TO_MONITOR,
                   GLFW_FALSE); // set to GLFW_TRUE by default?

    /* ----- Changing these window hints is NOT recommended ----- */

    // Framebuffer related hints
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
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
    this->defaultPositionInScreenCoordinates_ =
        this->GetPositionInScreenCoordinates();
    this->defaultSizeInScreenCoordinates_ = this->GetSizeInScreenCoordinates();
}

int window::GetWindowAttribute_(int attribute)
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return GLFW_DONT_CARE;
    }
    return glfwGetWindowAttrib(this->windowId_, attribute);
}

void window::SetWindowAttribute_(int attribute, int value)
{
    if (this->AssertCreation() == ASSERT_SUCCESS) {
        return glfwSetWindowAttrib(this->windowId_, attribute, value);
    }
}

window::window() = default;

void window::Destroy()
{
    if (this->windowId_ != WINDOW_ID_NULL) {
        glfwDestroyWindow(this->windowId_);
        this->windowId_ = WINDOW_ID_NULL;
        this->displayMode_ = NOT_CREATED;
    }
}

window::~window()
{
    this->Destroy();
}

bool window::AssertCreation(int errorCode, const char* errorMessage)
{
    if (this->windowId_ == WINDOW_ID_NULL) {
        ERROR_CALLBACK(errorCode, errorMessage);
        return ASSERT_FAILURE;
    }

    return ASSERT_SUCCESS;
}

void window::Create(int windowWidth,
                    int windowHeight,
                    const char* windowTitle,
                    const char* windowClassName,
                    monitor& fullScreenMonitor)
{
    if (fullScreenMonitor.AssertInitialization() == ASSERT_FAILURE) {
        return;
    }

    this->Create_(windowWidth,
                  windowHeight,
                  windowTitle,
                  windowClassName,
                  fullScreenMonitor.Id(),
                  nullptr);
}

void window::Create(int windowWidth,
                    int windowHeight,
                    const char* windowTitle,
                    const char* windowClassName,
                    window& contextShareWindow)
{
    if (contextShareWindow.AssertCreation(
            ERROR_PARENT_WINDOW_NOT_CREATED,
            ERROR_MESSAGE_PARENT_WINDOW_NOT_CREATED) == ASSERT_FAILURE) {
        return;
    }

    this->Create_(windowWidth,
                  windowHeight,
                  windowTitle,
                  windowClassName,
                  nullptr,
                  contextShareWindow.windowId_);
}

void window::Create(int windowWidth,
                    int windowHeight,
                    const char* windowTitle,
                    const char* windowClassName,
                    monitor& fullScreenMonitor,
                    window& contextShareWindow)
{
    if ((fullScreenMonitor.AssertInitialization() == ASSERT_FAILURE) ||
        (contextShareWindow.AssertCreation(
             ERROR_PARENT_WINDOW_NOT_CREATED,
             ERROR_MESSAGE_PARENT_WINDOW_NOT_CREATED) == ASSERT_FAILURE)) {
        return;
    }

    this->Create_(windowWidth,
                  windowHeight,
                  windowTitle,
                  windowClassName,
                  fullScreenMonitor.Id(),
                  contextShareWindow.windowId_);
}

GLFWwindow* window::Id()
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return nullptr;
    }
    return this->windowId_;
}

bool window::ShouldClose()
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return GLFW_FALSE;
    }
    return bool(glfwWindowShouldClose(this->windowId_));
}

void window::Close()
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    glfwSetWindowShouldClose(this->windowId_, GLFW_TRUE);
}

void window::CancelClose()
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    glfwSetWindowShouldClose(this->windowId_, GLFW_FALSE);
}

size<int> window::GetSizeInScreenCoordinates()
{
    size<int> windowSize = { -1, -1 };
    if (this->AssertCreation() == ASSERT_SUCCESS) {
        glfwGetWindowSize(
            this->windowId_, &windowSize.width, &windowSize.height);
    }
    return windowSize;
}

size<int> window::GetSize()
{
    size<int> windowSize = { -1, -1 };
    if (this->AssertCreation() == ASSERT_SUCCESS) {
        glfwGetFramebufferSize(
            this->windowId_, &windowSize.width, &windowSize.height);
    }
    return windowSize;
}

void window::SetSize(size<int> windowSize)
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    windowSize = PixelToScreenCoordinate(*this, windowSize);
    glfwSetWindowSize(this->windowId_, windowSize.width, windowSize.height);
}

void window::SetSize(int width, int height)
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    PixelToScreenCoordinate(*this, width, height, width, height);
    glfwSetWindowSize(this->windowId_, width, height);
}

coordinate<int> window::GetPositionInScreenCoordinates()
{
    coordinate<int> windowPosition = { -1, -1 };
    if (this->AssertCreation() == ASSERT_SUCCESS) {
        glfwGetWindowPos(this->windowId_, &windowPosition.x, &windowPosition.y);
    }
    return windowPosition;
}

coordinate<int> window::GetPosition()
{
    coordinate<int> windowPosition = { -1, -1 };
    if (this->AssertCreation() == ASSERT_SUCCESS) {
        glfwGetWindowPos(this->windowId_, &windowPosition.x, &windowPosition.y);
        windowPosition = ScreenCoordinateToPixel(*this, windowPosition);
    }
    return windowPosition;
}

void window::SetPosition(coordinate<int> windowPosition)
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    windowPosition = PixelToScreenCoordinate(*this, windowPosition);
    glfwSetWindowPos(this->windowId_, windowPosition.x, windowPosition.y);
}

void window::SetPosition(int xPos, int yPos)
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    PixelToScreenCoordinate(*this, xPos, yPos, xPos, yPos);
    glfwSetWindowPos(this->windowId_, xPos, yPos);
}

coordinate<float> window::GetContentScale()
{
    coordinate<float> contentScale = { -1.0f, -1.0f };
    if (this->AssertCreation() == ASSERT_SUCCESS) {
        glfwGetWindowContentScale(
            this->windowId_, &contentScale.x, &contentScale.y);
    }
    return contentScale;
}

void window::SetMinimumAndMaximumSize(int minimumWidth,
                                      int minimumHeight,
                                      int maximumWidth,
                                      int maximumHeight)
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    PixelToScreenCoordinate(
        *this, minimumWidth, minimumHeight, minimumWidth, minimumHeight);
    PixelToScreenCoordinate(
        *this, maximumWidth, maximumHeight, maximumWidth, maximumHeight);
    glfwSetWindowSizeLimits(this->windowId_,
                            minimumWidth,
                            minimumHeight,
                            maximumWidth,
                            maximumHeight);
}

void window::SetAspectRatio(int numerator, int denominator)
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    glfwSetWindowAspectRatio(this->windowId_, numerator, denominator);
}

void window::FullScreen(monitor& fullScreenMonitor)
{
    if ((this->AssertCreation() == ASSERT_FAILURE) ||
        (fullScreenMonitor.AssertInitialization() == ASSERT_FAILURE) ||
        (this->displayMode_ == FULL_SCREEN)) {
        return;
    }

    this->defaultPositionInScreenCoordinates_ =
        this->GetPositionInScreenCoordinates();
    this->defaultSizeInScreenCoordinates_ = this->GetSizeInScreenCoordinates();
    this->displayMode_ = FULL_SCREEN;
    const GLFWvidmode* videoMode =
        fullScreenMonitor.VideoModeInScreenCoordinates();
    glfwSetWindowMonitor(this->windowId_,
                         fullScreenMonitor.Id(),
                         GLFW_FALSE,
                         GLFW_FALSE,
                         videoMode->width,
                         videoMode->height,
                         videoMode->refreshRate);
}

void window::ExitFullScreen()
{
    if ((this->AssertCreation() == ASSERT_FAILURE) ||
        (this->displayMode_ != FULL_SCREEN)) {
        return;
    }

    this->displayMode_ = WINDOWED;
    glfwSetWindowMonitor(this->windowId_,
                         nullptr,
                         this->defaultPositionInScreenCoordinates_.x,
                         this->defaultPositionInScreenCoordinates_.y,
                         this->defaultSizeInScreenCoordinates_.width,
                         this->defaultSizeInScreenCoordinates_.height,
                         GLFW_DONT_CARE);
}

void window::ExitFullScreen(int xPos, int yPos, int width, int height)
{
    if ((this->AssertCreation() == ASSERT_FAILURE) ||
        (this->displayMode_ != FULL_SCREEN)) {
        return;
    }
    PixelToScreenCoordinate(*this, xPos, yPos, xPos, yPos);
    PixelToScreenCoordinate(*this, width, height, width, height);

    this->displayMode_ = WINDOWED;
    glfwSetWindowMonitor(
        this->windowId_, nullptr, xPos, yPos, width, height, GLFW_DONT_CARE);
}

void window::SetTitle(const char* title)
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    glfwSetWindowTitle(this->windowId_, title);
}

void window::SetIcon(const char* iconImagePath)
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    std::array<GLFWimage, 1> icon;
    icon[0].pixels =
        stbi_load(iconImagePath, &icon[0].width, &icon[0].height, nullptr, 4);
    if (icon[0].pixels == NULL) {
        ERROR_CALLBACK(ERROR_ICON_FAILED_TO_LOAD,
                       ERROR_MESSAGE_ICON_FAILED_TO_LOAD);
        return;
    }
    glfwSetWindowIcon(this->windowId_, icon.size(), icon.data());
    stbi_image_free(icon[0].pixels);
}

void window::SetIcon(std::vector<const char*> candidateIconImagePaths)
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    std::vector<GLFWimage> icons(candidateIconImagePaths.size());
    for (size_t iconIndex = 0; iconIndex < icons.size(); iconIndex++) {
        icons[iconIndex].pixels = stbi_load(candidateIconImagePaths[iconIndex],
                                            &icons[iconIndex].width,
                                            &icons[iconIndex].height,
                                            nullptr,
                                            4);
        if (icons[iconIndex].pixels == NULL) {
            ERROR_CALLBACK(ERROR_ICON_FAILED_TO_LOAD,
                           ERROR_MESSAGE_ICON_FAILED_TO_LOAD);
            return;
        }
    }
    glfwSetWindowIcon(this->windowId_, icons.size(), icons.data());
    for (size_t iconIndex = 0; iconIndex < icons.size(); iconIndex++) {
        stbi_image_free(icons[iconIndex].pixels);
    }
}

void window::Minimize()
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    glfwIconifyWindow(this->windowId_);
    this->displayMode_ = ICONIFIED;
}

void window::Maximize()
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    glfwMaximizeWindow(this->windowId_);
    this->displayMode_ = MAXIMIZED;
}

void window::Restore()
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    glfwRestoreWindow(this->windowId_);
    if (this->displayMode_ == FULL_SCREEN) {
        this->ExitFullScreen();
    }
    this->displayMode_ = WINDOWED;
}

void window::Hide()
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    glfwHideWindow(this->windowId_);
}

void window::Show()
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    glfwShowWindow(this->windowId_);
}

void window::Focus()
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    glfwFocusWindow(this->windowId_);
}

void window::RequestFocus()
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    glfwRequestWindowAttention(this->windowId_);
}

void window::SetOpacity(float opacity)
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    glfwSetWindowOpacity(this->windowId_, opacity);
}

void window::SwapBuffers()
{
    if (this->AssertCreation() == ASSERT_FAILURE) {
        return;
    }
    glfwSwapBuffers(this->windowId_);
}

} // namespace glfw