#include "window.tpp"

// Standard includes
#include <array>

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Local includes
#include "../common/file.hpp"
#include "../common/global.hpp"
#include "../common/image.hpp"
#include "../common/types.tpp"
#include "init.tpp"
#include "input.hpp"
#include "monitor.hpp"

namespace gvw {

void window::Create_(int windowWidth,
                     int windowHeight,
                     const char* windowTitle,
                     const char* windowClassName,
                     GLFWmonitor* fullScreenMonitor,
                     GLFWwindow* contextShareWindow)
{
    // Check if the window has already been created
    if (this->windowId_ != internal::WINDOW_ID_NULL) {
        internal::ERROR_CALLBACK(
            internal::ERROR_WINDOW_ALREADY_CREATED,
            internal::ERROR_MESSAGE_WINDOW_ALREADY_CREATED);
        return;
    }

    // Check if the window dimensions are valid
    if (windowWidth < internal::MIN_WINDOW_WIDTH ||
        windowHeight < internal::MIN_WINDOW_HEIGHT) {
        internal::ERROR_CALLBACK(internal::ERROR_INVALID_DIMENSIONS,
                                 internal::ERROR_MESSAGE_INVALID_DIMENSIONS);
        return;
    }

    if (fullScreenMonitor == nullptr) {
        this->displayMode_ = internal::WINDOWED;
    } else {
        this->displayMode_ = internal::FULL_SCREEN;
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

bool window::AssertCreation_(int errorCode, const char* errorMessage)
{
    if (this->windowId_ == internal::WINDOW_ID_NULL) {
        internal::ERROR_CALLBACK(errorCode, errorMessage);
        return internal::ASSERT_FAILURE;
    }

    return internal::ASSERT_SUCCESS;
}

int window::GetWindowAttribute_(int attribute)
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return GLFW_DONT_CARE;
    }
    return glfwGetWindowAttrib(this->windowId_, attribute);
}

void window::SetWindowAttribute_(int attribute, int value)
{
    if (this->AssertCreation_() == internal::ASSERT_SUCCESS) {
        return glfwSetWindowAttrib(this->windowId_, attribute, value);
    }
}

void window::RegisterWindowWithInputCallbacks_()
{
    size_t windowIndex;
    if (internal::FindWindowInputIndex(this->windowId_,
                                       internal::WINDOWS_RECIEVING_INPUT,
                                       windowIndex) == GLFW_FALSE) {
        internal::WINDOWS_RECIEVING_INPUT.push_back(this);
    }
}

void window::UnregisterWindowWithInputCallbacks_()
{
    size_t windowIndex;
    if (internal::FindWindowInputIndex(this->windowId_,
                                       internal::WINDOWS_RECIEVING_INPUT,
                                       windowIndex) == GLFW_TRUE) {
        internal::WINDOWS_RECIEVING_INPUT.erase(
            internal::WINDOWS_RECIEVING_INPUT.begin() + windowIndex);
    }
}

window::window() = default;

window::window(int windowWidth,
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

void window::Destroy()
{
    if (this->windowId_ != internal::WINDOW_ID_NULL) {
        // TODO: destroy surface
        this->UnregisterWindowWithInputCallbacks_();
        glfwDestroyWindow(this->windowId_);
        this->windowId_ = internal::WINDOW_ID_NULL;
        this->displayMode_ = internal::NOT_CREATED;
    }
}

window::~window()
{
    this->Destroy();
}

void window::Create(int windowWidth,
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

void window::Create(int windowWidth,
                    int windowHeight,
                    const char* windowTitle,
                    const char* windowClassName,
                    monitor& fullScreenMonitor)
{
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
    this->Create_(windowWidth,
                  windowHeight,
                  windowTitle,
                  windowClassName,
                  nullptr,
                  contextShareWindow.Id());
}

void window::Create(int windowWidth,
                    int windowHeight,
                    const char* windowTitle,
                    const char* windowClassName,
                    monitor& fullScreenMonitor,
                    window& contextShareWindow)
{
    this->Create_(windowWidth,
                  windowHeight,
                  windowTitle,
                  windowClassName,
                  fullScreenMonitor.Id(),
                  contextShareWindow.Id());
}

void window::Create(int windowWidth,
                    int windowHeight,
                    const char* windowTitle,
                    monitor& fullScreenMonitor)
{
    this->Create(windowWidth, windowHeight, windowTitle, "", fullScreenMonitor);
}

void window::Create(int windowWidth,
                    int windowHeight,
                    const char* windowTitle,
                    window& contextShareWindow)
{
    this->Create(
        windowWidth, windowHeight, windowTitle, "", contextShareWindow);
}

void window::Create(int windowWidth,
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

GLFWwindow* window::Id()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return nullptr;
    }
    return this->windowId_;
}

void window::SetupKeyInputBuffer()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    this->keyEvents.resize(internal::INPUT_BUFFER_INITIAL_SIZE);
    storedKeyEvents = 0;
    glfwSetKeyCallback(this->windowId_, internal::KeyCallback);
    this->RegisterWindowWithInputCallbacks_();
}

void window::SetupCharacterInputBuffer()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    this->characterEvents.resize(internal::INPUT_BUFFER_INITIAL_SIZE);
    storedCharacterEvents = 0;
    glfwSetCharCallback(this->windowId_, internal::CharacterCallback);
    this->RegisterWindowWithInputCallbacks_();
}

void window::SetupCursorPositionInputBuffer()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    this->cursorPositionEvents.resize(internal::INPUT_BUFFER_INITIAL_SIZE);
    storedCursorPositionEvents = 0;
    glfwSetCursorPosCallback(this->windowId_, internal::CursorPositionCallback);
    this->RegisterWindowWithInputCallbacks_();
}

void window::SetupCursorEnterInputBuffer()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    this->cursorEnterEvents.resize(internal::INPUT_BUFFER_INITIAL_SIZE);
    storedCursorEnterEvents = 0;
    glfwSetCursorEnterCallback(this->windowId_, internal::CursorEnterCallback);
    this->RegisterWindowWithInputCallbacks_();
}

void window::SetupMouseButtonInputBuffer()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    this->mouseButtonEvents.resize(internal::INPUT_BUFFER_INITIAL_SIZE);
    storedMouseButtonEvents = 0;
    glfwSetMouseButtonCallback(this->windowId_, internal::MouseButtonCallback);
    this->RegisterWindowWithInputCallbacks_();
}

void window::SetupScrollInputBuffer()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    this->scrollEvents.resize(internal::INPUT_BUFFER_INITIAL_SIZE);
    storedScrollEvents = 0;
    glfwSetScrollCallback(this->windowId_, internal::ScrollCallback);
    this->RegisterWindowWithInputCallbacks_();
}

void window::SetupFileDropInputBuffer()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    this->fileDropEvents.resize(internal::INPUT_BUFFER_INITIAL_SIZE);
    storedFileDropEvents = 0;
    glfwSetDropCallback(this->windowId_, internal::FileDropCallback);
    this->RegisterWindowWithInputCallbacks_();
}

void window::ClearInputBuffers()
{
    this->keyEvents.resize(internal::INPUT_BUFFER_INITIAL_SIZE);
    this->characterEvents.resize(internal::INPUT_BUFFER_INITIAL_SIZE);
    this->cursorPositionEvents.resize(internal::INPUT_BUFFER_INITIAL_SIZE);
    this->cursorEnterEvents.resize(internal::INPUT_BUFFER_INITIAL_SIZE);
    this->mouseButtonEvents.resize(internal::INPUT_BUFFER_INITIAL_SIZE);
    this->scrollEvents.resize(internal::INPUT_BUFFER_INITIAL_SIZE);
    this->fileDropEvents.resize(internal::INPUT_BUFFER_INITIAL_SIZE);
    this->storedKeyEvents = 0;
    this->storedCharacterEvents = 0;
    this->storedCursorPositionEvents = 0;
    this->storedCursorEnterEvents = 0;
    this->storedMouseButtonEvents = 0;
    this->storedScrollEvents = 0;
    this->storedFileDropEvents = 0;
}

bool window::ShouldClose()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return GLFW_FALSE;
    }
    return bool(glfwWindowShouldClose(this->windowId_));
}

void window::Close()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    glfwSetWindowShouldClose(this->windowId_, GLFW_TRUE);
}

void window::CancelClose()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    glfwSetWindowShouldClose(this->windowId_, GLFW_FALSE);
}

size<int> window::GetSizeInScreenCoordinates()
{
    size<int> windowSize = { -1, -1 };
    if (this->AssertCreation_() == internal::ASSERT_SUCCESS) {
        glfwGetWindowSize(
            this->windowId_, &windowSize.width, &windowSize.height);
    }
    return windowSize;
}

size<int> window::GetSize()
{
    size<int> windowSize = { -1, -1 };
    if (this->AssertCreation_() == internal::ASSERT_SUCCESS) {
        glfwGetFramebufferSize(
            this->windowId_, &windowSize.width, &windowSize.height);
    }
    return windowSize;
}

void window::SetSize(size<int> windowSize)
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    windowSize = this->PixelToScreenCoordinate(windowSize);
    glfwSetWindowSize(this->windowId_, windowSize.width, windowSize.height);
}

void window::SetSize(int width, int height)
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    this->PixelToScreenCoordinate(width, height, width, height);
    glfwSetWindowSize(this->windowId_, width, height);
}

coordinate<int> window::GetPositionInScreenCoordinates()
{
    coordinate<int> windowPosition = { -1, -1 };
    if (this->AssertCreation_() == internal::ASSERT_SUCCESS) {
        glfwGetWindowPos(this->windowId_, &windowPosition.x, &windowPosition.y);
    }
    return windowPosition;
}

coordinate<int> window::GetPosition()
{
    coordinate<int> windowPosition = { -1, -1 };
    if (this->AssertCreation_() == internal::ASSERT_SUCCESS) {
        glfwGetWindowPos(this->windowId_, &windowPosition.x, &windowPosition.y);
        windowPosition = this->ScreenCoordinateToPixel(windowPosition);
    }
    return windowPosition;
}

void window::SetPosition(coordinate<int> windowPosition)
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    windowPosition = this->PixelToScreenCoordinate(windowPosition);
    glfwSetWindowPos(this->windowId_, windowPosition.x, windowPosition.y);
}

void window::SetPosition(int xPos, int yPos)
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    this->PixelToScreenCoordinate(xPos, yPos, xPos, yPos);
    glfwSetWindowPos(this->windowId_, xPos, yPos);
}

coordinate<float> window::GetContentScale()
{
    coordinate<float> contentScale = { -1.0F, -1.0F };
    if (this->AssertCreation_() == internal::ASSERT_SUCCESS) {
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
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    this->PixelToScreenCoordinate(
        minimumWidth, minimumHeight, minimumWidth, minimumHeight);
    this->PixelToScreenCoordinate(
        maximumWidth, maximumHeight, maximumWidth, maximumHeight);
    glfwSetWindowSizeLimits(this->windowId_,
                            minimumWidth,
                            minimumHeight,
                            maximumWidth,
                            maximumHeight);
}

void window::SetMinimumAndMaximumSize(size<int> minimumSize,
                                      size<int> maximumSize)
{
    this->SetMinimumAndMaximumSize(minimumSize.width,
                                   minimumSize.height,
                                   maximumSize.width,
                                   maximumSize.height);
}

void window::SetMinimumSize(int minimumWidth, int minimumHeight)
{
    this->SetMinimumAndMaximumSize(
        minimumWidth, minimumHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);
}

void window::SetMinimumSize(size<int> minimumSize)
{
    this->SetMinimumAndMaximumSize(
        minimumSize.width, minimumSize.height, GLFW_DONT_CARE, GLFW_DONT_CARE);
}

void window::SetMaximumSize(int maximumWidth, int maximumHeight)
{
    this->SetMinimumAndMaximumSize(
        GLFW_DONT_CARE, GLFW_DONT_CARE, maximumWidth, maximumHeight);
}

void window::SetMaximumSize(size<int> maximumSize)
{
    this->SetMinimumAndMaximumSize(
        GLFW_DONT_CARE, GLFW_DONT_CARE, maximumSize.width, maximumSize.height);
}

void window::SetAspectRatio(int numerator, int denominator)
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    glfwSetWindowAspectRatio(this->windowId_, numerator, denominator);
}

void window::FullScreen(monitor& fullScreenMonitor)
{
    if ((this->AssertCreation_() == internal::ASSERT_FAILURE) ||
        (this->displayMode_ == internal::FULL_SCREEN)) {
        return;
    }

    this->defaultPositionInScreenCoordinates_ =
        this->GetPositionInScreenCoordinates();
    this->defaultSizeInScreenCoordinates_ = this->GetSizeInScreenCoordinates();
    this->displayMode_ = internal::FULL_SCREEN;
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
    if ((this->AssertCreation_() == internal::ASSERT_FAILURE) ||
        (this->displayMode_ != internal::FULL_SCREEN)) {
        return;
    }

    this->displayMode_ = internal::WINDOWED;
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
    if ((this->AssertCreation_() == internal::ASSERT_FAILURE) ||
        (this->displayMode_ != internal::FULL_SCREEN)) {
        return;
    }
    this->PixelToScreenCoordinate(xPos, yPos, xPos, yPos);
    this->PixelToScreenCoordinate(width, height, width, height);

    this->displayMode_ = internal::WINDOWED;
    glfwSetWindowMonitor(
        this->windowId_, nullptr, xPos, yPos, width, height, GLFW_DONT_CARE);
}

void window::ExitFullScreen(coordinate<int> position, size<int> size)
{
    ExitFullScreen(position.x, position.y, size.width, size.height);
}

void window::SetTitle(const char* title)
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    glfwSetWindowTitle(this->windowId_, title);
}

void window::SetIcon(const char* iconImagePath)
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    image icon = LoadRGBAImage(iconImagePath);
    std::array<GLFWimage, 1> glfwIcon;
    if (icon.pixelData.data() == nullptr) {
        return;
    }
    if (icon.componentsPerPixel != 4) {
        internal::ERROR_CALLBACK(
            internal::ERROR_NOT_ENOUGH_COMPONENTS_PER_PIXEL,
            internal::ERROR_MESSAGE_NOT_ENOUGH_COMPONENTS_PER_PIXEL);
        return;
    }
    glfwIcon.at(0) = icon;
    glfwSetWindowIcon(this->windowId_, glfwIcon.size(), glfwIcon.data());
}

void window::SetIcon(image iconImage)
{
    if (iconImage.pixelData.data() == nullptr) {
        internal::ERROR_CALLBACK(internal::ERROR_ICON_FAILED_TO_LOAD,
                                 internal::ERROR_MESSAGE_ICON_FAILED_TO_LOAD);
        return;
    }
    if (iconImage.componentsPerPixel != 4) {
        internal::ERROR_CALLBACK(
            internal::ERROR_NOT_ENOUGH_COMPONENTS_PER_PIXEL,
            internal::ERROR_MESSAGE_NOT_ENOUGH_COMPONENTS_PER_PIXEL);
        return;
    }
    std::array<GLFWimage, 1> glfwIcon;
    glfwIcon.at(0) = iconImage;
    glfwSetWindowIcon(this->windowId_, glfwIcon.size(), glfwIcon.data());
}

void window::SetCandidateIcons(std::vector<const char*> candidateIconImagePaths)
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    std::vector<GLFWimage> glfwIcons(candidateIconImagePaths.size());
    std::vector<image> icons(candidateIconImagePaths.size());
    size_t iconIndex = 0;
    for (const char* candidateIconImagePath : candidateIconImagePaths) {
        icons.at(iconIndex) = LoadRGBAImage(candidateIconImagePath);
        if (icons.at(iconIndex).pixelData.data() == nullptr) {
            icons.at(iconIndex).pixelData.clear();
            continue;
        }
        if (icons.at(iconIndex).componentsPerPixel != 4) {
            internal::ERROR_CALLBACK(
                internal::ERROR_NOT_ENOUGH_COMPONENTS_PER_PIXEL,
                internal::ERROR_MESSAGE_NOT_ENOUGH_COMPONENTS_PER_PIXEL);
            icons.at(iconIndex).pixelData.clear();
            continue;
        }
        glfwIcons.at(iconIndex) = icons.at(iconIndex);
        iconIndex++;
    }
    glfwIcons.resize(iconIndex);
    glfwSetWindowIcon(this->windowId_, glfwIcons.size(), glfwIcons.data());
}

void window::SetCandidateIcons(std::vector<image> candidateIconImages)
{
    std::vector<GLFWimage> glfwIcons(candidateIconImages.size());
    size_t iconIndex = 0;
    for (image candidateIconImage : candidateIconImages) {
        if (candidateIconImage.pixelData.data() == nullptr) {
            internal::ERROR_CALLBACK(
                internal::ERROR_ICON_FAILED_TO_LOAD,
                internal::ERROR_MESSAGE_ICON_FAILED_TO_LOAD);
            return;
        }
        if (candidateIconImage.componentsPerPixel != 4) {
            internal::ERROR_CALLBACK(
                internal::ERROR_NOT_ENOUGH_COMPONENTS_PER_PIXEL,
                internal::ERROR_MESSAGE_NOT_ENOUGH_COMPONENTS_PER_PIXEL);
            return;
        }
        glfwIcons.at(iconIndex) = candidateIconImage;
        iconIndex++;
    }
    glfwIcons.resize(iconIndex);
    glfwSetWindowIcon(this->windowId_, glfwIcons.size(), glfwIcons.data());
}

bool window::IsCursorHovering()
{
    return static_cast<bool>(this->GetWindowAttribute_(GLFW_HOVERED));
}

bool window::IsResizable()
{
    return static_cast<bool>(this->GetWindowAttribute_(GLFW_RESIZABLE));
}

void window::Decorate()
{
    this->SetWindowAttribute_(GLFW_DECORATED, GLFW_TRUE);
}

void window::Undecorate()
{
    this->SetWindowAttribute_(GLFW_DECORATED, GLFW_FALSE);
}

bool window::IsDecorated()
{
    return static_cast<bool>(this->GetWindowAttribute_(GLFW_DECORATED));
}

void window::MinimizeOnFocusLoss()
{
    this->SetWindowAttribute_(GLFW_AUTO_ICONIFY, GLFW_TRUE);
}

void window::DontMinimizeOnFocusLoss()
{
    this->SetWindowAttribute_(GLFW_AUTO_ICONIFY, GLFW_FALSE);
}

bool window::IsMinimizedOnFocusLoss()
{
    return static_cast<bool>(this->GetWindowAttribute_(GLFW_AUTO_ICONIFY));
}

void window::AlwaysOnTop()
{
    this->SetWindowAttribute_(GLFW_FLOATING, GLFW_TRUE);
}

void window::NotAlwaysOnTop()
{
    this->SetWindowAttribute_(GLFW_FLOATING, GLFW_FALSE);
}

bool window::IsAlwaysOnTop()
{
    return static_cast<bool>(this->GetWindowAttribute_(GLFW_FLOATING));
}

void window::Minimize()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    glfwIconifyWindow(this->windowId_);
    this->displayMode_ = internal::ICONIFIED;
}

void window::Maximize()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    glfwMaximizeWindow(this->windowId_);
    this->displayMode_ = internal::MAXIMIZED;
}

void window::Restore()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    glfwRestoreWindow(this->windowId_);
    if (this->displayMode_ == internal::FULL_SCREEN) {
        this->ExitFullScreen();
    }
    this->displayMode_ = internal::WINDOWED;
}

bool window::IsMinimized()
{
    return static_cast<bool>(this->GetWindowAttribute_(GLFW_ICONIFIED));
}

bool window::IsMaximized()
{
    return static_cast<bool>(this->GetWindowAttribute_(GLFW_MAXIMIZED));
}

void window::Hide()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    glfwHideWindow(this->windowId_);
}

void window::Show()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    glfwShowWindow(this->windowId_);
}

bool window::IsVisible()
{
    return static_cast<bool>(this->GetWindowAttribute_(GLFW_VISIBLE));
}

void window::Focus()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    glfwFocusWindow(this->windowId_);
}

bool window::IsFocused()
{
    return static_cast<bool>(this->GetWindowAttribute_(GLFW_FOCUSED));
}

void window::RequestFocus()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    glfwRequestWindowAttention(this->windowId_);
}

void window::FocusOnShow()
{
    this->SetWindowAttribute_(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
}

void window::DontFocusOnShow()
{
    this->SetWindowAttribute_(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
}

bool window::IsFocusedOnShow()
{
    return static_cast<bool>(this->GetWindowAttribute_(GLFW_FOCUS_ON_SHOW));
}

void window::SetOpacity(float opacity)
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    glfwSetWindowOpacity(this->windowId_, opacity);
}

bool window::IsTransparent()
{
    return static_cast<bool>(
        this->GetWindowAttribute_(GLFW_TRANSPARENT_FRAMEBUFFER));
}

void window::SwapBuffers()
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }
    glfwSwapBuffers(this->windowId_);
}

VkResult window::CreateSurface(VkInstance instance,
                               const VkAllocationCallbacks* allocator)
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return VK_ERROR_UNKNOWN;
    }
    if (this->surface_ != nullptr) {
        internal::ERROR_CALLBACK(
            internal::ERROR_WINDOW_SURFACE_ALREADY_CREATED,
            internal::ERROR_MESSAGE_WINDOW_SURFACE_ALREADY_CREATED);
        return VK_ERROR_UNKNOWN;
    }
    return glfwCreateWindowSurface(
        instance, this->windowId_, allocator, &this->surface_);
}

std::vector<key_event> window::GetKeyEvents() const
{
    std::vector<key_event> truncatedKeyEvents = this->keyEvents;
    truncatedKeyEvents.resize(this->storedKeyEvents);
    return truncatedKeyEvents;
}

std::vector<character_event> window::GetCharacterEvents() const
{
    std::vector<character_event> truncatedCharacterEvents =
        this->characterEvents;
    truncatedCharacterEvents.resize(this->storedCharacterEvents);
    return truncatedCharacterEvents;
}

std::vector<cursor_position_event> window::GetCursorPositionEvents() const
{
    std::vector<cursor_position_event> truncatedCursorPositionEvents =
        this->cursorPositionEvents;
    truncatedCursorPositionEvents.resize(this->storedCursorPositionEvents);
    return truncatedCursorPositionEvents;
}

std::vector<cursor_enter_event> window::GetCursorEnterEvents() const
{
    std::vector<cursor_enter_event> truncatedCursorEnterEvents =
        this->cursorEnterEvents;
    truncatedCursorEnterEvents.resize(this->storedCursorEnterEvents);
    return truncatedCursorEnterEvents;
}

std::vector<mouse_button_event> window::GetMouseButtonEvents() const
{
    std::vector<mouse_button_event> truncatedMouseButtonEvents =
        this->mouseButtonEvents;
    truncatedMouseButtonEvents.resize(this->storedMouseButtonEvents);
    return truncatedMouseButtonEvents;
}

std::vector<scroll_event> window::GetScrollOffsetEvents() const
{
    std::vector<scroll_event> truncatedScrollEvents = this->scrollEvents;
    truncatedScrollEvents.resize(this->storedScrollEvents);
    return truncatedScrollEvents;
}

std::vector<file_drop_event> window::GetFileDropEvents() const
{
    std::vector<file_drop_event> truncatedFileDropEvents = this->fileDropEvents;
    truncatedFileDropEvents.resize(this->storedFileDropEvents);
    return truncatedFileDropEvents;
}

} // namespace gvw