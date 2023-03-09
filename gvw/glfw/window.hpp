#pragma once

/**
 * @file window.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief The window class.
 * @date 2023-03-03
 */

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

/// @todo Unregister windows with no initialized input buffers.
/// @todo Allow creation and deletion of custom cursors.

/// @brief Window creation and management.
class window
{
    GLFWwindow* windowId_ = con::WINDOW_ID_NULL;
    VkSurfaceKHR surface_ = VK_NULL_HANDLE;
    coordinate<int> defaultPositionInScreenCoordinates_;
    size<int> defaultSizeInScreenCoordinates_;
    int displayMode_ = con::NOT_CREATED;

    // Checks if the window has been created.
    // If it has not, an error is sent to the GLFW error callback.
    bool AssertCreation_(
        int errorCode = con::ERROR_WINDOW_NOT_CREATED_BEFORE_OPERATION,
        const char* errorMessage =
            con::ERROR_MESSAGE_WINDOW_NOT_CREATED_BEFORE_OPERATION);

    int GetWindowAttribute_(int attribute);
    void SetWindowAttribute_(int attribute, int value);

    void RegisterWindowWithInputCallbacks_();
    void UnregisterWindowWithInputCallbacks_();

    /// @brief Input buffer for storing key events.
    std::vector<key_event> keyEvents_;

    /// @brief Input buffer for storing character events.
    std::vector<character_event> characterEvents_;

    /// @brief Input buffer for storing cursor position events.
    std::vector<cursor_position_event> cursorPositionEvents_;

    /// @brief Input buffer for storing cursor enter events.
    std::vector<cursor_enter_event> cursorEnterEvents_;

    /// @brief Input buffer for storing mouse button events.
    std::vector<mouse_button_event> mouseButtonEvents_;

    /// @brief Input buffer for storing scroll events.
    std::vector<scroll_event> scrollEvents_;

    /// @brief Input buffer for storing file drop events.
    std::vector<file_drop_event> fileDropEvents_;

    /// @brief The number of events stored in the key events input buffer.
    size_t storedKeyEvents_ = 0;

    /// @brief The number of events stored in the character events input buffer.
    size_t storedCharacterEvents_ = 0;

    /// @brief The number of events stored in the cursor position events input
    /// buffer.
    size_t storedCursorPositionEvents_ = 0;

    /// @brief The number of events stored in the cursor enter events input
    /// buffer.
    size_t storedCursorEnterEvents_ = 0;

    /// @brief The number of events stored in the mouse button events input
    /// buffer.
    size_t storedMouseButtonEvents_ = 0;

    /// @brief The number of events stored in the scroll events input buffer.
    size_t storedScrollEvents_ = 0;

    /// @brief The number of events stored in the file drop events input buffer.
    size_t storedFileDropEvents_ = 0;

  public:
    /// @brief Creates the window.
    /// @param width Initial width of the window.
    /// @param height Initial height of the window.
    /// @param title The title of the window.
    /// @param className The class name of the window. Used to group
    /// processes together on linux systems using the X window manager.
    /// @param fullScreenMonitorId The ID of the monitor that the window will be
    /// put into full screen mode on. If set to nullptr, the window is not put
    /// into full screen mode.
    /// @param contextShareWindowId The ID of the parent window. If set to
    /// nullptr, the window is assumed to not be the child of another window.
    void Create(int width,
                int height,
                const char* title,
                const char* className,
                GLFWmonitor* fullScreenMonitorId,
                GLFWwindow* contextShareWindowId);

    /// @brief Initializes the window object without creating the window. The
    /// `window::Create` function must be called to create the window.
    window();

    /// @brief Creates the window.
    /// @param width Initial width of the window.
    /// @param height Initial height of the window.
    /// @param title The title of the window.
    /// @param className The class name of the window. Used to group
    /// processes together on linux systems using the X window manager.
    /// @param fullScreenMonitorId The ID of the monitor that the window will be
    /// put into full screen mode on. If set to nullptr, the window is not put
    /// into full screen mode.
    /// @param contextShareWindowId The ID of the parent window. If set to
    /// nullptr, the window is assumed to not be the child of another window.
    window(int width,
           int height,
           const char* title,
           const char* className,
           GLFWmonitor* fullScreenMonitorId,
           GLFWwindow* contextShareWindowId);

    /// @brief Destroys the window.
    void Destroy();

    /// @brief Destroys the window object. Calls the `window::Destroy` function.
    ~window();

    // Delete the copy constructor and copy assignment operators
    // It should not be possible to copy this object
    window(const window&) = delete;
    window& operator=(const window&) = delete;

    /// @brief Returns the window ID.
    /// @return A GLFWwindow* object.
    GLFWwindow* Id();

    /// @brief Start recording key events. If the key event input buffer is not
    /// initialized, key events are discarded.
    void InitKeyInputBuffer();

    /// @brief Start recording character events. If the character event input
    /// buffer is not initialized, character events are discarded.
    void InitCharacterInputBuffer();

    /// @brief Start recording cursor position events. If the cursor position
    /// event input buffer is not initialized, cursor position events are
    /// discarded.
    void InitCursorPositionInputBuffer();

    /// @brief Start recording cursor enter events. If the cursor enter event
    /// input buffer is not initialized, cursor enter events are discarded.
    void InitCursorEnterInputBuffer();

    /// @brief Start recording mouse button events. If the mouse button event
    /// input buffer is not initialized, mouse button events are discarded.
    void InitMouseButtonInputBuffer();

    /// @brief Start recording scroll events. If the scroll event input buffer
    /// is not initialized, scroll events are discarded.
    void InitScrollInputBuffer();

    /// @brief Start recording file drop events. If the file drop event input
    /// buffer is not initialized, file drop events are discarded.
    void InitFileDropInputBuffer();

    /// @brief Stop storing new key events, but don't delete the key events
    /// already stored in the key event input buffer.
    void FreezeKeyInputBuffer();

    /// @brief Stop storing new character events, but don't delete the character
    /// events already stored in the character event input buffer.
    void FreezeCharacterInputBuffer();

    /// @brief Stop storing new cursor position events, but don't delete the
    /// cursor position events already stored in the cursor position event input
    /// buffer.
    void FreezeCursorPositionInputBuffer();

    /// @brief Stop storing new cursor enter events, but don't delete the cursor
    /// enter events already stored in the cursor enter event input buffer.
    void FreezeCursorEnterInputBuffer();

    /// @brief Stop storing new mouse button events, but don't delete the mouse
    /// button events already stored in the mouse button event input buffer.
    void FreezeMouseButtonInputBuffer();

    /// @brief Stop storing new scroll events, but don't delete the
    /// scroll events already stored in the scroll event input buffer.
    void FreezeScrollInputBuffer();

    /// @brief Stop storing new file drop events, but don't delete the
    /// file drop events already stored in the file drop event input buffer.
    void FreezeFileDropInputBuffer();

    /// @brief Clears all input buffers. All events stored in the input buffers
    /// are deleted.
    void ClearInputBuffers();

    /// @brief Returns the state of the "close" flag.
    bool ShouldClose();

    /// @brief Sets the "close" flag to true.
    void Close();

    /// @brief Sets the "close" flag to false.
    void CancelClose();

    /// @brief Returns the size of the window in screen coordinates.
    /// @return A `size` object of type int.
    size<int> GetSize();

    /// @brief Returns the size of the window in pixels.
    /// @return A `size` object of type int.
    size<int> GetSizeInPixels();

    /// @brief Sets the size of the window in screen coordinates.
    /// @param windowSize
    void SetSize(size<int> windowSize);

    /// @brief Sets the size of the window in screen coordinates.
    /// @param width
    /// @param height
    void SetSize(int width, int height);

    /// @brief Returns the position of the window in screen coordinates.
    /// @return A `coordinate` object of type int.
    coordinate<int> GetPosition();

    /// @brief Sets the position of the window in screen coordinates.
    /// @param windowPosition
    void SetPosition(coordinate<int> windowPosition);

    /// @brief Sets the position of the window in screen coordinates.
    /// @param xPos
    /// @param yPos
    void SetPosition(int xPos, int yPos);

    /// @brief Returns the content scale of the window. The content scale is the
    /// ratio between the current DPI and the platform's default DPI. It is used
    /// to appropriately scale text and graphical elements to they appear at
    /// roughly the same size on different monitors.
    /// @return A `coordinate` object of type float.
    coordinate<float> GetContentScale();

    /// @brief Sets the minimum and maximum size of the window.
    /// @param minimumWidth
    /// @param minimumHeight
    /// @param maximumWidth
    /// @param maximumHeight
    void SetMinimumAndMaximumSize(int minimumWidth,
                                  int minimumHeight,
                                  int maximumWidth,
                                  int maximumHeight);

    /// @brief Sets the minimum and maximum size of the window.
    /// @param minimumSize
    /// @param maximumSize
    void SetMinimumAndMaximumSize(size<int> minimumSize, size<int> maximumSize);

    /// @brief Sets the minimum size of the window.
    /// @param minimumWidth
    /// @param minimumHeight
    void SetMinimumSize(int minimumWidth, int minimumHeight);

    /// @brief Sets the minimum size of the window.
    /// @param minimumSize
    void SetMinimumSize(size<int> minimumSize);

    /// @brief Sets the maximum size of the window.
    /// @param maximumWidth
    /// @param maximumHeight
    void SetMaximumSize(int maximumWidth, int maximumHeight);

    /// @brief Sets the maximum size of the window.
    /// @param maximumSize
    void SetMaximumSize(size<int> maximumSize);

    /// @brief Sets the aspect ratio of the window.
    /// @param numerator
    /// @param denominator
    void SetAspectRatio(int numerator, int denominator);

    /// @brief Exits windowed mode and enters full screen mode.
    /// @param fullScreenMonitor
    void FullScreen(monitor& fullScreenMonitor);

    /// @brief Exits full screen mode and enters windowed mode.
    void ExitFullScreen();

    /// @brief Exits full screen mode and enters windowed mode at a specific
    /// size and position.
    /// @param xPos
    /// @param yPos
    /// @param width
    /// @param height
    void ExitFullScreen(int xPos, int yPos, int width, int height);

    /// @brief Exits full screen mode and enters windowed mode at a specific
    /// size and position.
    /// @param position
    /// @param size
    void ExitFullScreen(coordinate<int> position, size<int> size);

    /// @brief Sets the title of the window.
    void SetTitle(const char* title);

    /// @brief Sets the icon of the window to an image stored on the file
    /// system.
    /// @param iconImagePath
    void SetIcon(const char* iconImagePath);

    /// @brief Sets the icon of the window to an image stored in an `image`
    /// object.
    /// @param iconImage
    void SetIcon(image iconImage);

    /// @brief Set a vector of candidate images stored on the file system.
    /// @param candidateIconImagePaths
    void SetCandidateIcons(std::vector<const char*> candidateIconImagePaths);

    /// @brief Set a vector of candidate images stored in `image` objects.
    /// @param candidateIconImages
    void SetCandidateIcons(std::vector<image> candidateIconImages);

    /// @brief Returns the state of the cursor hovering over the window.
    /// @return True if the cursor is hovering over the window. False if the
    /// cursor is not hovering over the window.
    bool IsCursorHovering();

    /// @brief Returns the resizability of the window.
    /// @return True if the window can be reisized. False if the window cannot
    /// be resized.
    bool IsResizable();

    /// @brief Decorates the window. Decorating a window adds borders, the
    /// window title, and the minimize/maximize/close buttons.
    void Decorate();

    /// @brief Undecorates the window. Undecorating a window removes everything
    /// except the frame. Borders, the window title and the
    /// minimize/maximize/close buttons are removed.
    void Undecorate();

    /// @brief Returns the decoration state of the window.
    /// @return True if the window is decorated. False if the window is not
    /// decorated.
    bool IsDecorated();

    /// @brief Minimizes the window when focus is lost.
    void MinimizeOnFocusLoss();

    /// @brief Doesn't minimize the window when focus is lost.
    void DontMinimizeOnFocusLoss();

    /// @brief Returns the state of the window's minimization on focus loss.
    /// @return True if the window minimizes on focus loss. False if the window
    /// doesn't minimize on focus loss.
    bool IsMinimizedOnFocusLoss();

    /// @brief Sets the window on top of all other windows even when focus is
    /// lost.
    void AlwaysOnTop();

    /// @brief Allows other windows to cover the window if focus is lost.
    void NotAlwaysOnTop();

    /// @brief Returns the state of the window's precidence over other windows
    /// if focus is lost.
    /// @return True if the window is always on top of other windows. False if
    /// the window is only on top of other windows if it has focus.
    bool IsAlwaysOnTop();

    /// @brief Minimizes the window.
    void Minimize();

    /// @brief Maximizes the window.
    void Maximize();

    /// @brief Unmaximises or unminimises the window.
    void Restore();

    /// @brief Returns the minimization state of the window.
    /// @return True if the window is minimized. False if the window is not
    /// minimized.
    bool IsMinimized();

    /// @brief Returns the maximization state of the window.
    /// @return True if the window is maximised. False if the window is not
    /// maximised.
    bool IsMaximized();

    /// @brief Hides the window if it is not already hidden. This function does
    /// nothing if the window is in full screen mode.
    void Hide();

    /// @brief Makes the window visible if it is not already visible. This
    /// function does nothing if the window is in full screen mode.
    void Show();

    /// @brief Returns the visibility state of the window.
    /// @return True if the window is visible. False if the window is hidden.
    bool IsVisible();

    /// @brief Brings focus to the window.
    void Focus();

    /// @brief Returns the state of the window's focus.
    /// @return True if the window is focused. False if the window is not
    /// focused.
    bool IsFocused();

    /// @brief Requests focus to the window.
    void RequestFocus();

    /// @brief Focus is brought to the window when the window is shown.
    void FocusOnShow();

    /// @brief Focus is not brought to the window when the window is shown.
    void DontFocusOnShow();

    /// @brief Returns the state of focus brought to the window when the window
    /// is shown.
    /// @return True if the window is given focus when it's shown. False if the
    /// window is not given focus when it's shown.
    bool IsFocusedOnShow();

    /// @brief Sets the opacity of the window frame.
    /// @param opacity
    void SetOpacity(float opacity);

    /// @brief Returns the transparency state of the window.
    bool IsTransparent();

    /// @brief Swaps the window's frame buffers (Updates the window frame with
    /// whatever was last drawn to it).
    void SwapBuffers();

    /// @brief Creates a vulkan surface for the window.
    /// @param instance
    /// @param allocator
    /// @return A VkResult object.
    VkResult CreateSurface(VkInstance instance,
                           const VkAllocationCallbacks* allocator);

    /// @brief Returns a vector of key events retrieved from the key event input
    /// buffer. Key events are refreshed when events are polled.
    /// @return A vector of `key_event` objects.
    std::vector<key_event> GetKeyEvents() const;

    /// @brief Returns a vector of character events retrieved from the character
    /// event input buffer. Character events are refreshed when events are
    /// polled.
    /// @return A vector of `character_event` objects.
    std::vector<character_event> GetCharacterEvents() const;

    /// @brief Returns a vector of cursor position events retrieved from the
    /// cursor position event input buffer. Cursor position events are refreshed
    /// when events are polled.
    /// @return A vector of `cursor_position_event` objects.
    std::vector<cursor_position_event> GetCursorPositionEvents() const;

    /// @brief Returns a vector of cursor enter events retrieved from the cursor
    /// enter event input buffer. Cursor enter events are refreshed when events
    /// are polled.
    /// @return A vector of `cursor_enter_event` objects.
    std::vector<cursor_enter_event> GetCursorEnterEvents() const;

    /// @brief Returns a vector of mouse button events retrieved from the mouse
    /// button event input buffer. Mouse button events are refreshed when events
    /// are polled.
    /// @return A vector of `mouse_button_event` objects.
    std::vector<mouse_button_event> GetMouseButtonEvents() const;

    /// @brief Returns a vector of scroll events retrieved from the scroll event
    /// input buffer. Scroll events are refreshed when events are polled.
    /// @return A vector of `scroll_event` objects.
    std::vector<scroll_event> GetScrollOffsetEvents() const;

    /// @brief Returns a vector of file drop events retrieved from the file drop
    /// event input buffer. File drop events are refreshed when events are
    /// polled.
    /// @return A vector of `file_drop_event` objects.
    std::vector<file_drop_event> GetFileDropEvents() const;
};

} // namespace gvw