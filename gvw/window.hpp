#pragma once

/**
 * @file window.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief The window class.
 * @date 2023-05-22
 */

// Standard includes

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
// #include <vulkan/vulkan.hpp>

// Local includes
#include "init.hpp"
#include "monitor.hpp"
#include "types.tpp"

/// @brief Creates and manages a GLFW window.
class window_t
{
  private:
    //////////////////////////////////////////////////
    //        Private Static Const Variables        //
    //////////////////////////////////////////////////
    // NOLINTBEGIN

    /// @brief Joystick events.
    static std::vector<gvw::joystick_event> JOYSTICK_EVENTS;

    // NOLINTEND

  public:
    //////////////////////////////////////////////////
    //         Public Static Const Variables        //
    //////////////////////////////////////////////////

    /// @brief The window position is invalid.
    static const gvw::error ERROR_GLFW_INVALID_POSITION;

    /// @brief The window size is invalid.
    static const gvw::error ERROR_GLFW_INVALID_SIZE;

    /// @brief Default values for general window hints.
    static const gvw::glfw_general_hints DEFAULT_GLFW_GENERAL_HINTS;

    /// @brief Default values for framebuffer hints.
    static const gvw::glfw_framebuffer_hints DEFAULT_GLFW_FRAMEBUFFER_HINTS;

    /// @brief Default values for monitor hints.
    static const gvw::glfw_monitor_hints DEFAULT_GLFW_MONITOR_HINTS;

    /// @brief Default values for context hints.
    static const gvw::glfw_context_hints DEFAULT_GLFW_CONTEXT_HINTS;

    /// @brief Default values for MacOS window hints.
    static const gvw::glfw_macos_window_hints DEFAULT_GLFW_MACOS_WINDOW_HINTS;

    /// @brief Default values for Linux window hints.
    static const gvw::glfw_linux_window_hints DEFAULT_GLFW_LINUX_WINDOW_HINTS;

    /// @brief The default position of the window.
    static const gvw::coordinate<int> DEFAULT_WINDOW_POSITION;

    /// @brief The default size of the window.
    static const gvw::area<int> DEFAULT_WINDOW_SIZE;

    /// @brief The default minimum size of the window.
    static const gvw::area<int> DEFAULT_MINIMUM_WINDOW_SIZE;

    /// @brief The default maximum size of the window.
    static const gvw::area<int> DEFAULT_MAXIMUM_WINDOW_SIZE;

    /// @brief Null window position.
    static const gvw::coordinate<int> NULL_WINDOW_POSITION;

    /// @brief Null window size.
    static const gvw::area<int> NULL_WINDOW_SIZE;

    //////////////////////////////////////////////////
    //            Public Static Functions           //
    //////////////////////////////////////////////////

    /// @brief The joystick event callback.
    /// @param JID The joystick id.
    /// @param Event The joystick event.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Must be called from the main thread.
    static void JoystickCallback(int JID, int Event);

    /// @brief Returns the joystick event buffer.
    /// @returns A reference to a vector of joystick events.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Must be called from the main thread.
    static std::vector<gvw::joystick_event>& JoystickEvents();

    /// @brief Clears the joystick event buffer.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Must be called from the main thread.
    static void ClearJoystickEvents();

    /// @brief Returns the GVW instance associated with a specific window
    /// handle.
    /// @param Window The window handle.
    /// @returns A pointer to a gvw object.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`.
    /// @thread_safety Can be called from any thread.
    [[nodiscard]] static window_t& WindowInstance(GLFWwindow* Window);

    /// @brief The key event callback.
    /// @param Window The window handle.
    /// @param Key The keyboard key.
    /// @param Scancode The key scancode.
    /// @param Action The key action.
    /// @param Mods Key modifier bits.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`.
    /// @thread_safety Can be called from any thread.
    static void KeyCallback(GLFWwindow* Window,
                            int Key,
                            int Scancode,
                            int Action,
                            int Mods);

    /// @brief The character event callback.
    /// @param Window The window handle.
    /// @param Code_Point The unicode code point.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`.
    /// @thread_safety Can be called from any thread.
    static void CharacterCallback(GLFWwindow* Window, unsigned int Code_Point);

    /// @brief The cursor position event callback.
    /// @param Window The window handle.
    /// @param X_Position The X position of the cursor on the window.
    /// @param Y_Position The Y position of the cursor on the window.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`.
    /// @thread_safety Can be called from any thread.
    static void CursorPositionCallback(GLFWwindow* Window,
                                       double X_Position,
                                       double Y_Position);

    /// @brief The cursor enter event callback.
    /// @param Window The window handle.
    /// @param Entered `GLFW_TRUE` if the cursor has entered the window.
    /// `GLFW_FALSE` if the cursor has left the window.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`.
    /// @thread_safety Can be called from any thread.
    static void CursorEnterCallback(GLFWwindow* Window, int Entered);

    /// @brief The mouse button event callback.
    /// @param Window The window handle.
    /// @param Button The mouse button.
    /// @param Action The mouse button action.
    /// @param Mods Mouse button modifier bits.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`.
    /// @thread_safety Can be called from any thread.
    static void MouseButtonCallback(GLFWwindow* Window,
                                    int Button,
                                    int Action,
                                    int Mods);

    /// @brief The scroll event callback.
    /// @param Window The window handle.
    /// @param X_Offset The X offset of the scroll wheel.
    /// @param Y_Offset The Y offset of the scroll wheel.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`.
    /// @thread_safety Can be called from any thread.
    static void ScrollCallback(GLFWwindow* Window,
                               double X_Offset,
                               double Y_Offset);

    /// @brief The file drop event callback.
    /// @param Window The window handle.
    /// @param Count The number of files dropped.
    /// @param Paths A C-style array of file paths.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`.
    /// @thread_safety Can be called from any thread.
    static void FileDropCallback(GLFWwindow* Window,
                                 int Count,
                                 const char** Paths);

  private:
    //////////////////////////////////////////////////
    //               Private Variables              //
    //////////////////////////////////////////////////

    /// @brief The pointer to the underlying GLFW window object.
    GLFWwindow* windowHandle = nullptr;

    /// @brief The Vulkan window surface.
    VkSurfaceKHR_T* surface = VK_NULL_HANDLE;

    /// @brief The reset size of the window. This is the size of the window when
    /// it exits full screen, maximization, or iconification.
    gvw::area<int> resetSize = DEFAULT_WINDOW_SIZE;

    /// @brief The reset position of the window. This is the position of the
    /// window when it exits full screen, maximization, or iconification.
    gvw::coordinate<int> resetPosition = DEFAULT_WINDOW_POSITION;

    /// @brief Key events.
    std::vector<gvw::key_event> keyEvents = {};

    /// @brief Character events.
    std::vector<gvw::character_event> characterEvents = {};

    /// @brief Cursor position events.
    std::vector<gvw::cursor_position_event> cursorPositionEvents = {};

    /// @brief Cursor enter events.
    std::vector<gvw::cursor_enter_event> cursorEnterEvents = {};

    /// @brief Mouse button events.
    std::vector<gvw::mouse_button_event> mouseButtonEvents = {};

    /// @brief Scroll events.
    std::vector<gvw::scroll_event> scrollEvents = {};

    /// @brief File drop events.
    std::vector<gvw::file_drop_event> fileDropEvents = {};

    //////////////////////////////////////////////////
    //               Private Functions              //
    //////////////////////////////////////////////////

    /// @brief Initializes all input buffers (key, character, cursor position,
    /// cursor enter, mouse button, scroll input, and file drop).
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`.
    /// @thread_safety Must be called from the main thread.
    void InitInputBuffers();

    /// @brief Returns an attribute of the window.
    /// @param Attribute The GLFW window attribute.
    /// @returns The value of the specified attribute.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM`, or
    /// `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] int WindowAttribute(int Attribute) const;

    /// @brief Sets an attribute of the window.
    /// @param Attribute The GLFW window attribute.
    /// @param Value The value that the attribute will be set to.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM`,
    /// `GLFW_INVALID_VALUE` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void WindowAttribute(int Attribute, int Value) const;

  public:
    //////////////////////////////////////////////////
    //               Public Functions               //
    //////////////////////////////////////////////////

    /// @brief Creates a window, sets the GLFW and GVW error callbacks, and
    /// initializes GLFW if it hasn't already been initialized.
    /// @remark The first 11 parameters are for window creation (position, size,
    /// title, associatedMonitor, parentWindow, generalHInts, framebufferHints,
    /// monitorHints, contextHints, macosWindowHints, linuxWindowHints).
    /// @remark The last 4 parameters are for GVW initialization. If GVW is
    /// already initialized (this function has been run once before), then
    /// arguments passed to these parameters are ignored.
    /// @param Position The initial position of the window. If this is set to
    /// `DEFAULT_WINDOW_POSITION`, then the position of the window is
    /// automatically determined by the operating system.
    /// @param Size The initial size of the window. This can be set to
    /// `DEFAULT_WINDOW_SIZE`.
    /// @param Title The title of the window. This be an empty string, but
    /// cannot be nullptr.
    /// @param Full_Screen_Monitor The full screen monitor of the window.
    /// Passing nullptr to this parameter creates the window without associating
    /// it with any particular monitor.
    /// @param Parent_Window The parent window. Passing nullptr to this
    /// parameter creates the window independant of any other window.
    /// @param General_Hints General hints for window creation. These hints
    /// change the general appearance and behavior of the window.
    /// @param Framebuffer_Hints Framebuffer hints for window creation. Changing
    /// these hints is NOT recommended.
    /// @param Monitor_Hints Monitor hints for window creation. Sets the
    /// preferred refresh rate. Changing these hints is NOT recommended.
    /// @param Context_Hints Context hints for window creation. These hints only
    /// apply to OpenGL. These hints may be removed entirely in later versions
    /// of GVW.
    /// @param Macos_Window_Hints MacOS specific window creation hints.
    /// @param Linux_Window_Hints Linux specific window creation hints.
    /// @param GLFW_Error_Callback The GLFW error callback. The default GLFW
    /// error callback throws all errors.
    /// @param GVW_Error_Callback The GVW error callback. The default GVW error
    /// callback throws all errors.
    /// @param Shared_Init_Hints Shared initialization hints for GLFW. Ignored
    /// if GLFW is already initialized.
    /// @param Macos_Init_Hints MacOS specific initialization hints for GLFW.
    /// Ignored if GLFW is already initialized.
    /// @gvw_errors `ERROR_GLFW_INIT_FAILED` or `ERROR_VULKAN_NOT_SUPPORTED`.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM`,
    /// `GLFW_INVALID_VALUE`, `GLFW_API_UNAVAILABLE`,
    /// `GLFW_VERSION_UNAVAILABLE`, `GLFW_FORMAT_UNAVAILABLE`, and
    /// `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    window_t(const gvw::coordinate<int>& Position = DEFAULT_WINDOW_POSITION,
             const gvw::area<int>& Size = DEFAULT_WINDOW_SIZE,
             const char* Title = "",
             GLFWmonitor* Full_Screen_Monitor = nullptr,
             GLFWwindow* Parent_Window = nullptr,
             const gvw::glfw_general_hints& General_Hints =
                 DEFAULT_GLFW_GENERAL_HINTS,
             const gvw::glfw_framebuffer_hints& Framebuffer_Hints =
                 DEFAULT_GLFW_FRAMEBUFFER_HINTS,
             const gvw::glfw_monitor_hints& Monitor_Hints =
                 DEFAULT_GLFW_MONITOR_HINTS,
             const gvw::glfw_context_hints& Context_Hints =
                 DEFAULT_GLFW_CONTEXT_HINTS,
             const gvw::glfw_macos_window_hints& MacOS_Window_Hints =
                 DEFAULT_GLFW_MACOS_WINDOW_HINTS,
             const gvw::glfw_linux_window_hints& Linux_Window_Hints =
                 DEFAULT_GLFW_LINUX_WINDOW_HINTS,
             GLFWerrorfun GLFW_Error_Callback = gvw::DefaultGlfwErrorCallback,
             gvw::gvw_error_callback GVW_Error_Callback =
                 gvw::DefaultGvwErrorCallback,
             const gvw::glfw_shared_init_hints& Shared_Init_Hints =
                 gvw::DEFAULT_GLFW_SHARED_INIT_HINTS,
             const gvw::glfw_macos_init_hints& MacOS_Init_Hints =
                 gvw::DEFAULT_GLFW_MACOS_INIT_HINTS);

    // Delete the copy constructor, copy assignment operator, move constructor,
    // and move assignment operator. It should not be possible to copy or move
    // this object.
    window_t(const window_t&) = delete;
    window_t& operator=(const window_t&) = delete;
    window_t(window_t&&) = delete;
    window_t& operator=(window_t&&) = delete;

    /// @todo Make this function thread safe!
    /// @brief The destructor is public so as to allow explicit destruction
    /// using the delete operator.
    ~window_t();

    /// @brief Returns the handle to the underlying GLFW window object.
    /// @returns A pointer to a `GLFWwindow` object.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Can be called from any thread.
    [[nodiscard]] GLFWwindow* WindowHandle() const noexcept;

    /// @brief Returns the key event buffer.
    /// @returns A reference to a vector of key events.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] std::vector<gvw::key_event>& KeyEvents() noexcept;

    /// @brief Returns the character event buffer.
    /// @returns A reference to a vector of character events.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] std::vector<gvw::character_event>& CharacterEvents() noexcept;

    /// @brief Returns the cursor position buffer.
    /// @returns A reference to a vector of cursor position events.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] std::vector<gvw::cursor_position_event>&
    CursorPositionEvents() noexcept;

    /// @brief Returns the cursor enter event buffer.
    /// @returns A reference to a vector of cursor enter events.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] std::vector<gvw::cursor_enter_event>&
    CursorEnterEvents() noexcept;

    /// @brief Returns the mouse button event buffer.
    /// @returns A reference to a vector of mouse button events.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] std::vector<gvw::mouse_button_event>&
    MouseButtonEvents() noexcept;

    /// @brief Returns the scroll event buffer.
    /// @returns A reference to a vector of scroll events.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] std::vector<gvw::scroll_event>& ScrollEvents() noexcept;

    /// @brief Returns the file drop event buffer.
    /// @returns A reference to a vector of file drop events.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] std::vector<gvw::file_drop_event>& FileDropEvents() noexcept;

    /// @brief Clears the key event buffer.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Must be called from the main thread.
    void ClearKeyEvents() noexcept;

    /// @brief Clears the character event buffer.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Must be called from the main thread.
    void ClearCharacterEvents() noexcept;

    /// @brief Clears the cursor position event buffer.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Must be called from the main thread.
    void ClearCursorPositionEvents() noexcept;

    /// @brief Clears the cursor enter event buffer.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Must be called from the main thread.
    void ClearCursorEnterEvents() noexcept;

    /// @brief Clears the mouse button event buffer.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Must be called from the main thread.
    void ClearMouseButtonEvents() noexcept;

    /// @brief Clears the scroll event buffer.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Must be called from the main thread.
    void ClearScrollEvents() noexcept;

    /// @brief Clears the file drop event buffer.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Must be called from the main thread.
    void ClearFileDropEvents() noexcept;

    /// @brief Clears all event buffers.
    /// @gvw_errors None.
    /// @glfw_errors None.
    /// @thread_safety Must be called from the main thread.
    void ClearEvents() noexcept;

    /// @brief Returns the state of the 'close' flag.
    /// @returns True if the 'close' flag is `GLFW_TRUE`. False if the 'close'
    /// flag is `GLFW_FALSE`.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`.
    /// @thread_safety Can be called from any thread.
    [[nodiscard]] bool ShouldClose() const;

    /// @brief Sets the state of the 'close' flag.
    /// @param State The new state of the 'close' flag.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`.
    /// @thread_safety Can be called from any thread.
    void ShouldClose(bool State) const;

    /// @brief Returns the size of the content area of the window in screen
    /// coordinates.
    /// @returns An `area` object of type int.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] gvw::area<int> Size() const;

    /// @brief Sets the size of the window.
    /// @param Size The new size of the window.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void Size(const gvw::area<int>& Size) const;

    /// @brief Returns the size of the content area of the window in pixels.
    /// @returns An `area` object of type int.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] gvw::area<int> SizeInPixels() const;

    /// @brief Returns the position of the window in screen coordinates.
    /// @returns A `coordinate` object of type int.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] gvw::coordinate<int> Position() const;

    /// @brief Sets the position of the window in screen coordinates.
    /// @param Position The new position of the window.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void Position(const gvw::coordinate<int>& Position) const;

    /// @brief Returns the content scale of the window.
    /// @remark The content scale of the window is the ratio between the current
    /// DPI and the platform's default DPI.
    /// @returns A `coordinate` object of type float.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] gvw::coordinate<float> ContentScale() const;

    /// @brief Sets the minimum and maximum size limits of the window.
    /// @param Minimum_Size The minimum size of the window. If this is set to
    /// `DEFAULT_MINIMUM_WINDOW_SIZE`, the window has no minimum size.
    /// @param Maximum_Size The maximum size of the window. If this is set to
    /// `DEFAULT_MAXIMUM_WINDOW_SIZE`, the window has no maximum size.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_VALUE` or
    /// `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void SizeLimits(
        const gvw::area<int>& Minimum_Size = DEFAULT_MINIMUM_WINDOW_SIZE,
        const gvw::area<int>& Maximum_Size = DEFAULT_MAXIMUM_WINDOW_SIZE) const;

    /// @brief Sets the aspect ratio of the window.
    /// @param Numerator The width of the window.
    /// @param Denominator The height of the window.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_VALUE` or
    /// `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void AspectRatio(int Numerator, int Denominator) const;

    /// @todo Declare and define the GLFW full screen functions.

    /// @todo Declare and define the GLFW image related functions.

    /// @brief Returns the cursor hovering state over the window.
    /// @returns True if the cursor is hovering over the window. False if the
    /// cursor is NOT hovering over the window.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM` or
    /// `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] bool IsCursorHovering() const;

    /// @brief Returns the resizability of the window.
    /// @returns True if the window is resizable. False if the window is NOT
    /// resizable.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM` or
    /// `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] bool IsResizable() const;

    /// @brief Decorates the window.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM`,
    /// `GLFW_INVALID_VALUE` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void Decorate() const;

    /// @brief Undecorates the window.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM`,
    /// `GLFW_INVALID_VALUE` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void Undecorate() const;

    /// @brief Returns the decoration state of the window.
    /// @returns True if the window is decorated. False if the window is NOT
    /// decorated.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM` or
    /// `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] bool IsDecorated() const;

    /// @brief Minimizes the window when focus is lost.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM`,
    /// `GLFW_INVALID_VALUE` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void MinimizeOnFocusLoss() const;

    /// @brief Does NOT minimize the window when focus is lost.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM`,
    /// `GLFW_INVALID_VALUE` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void DontMinimizeOnFocusLoss() const;

    /// @brief Returns the state of the window's minimization on focus loss.
    /// @returns True if the the window is minimized on focus loss. False if the
    /// window is NOT minimized on focus loss.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM` or
    /// `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] bool IsMinimizedOnFocusLoss() const;

    /// @brief Sets the window on top of all other windows even when focus is
    /// lost.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM`,
    /// `GLFW_INVALID_VALUE` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void AlwaysOnTop() const;

    /// @brief Allows other windows to cover the window if focus is lost.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM`,
    /// `GLFW_INVALID_VALUE` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void NotAlwaysOnTop() const;

    /// @brief Returns the state of the window's precidence over other windows
    /// if focus is lost.
    /// @returns True if the the window is always on top of other windows. False
    /// if the window is only on top of other windows if it has focus.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM` or
    /// `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] bool IsAlwaysOnTop() const;

    /// @brief Unmaximizes or unminimizes the window.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void Restore() const;

    /// @brief Minimizes the window.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void Minimize() const;

    /// @brief Returns the minimization state of the window.
    /// @returns True if the the window is minimized. False if the window is NOT
    /// minimized.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM` or
    /// `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] bool IsMinimized() const;

    /// @brief Maximizes the window.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void Maximize() const;

    /// @brief Returns the Maximization state of the window.
    /// @returns True if the the window is maximized. False if the window is NOT
    /// maximized.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM` or
    /// `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] bool IsMaximized() const;

    /// @brief Hides the window.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void Hide() const;

    /// @brief Shows the window.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void Show() const;

    /// @brief Returns the visibility of the window.
    /// @returns True if the the window is visible. False if the window is
    /// hidden.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM` or
    /// `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] bool IsVisible() const;

    /// @brief Requests focus for the window.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void RequestFocus() const;

    /// @brief Brings focus to the window.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void Focus() const;

    /// @brief Returns the state of the window's focus.
    /// @returns True if the the window has focus. False if the window is does
    /// NOT have focus.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM` or
    /// `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] bool IsFocused() const;

    /// @brief Brings focus to the window when it is shown.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM`,
    /// `GLFW_INVALID_VALUE` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void FocusOnShow() const;

    /// @brief Does NOT bring focus to the window when it is shown.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM`,
    /// `GLFW_INVALID_VALUE` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void DontFocusOnShow() const;

    /// @brief Returns the state of focus brought to the window when it is
    /// shown.
    /// @returns True if the focus is brought to the window when shown. False if
    /// focus is NOT brought to the window when shown.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM` or
    /// `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] bool IsFocusedOnShow() const;

    /// @brief Sets the opacity of the entire window including decorations.
    /// @param Opacity The opacity of the window.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void Opacity(float Opacity) const;

    /// @brief Returns the transparency of the framebuffer.
    /// @returns True if the framebuffer is transparent. False if the
    /// framebuffer is NOT framebuffer.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_ENUM` or
    /// `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    [[nodiscard]] bool IsTransparent() const;

    /// @brief Enters full screen.
    /// @param Full_Screen_Monitor The handle to the full screen monitor.
    /// @param Video_Mode The preferred video mode. If the video mode is
    /// nullptr, the current video mode of the monitor is used.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void EnterFullScreen(const monitor_t& Full_Screen_Monitor,
                         const GLFWvidmode* Video_Mode = nullptr);

    /// @brief Exits full screen.
    /// @param Position The position of the window when it exits full screen. If
    /// set to `NULL_WINDOW_POSITION`, the previous position of the window is
    /// used.
    /// @param Size The size of the window when it exits full screen. If set to
    /// `NULL_WINDOW_SIZE`, the previous size of the window is used.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
    /// @thread_safety Must be called from the main thread.
    void ExitFullScreen(
        const gvw::coordinate<int>& Position = NULL_WINDOW_POSITION,
        const gvw::area<int>& Size = NULL_WINDOW_SIZE);

    /// @todo Declare and define the icon functions.
    /// @todo Declare and define the cursor customization functions.
    /// @todo Declare and define the GLFW Vulkan surface creation function.
    /// @todo Declare and define the GLFW Vulkan functions (currently present in
    /// deprecated GVW code).
};