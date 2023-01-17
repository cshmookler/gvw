#pragma once

// Standard includes
#include <vector>

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Local includes
#include "../common/const.hpp"
#include "../common/types.hpp"
#include "init.hpp"

namespace gvw {

class monitor;

// TODO: Unregister windows upon destruction
class window
{
    GLFWwindow* windowId_ = WINDOW_ID_NULL;
    VkSurfaceKHR surface_ = VK_NULL_HANDLE;
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

    int GetWindowAttribute_(int attribute);
    void SetWindowAttribute_(int attribute, int value);

    void RegisterWindowWithInputCallbacks_();

  public:
    // Input buffers
    std::vector<key_event> keyEvents;
    std::vector<character_event> characterEvents;
    std::vector<cursor_position_event> cursorPositionEvents;
    std::vector<cursor_enter_event> cursorEnterEvents;
    std::vector<mouse_button_event> mouseButtonEvents;
    std::vector<scroll_event> scrollEvents;
    std::vector<file_drop_event> fileDropEvents;

    size_t storedKeyEvents = 0;
    size_t storedCharacterEvents = 0;
    size_t storedCursorPositionEvents = 0;
    size_t storedCursorEnterEvents = 0;
    size_t storedMouseButtonEvents = 0;
    size_t storedScrollEvents = 0;
    size_t storedFileDropEvents = 0;

    // Constructors
    window();
    window(int windowWidth,
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

    void Create(int windowWidth,
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
    void Create(int windowWidth,
                int windowHeight,
                const char* windowTitle,
                monitor& fullScreenMonitor);
    void Create(int windowWidth,
                int windowHeight,
                const char* windowTitle,
                window& contextShareWindow);
    void Create(int windowWidth,
                int windowHeight,
                const char* windowTitle,
                monitor& fullScreenMonitor,
                window& contextShareWindow);

    template<typename type>
    void ScreenCoordinateToPixel(type screenCoordinateX,
                                 type screenCoordinateY,
                                 type& pixelX,
                                 type& pixelY);
    template<typename type>
    coordinate<type> ScreenCoordinateToPixel(
        coordinate<type>& screenCoordinate);
    template<typename type>
    size<type> ScreenCoordinateToPixel(size<type>& screenCoordinate);

    template<typename type>
    void PixelToScreenCoordinate(type pixelX,
                                 type pixelY,
                                 type& screenCoordinateX,
                                 type& screenCoordinateY);
    template<typename type>
    coordinate<type> PixelToScreenCoordinate(coordinate<type>& pixel);
    template<typename type>
    size<type> PixelToScreenCoordinate(size<type>& pixel);

    // Setup input buffers and event callbacks
    void SetupKeyInputBuffer();
    void SetupCharacterInputBuffer();
    void SetupCursorPositionInputBuffer();
    void SetupCursorEnterInputBuffer();
    void SetupMouseButtonInputBuffer();
    void SetupScrollInputBuffer();
    void SetupFileDropInputBuffer();

    // Clear input buffers
    void ClearInputBuffers();

    GLFWwindow* Id();

    bool ShouldClose(); // Returns the state of the 'close' flag
    void Close();       // Set the 'close' flag to true
    void CancelClose(); // Set the 'close' flag to false

    size<int> GetSizeInScreenCoordinates();
    size<int> GetSize();
    void SetSize(size<int> windowSize);
    void SetSize(int width, int height);

    coordinate<int> GetPositionInScreenCoordinates();
    coordinate<int> GetPosition();
    void SetPosition(coordinate<int> windowPosition);
    void SetPosition(int xPos, int yPos);

    coordinate<float> GetContentScale();

    void SetMinimumAndMaximumSize(int minimumWidth,
                                  int minimumHeight,
                                  int maximumWidth,
                                  int maximumHeight);
    void SetMinimumAndMaximumSize(size<int> minimumSize, size<int> maximumSize);
    void SetMinimumSize(int minimumWidth, int minimumHeight);
    void SetMinimumSize(size<int> minimumSize);
    void SetMaximumSize(int maximumWidth, int maximumHeight);
    void SetMaximumSize(size<int> maximumSize);

    void SetAspectRatio(int numerator, int denominator);

    void FullScreen(monitor& fullScreenMonitor);
    void ExitFullScreen();
    void ExitFullScreen(int xPos, int yPos, int width, int height);
    void ExitFullScreen(coordinate<int> position, size<int> size);

    void SetTitle(const char* title);

    void SetIcon(const char* iconImagePath);
    void SetIcon(image iconImage);
    void SetCandidateIcons(std::vector<const char*> candidateIconImagePaths);
    void SetCandidateIcons(std::vector<image> candidateIconImages);

    bool IsCursorHovering();
    bool IsResizable();

    void Decorate();
    void Undecorate();
    bool IsDecorated();

    void MinimizeOnFocusLoss();
    void DontMinimizeOnFocusLoss();
    bool IsMinimizedOnFocusLoss();

    void AlwaysOnTop();
    void NotAlwaysOnTop();
    bool IsAlwaysOnTop();

    void Minimize();
    void Maximize();
    void Restore();
    bool IsMinimized();
    bool IsMaximized();

    void Hide();
    void Show();
    bool IsVisible();

    void Focus();
    bool IsFocused();

    void RequestFocus();

    void FocusOnShow();
    void DontFocusOnShow();
    bool IsFocusedOnShow();

    void SetOpacity(float opacity);
    bool IsTransparent();

    void SwapBuffers();

    VkResult CreateSurface(VkInstance instance,
                           const VkAllocationCallbacks* allocator);

    std::vector<key_event> GetKeyEvents();
    std::vector<character_event> GetCharacterEvents();
    std::vector<cursor_position_event> GetCursorPositionEvents();
    std::vector<cursor_enter_event> GetCursorEnterEvents();
    std::vector<mouse_button_event> GetMouseButtonEvents();
    std::vector<scroll_event> GetScrollOffsetEvents();
    std::vector<file_drop_event> GetFileDropEvents();
};

} // namespace gvw