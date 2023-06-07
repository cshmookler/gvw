// Standard includes
#include <limits>

// External includes

// Local includes
#include "init.hpp"
#include "types.hpp"
#include "window.hpp"

// NOLINTBEGIN
std::vector<gvw::joystick_event> window_t::JOYSTICK_EVENTS;
// NOLINTEND

const gvw::error window_t::ERROR_GLFW_INVALID_POSITION = {
    0x00200001,
    "\"Invalid window position\""
};
const gvw::error window_t::ERROR_GLFW_INVALID_SIZE = {
    0x00200002,
    "\"Invalid window size\""
};

const gvw::glfw_general_hints window_t::DEFAULT_GLFW_GENERAL_HINTS = {};
const gvw::glfw_framebuffer_hints window_t::DEFAULT_GLFW_FRAMEBUFFER_HINTS = {};
const gvw::glfw_monitor_hints window_t::DEFAULT_GLFW_MONITOR_HINTS = {};
const gvw::glfw_context_hints window_t::DEFAULT_GLFW_CONTEXT_HINTS = {};
const gvw::glfw_macos_window_hints
    window_t::DEFAULT_GLFW_MACOS_WINDOW_HINTS = {};
const gvw::glfw_linux_window_hints
    window_t::DEFAULT_GLFW_LINUX_WINDOW_HINTS = {};

const gvw::coordinate<int> window_t::DEFAULT_WINDOW_POSITION = { 0, 0 };
const gvw::area<int> window_t::DEFAULT_WINDOW_SIZE = { 640, 360 };
const gvw::area<int> window_t::DEFAULT_MINIMUM_WINDOW_SIZE = { GLFW_DONT_CARE,
                                                               GLFW_DONT_CARE };
const gvw::area<int> window_t::DEFAULT_MAXIMUM_WINDOW_SIZE = { GLFW_DONT_CARE,
                                                               GLFW_DONT_CARE };
const gvw::coordinate<int> window_t::NULL_WINDOW_POSITION = {
    std::numeric_limits<int>::min(),
    std::numeric_limits<int>::min()
};
const gvw::area<int> window_t::NULL_WINDOW_SIZE = { 0, 0 };

void window_t::JoystickCallback(int JID, int Event)
{
    JOYSTICK_EVENTS.emplace_back(JID, Event);
}

std::vector<gvw::joystick_event>& window_t::JoystickEvents()
{
    return JOYSTICK_EVENTS;
}

void window_t::ClearJoystickEvents()
{
    JOYSTICK_EVENTS.clear();
}

window_t& window_t::WindowInstance(GLFWwindow* Window)
{
    return *static_cast<window_t*>(glfwGetWindowUserPointer(Window));
}

void window_t::KeyCallback(GLFWwindow* Window,
                           int Key,
                           int Scancode,
                           int Action,
                           int Mods)
{
    WindowInstance(Window).keyEvents.emplace_back(Key, Scancode, Action, Mods);
};

void window_t::CharacterCallback(GLFWwindow* Window, unsigned int Code_Point)
{
    WindowInstance(Window).characterEvents.emplace_back(Code_Point);
}

void window_t::CursorPositionCallback(GLFWwindow* Window,
                                      double X_Position,
                                      double Y_Position)
{
    WindowInstance(Window).cursorPositionEvents.emplace_back(X_Position,
                                                             Y_Position);
}

void window_t::CursorEnterCallback(GLFWwindow* Window, int Entered)
{
    WindowInstance(Window).cursorEnterEvents.emplace_back(Entered);
}

void window_t::MouseButtonCallback(GLFWwindow* Window,
                                   int Button,
                                   int Action,
                                   int Mods)
{
    WindowInstance(Window).mouseButtonEvents.emplace_back(Button, Action, Mods);
}

void window_t::ScrollCallback(GLFWwindow* Window,
                              double X_Offset,
                              double Y_Offset)
{
    WindowInstance(Window).scrollEvents.emplace_back(X_Offset, Y_Offset);
}

void window_t::FileDropCallback(GLFWwindow* Window,
                                int Count,
                                const char** Paths)
{
    WindowInstance(Window).fileDropEvents.emplace_back(Count, Paths);
}

window_t::window_t(const gvw::coordinate<int>& Position,
                   const gvw::area<int>& Size,
                   const char* Title,
                   GLFWmonitor* Full_Screen_Monitor,
                   GLFWwindow* Parent_Window,
                   const gvw::glfw_general_hints& General_Hints,
                   const gvw::glfw_framebuffer_hints& Framebuffer_Hints,
                   const gvw::glfw_monitor_hints& Monitor_Hints,
                   const gvw::glfw_context_hints& Context_Hints,
                   const gvw::glfw_macos_window_hints& MacOS_Window_Hints,
                   const gvw::glfw_linux_window_hints& Linux_Window_Hints,
                   GLFWerrorfun GLFW_Error_Callback,
                   gvw::gvw_error_callback GVW_Error_Callback,
                   const gvw::glfw_shared_init_hints& Shared_Init_Hints,
                   const gvw::glfw_macos_init_hints& MacOS_Init_Hints)
{
    if (Position == NULL_WINDOW_POSITION) {
        GVW_Error_Callback(ERROR_GLFW_INVALID_POSITION);
    }
    if (Size == NULL_WINDOW_SIZE) {
        GVW_Error_Callback(ERROR_GLFW_INVALID_SIZE);
    }

    // Initialize GVW if it hasn't been initialized yet.
    if (gvw::INSTANCE_COUNT == 0) {
        gvw::Init(GVW_Error_Callback,
                  GLFW_Error_Callback,
                  Shared_Init_Hints,
                  MacOS_Init_Hints);
    }
    gvw::INSTANCE_COUNT++;

    // Apply window creation hints
    General_Hints.Apply();
    Framebuffer_Hints.Apply();
    Monitor_Hints.Apply();
    Context_Hints.Apply();
    MacOS_Window_Hints.Apply();
    Linux_Window_Hints.Apply();

    // Apply window creation hint overrides
    glfwWindowHint(General_Hints.visible.HINT, GLFW_FALSE);

    // Create the window
    this->windowHandle = glfwCreateWindow(
        Size.width, Size.height, Title, Full_Screen_Monitor, Parent_Window);

    // Link this window object with the underlying GLFW window object
    glfwSetWindowUserPointer(this->windowHandle, this);

    // Set position
    if (Position != DEFAULT_WINDOW_POSITION) {
        glfwSetWindowPos(this->windowHandle, Position.x, Position.y);
        this->Position(Position);
    }
    this->resetPosition = this->Position();
    this->resetSize = this->Size();

    // Remove window creation hint overrides
    this->Show();

    // Set input callbacks
    glfwSetKeyCallback(this->windowHandle, KeyCallback);
    glfwSetCharCallback(this->windowHandle, CharacterCallback);
    glfwSetCursorPosCallback(this->windowHandle, CursorPositionCallback);
    glfwSetCursorEnterCallback(this->windowHandle, CursorEnterCallback);
    glfwSetMouseButtonCallback(this->windowHandle, MouseButtonCallback);
    glfwSetScrollCallback(this->windowHandle, ScrollCallback);
    glfwSetDropCallback(this->windowHandle, FileDropCallback);
}

void window_t::InitInputBuffers()
{
    // glfwSetKeyCallback(GLFWwindow* window, GLFWkeyfun callback)
}

int window_t::WindowAttribute(int Attribute) const
{
    return glfwGetWindowAttrib(this->windowHandle, Attribute);
}

void window_t::WindowAttribute(int Attribute, int Value) const
{
    glfwSetWindowAttrib(this->windowHandle, Attribute, Value);
}

window_t::~window_t()
{
    // Destroy the window
    glfwDestroyWindow(this->windowHandle);

    gvw::INSTANCE_COUNT--;

    // Terminate GVW if there aren't any instances left to manage.
    if (gvw::INSTANCE_COUNT == 0) {
        // Terminate GLFW
        glfwTerminate();
    }
}

GLFWwindow* window_t::WindowHandle() const noexcept
{
    return this->windowHandle;
}

std::vector<gvw::key_event>& window_t::KeyEvents() noexcept
{
    return this->keyEvents;
}

std::vector<gvw::character_event>& window_t::CharacterEvents() noexcept
{
    return this->characterEvents;
}

std::vector<gvw::cursor_position_event>&
window_t::CursorPositionEvents() noexcept
{
    return this->cursorPositionEvents;
}

std::vector<gvw::cursor_enter_event>& window_t::CursorEnterEvents() noexcept
{
    return this->cursorEnterEvents;
}

std::vector<gvw::mouse_button_event>& window_t::MouseButtonEvents() noexcept
{
    return this->mouseButtonEvents;
}

std::vector<gvw::scroll_event>& window_t::ScrollEvents() noexcept
{
    return this->scrollEvents;
}

std::vector<gvw::file_drop_event>& window_t::FileDropEvents() noexcept
{
    return this->fileDropEvents;
}

void window_t::ClearKeyEvents() noexcept
{
    this->keyEvents.clear();
}

void window_t::ClearCharacterEvents() noexcept
{
    this->characterEvents.clear();
}

void window_t::ClearCursorPositionEvents() noexcept
{
    this->cursorPositionEvents.clear();
}

void window_t::ClearCursorEnterEvents() noexcept
{
    this->cursorEnterEvents.clear();
}

void window_t::ClearMouseButtonEvents() noexcept
{
    this->mouseButtonEvents.clear();
}

void window_t::ClearScrollEvents() noexcept
{
    this->scrollEvents.clear();
}

void window_t::ClearFileDropEvents() noexcept
{
    this->fileDropEvents.clear();
}

void window_t::ClearEvents() noexcept
{
    ClearKeyEvents();
    ClearCharacterEvents();
    ClearCursorPositionEvents();
    ClearCursorEnterEvents();
    ClearMouseButtonEvents();
    ClearScrollEvents();
    ClearFileDropEvents();
}

bool window_t::ShouldClose() const
{
    return (glfwWindowShouldClose(this->windowHandle) != GLFW_FALSE);
}

void window_t::ShouldClose(bool State) const
{
    glfwSetWindowShouldClose(this->windowHandle, static_cast<int>(State));
}

gvw::area<int> window_t::Size() const
{
    gvw::area<int> size = { 0, 0 };
    glfwGetWindowSize(this->windowHandle, &size.width, &size.height);
    return size;
}

void window_t::Size(const gvw::area<int>& Size) const
{
    glfwSetWindowSize(this->windowHandle, Size.width, Size.height);
}

gvw::area<int> window_t::SizeInPixels() const
{
    gvw::area<int> size = { 0, 0 };
    glfwGetFramebufferSize(this->windowHandle, &size.width, &size.height);
    return size;
}

gvw::coordinate<int> window_t::Position() const
{
    gvw::coordinate<int> position = { 0, 0 };
    glfwGetWindowPos(this->windowHandle, &position.x, &position.y);
    return position;
}

void window_t::Position(const gvw::coordinate<int>& Position) const
{
    glfwSetWindowPos(this->windowHandle, Position.x, Position.y);
}

gvw::coordinate<float> window_t::ContentScale() const
{
    gvw::coordinate<float> contentScale = { 0.0F, 0.0F };
    glfwGetWindowContentScale(
        this->windowHandle, &contentScale.x, &contentScale.y);
    return contentScale;
}

void window_t::SizeLimits(const gvw::area<int>& Minimum_Size,
                          const gvw::area<int>& Maximum_Size) const
{
    glfwSetWindowSizeLimits(this->windowHandle,
                            Minimum_Size.width,
                            Minimum_Size.height,
                            Maximum_Size.width,
                            Maximum_Size.height);
}

void window_t::AspectRatio(int Numerator, int Denominator) const
{
    glfwSetWindowAspectRatio(this->windowHandle, Numerator, Denominator);
}

bool window_t::IsCursorHovering() const
{
    return static_cast<bool>(this->WindowAttribute(GLFW_HOVERED));
}

bool window_t::IsResizable() const
{
    return static_cast<bool>(this->WindowAttribute(GLFW_RESIZABLE));
}

void window_t::Decorate() const
{
    this->WindowAttribute(GLFW_DECORATED, GLFW_TRUE);
}

void window_t::Undecorate() const
{
    this->WindowAttribute(GLFW_DECORATED, GLFW_FALSE);
}

bool window_t::IsDecorated() const
{
    return static_cast<bool>(this->WindowAttribute(GLFW_DECORATED));
}

void window_t::MinimizeOnFocusLoss() const
{
    this->WindowAttribute(GLFW_AUTO_ICONIFY, GLFW_TRUE);
}

void window_t::DontMinimizeOnFocusLoss() const
{
    this->WindowAttribute(GLFW_AUTO_ICONIFY, GLFW_FALSE);
}

bool window_t::IsMinimizedOnFocusLoss() const
{
    return static_cast<bool>(this->WindowAttribute(GLFW_AUTO_ICONIFY));
}

void window_t::AlwaysOnTop() const
{
    this->WindowAttribute(GLFW_FLOATING, GLFW_TRUE);
}

void window_t::NotAlwaysOnTop() const
{
    this->WindowAttribute(GLFW_FLOATING, GLFW_FALSE);
}

bool window_t::IsAlwaysOnTop() const
{
    return static_cast<bool>(this->WindowAttribute(GLFW_FLOATING));
}

void window_t::Restore() const
{
    glfwRestoreWindow(this->windowHandle);
}

void window_t::Minimize() const
{
    glfwIconifyWindow(this->windowHandle);
}

bool window_t::IsMinimized() const
{
    return static_cast<bool>(this->WindowAttribute(GLFW_ICONIFIED));
}

void window_t::Maximize() const
{
    glfwMaximizeWindow(this->windowHandle);
}

bool window_t::IsMaximized() const
{
    return static_cast<bool>(this->WindowAttribute(GLFW_MAXIMIZED));
}

void window_t::Hide() const
{
    glfwHideWindow(this->windowHandle);
}

void window_t::Show() const
{
    glfwShowWindow(this->windowHandle);
}

bool window_t::IsVisible() const
{
    return static_cast<bool>(this->WindowAttribute(GLFW_VISIBLE));
}

void window_t::RequestFocus() const
{
    glfwRequestWindowAttention(this->windowHandle);
}

void window_t::Focus() const
{
    glfwFocusWindow(this->windowHandle);
}

bool window_t::IsFocused() const
{
    return static_cast<bool>(this->WindowAttribute(GLFW_FOCUSED));
}

void window_t::FocusOnShow() const
{
    this->WindowAttribute(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
}

void window_t::DontFocusOnShow() const
{
    this->WindowAttribute(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
}

bool window_t::IsFocusedOnShow() const
{
    return static_cast<bool>(this->WindowAttribute(GLFW_FOCUS_ON_SHOW));
}

void window_t::Opacity(float Opacity) const
{
    glfwSetWindowOpacity(this->windowHandle, Opacity);
}

bool window_t::IsTransparent() const
{
    return static_cast<bool>(
        this->WindowAttribute(GLFW_TRANSPARENT_FRAMEBUFFER));
}

void window_t::EnterFullScreen(const monitor_t& Full_Screen_Monitor,
                               const GLFWvidmode* Video_Mode)
{
    this->resetPosition = this->Position();
    this->resetSize = this->Size();
    const GLFWvidmode* videoMode =
        Video_Mode == nullptr ? Full_Screen_Monitor.VideoMode() : Video_Mode;
    glfwSetWindowMonitor(this->windowHandle,
                         Full_Screen_Monitor.MonitorHandle(),
                         GLFW_FALSE,
                         GLFW_FALSE,
                         videoMode->width,
                         videoMode->height,
                         videoMode->refreshRate);
}

void window_t::ExitFullScreen(const gvw::coordinate<int>& Position,
                              const gvw::area<int>& Size)
{
    gvw::coordinate<int> position =
        Position == NULL_WINDOW_POSITION ? this->resetPosition : Position;
    gvw::area<int> size = Size == NULL_WINDOW_SIZE ? this->resetSize : Size;
    glfwSetWindowMonitor(this->windowHandle,
                         nullptr,
                         position.x,
                         position.y,
                         size.width,
                         size.height,
                         GLFW_FALSE);
}