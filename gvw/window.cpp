// Standard includes

// External includes

// Local includes
#include "window.hpp"
#include "init.hpp"
#include "types.hpp"

namespace gvw {

const glfw_input_event_callbacks NO_INPUT_EVENT_CALLBACKS = {
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
};
const glfw_window_event_callbacks NO_WINDOW_EVENT_CALLBACKS = {
    nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr
};

const window_info DEFAULT_WINDOW_INFO = {};

window_t& window_t::Instance(GLFWwindow* Window)
{
    return *static_cast<window_t*>(glfwGetWindowUserPointer(Window));
}

void window_t::KeyCallback(GLFWwindow* Window,
                           int Key,
                           int Scancode,
                           int Action,
                           int Mods)
{
    Instance(Window).keyEvents.emplace_back(Key, Scancode, Action, Mods);
};

void window_t::CharacterCallback(GLFWwindow* Window, unsigned int Code_Point)
{
    Instance(Window).characterEvents.emplace_back(Code_Point);
}

void window_t::CursorPositionCallback(GLFWwindow* Window,
                                      double X_Position,
                                      double Y_Position)
{
    Instance(Window).cursorPositionEvents.emplace_back(X_Position, Y_Position);
}

void window_t::CursorEnterCallback(GLFWwindow* Window, int Entered)
{
    Instance(Window).cursorEnterEvents.emplace_back(Entered);
}

void window_t::MouseButtonCallback(GLFWwindow* Window,
                                   int Button,
                                   int Action,
                                   int Mods)
{
    Instance(Window).mouseButtonEvents.emplace_back(Button, Action, Mods);
}

void window_t::ScrollCallback(GLFWwindow* Window,
                              double X_Offset,
                              double Y_Offset)
{
    Instance(Window).scrollEvents.emplace_back(X_Offset, Y_Offset);
}

void window_t::FileDropCallback(GLFWwindow* Window,
                                int Count,
                                const char** Paths)
{
    Instance(Window).fileDropEvents.emplace_back(Count, Paths);
}

void window_t::CloseCallback(GLFWwindow* Window)
{
    Instance(Window).closeEvents++;
}

void window_t::SizeCallback(GLFWwindow* Window, int Width, int Height)
{
    Instance(Window).sizeEvents.emplace_back(Width, Height);
}

void window_t::FramebufferSizeCallback(GLFWwindow* Window,
                                       int Width,
                                       int Height)
{
    Instance(Window).framebufferSizeEvents.emplace_back(Width, Height);
}

void window_t::ContentScaleCallback(GLFWwindow* Window,
                                    float XScale,
                                    float YScale)
{
    Instance(Window).contentScaleEvents.emplace_back(XScale, YScale);
}

void window_t::PositionCallback(GLFWwindow* Window,
                                int XPosition,
                                int YPosition)
{
    Instance(Window).positionEvents.emplace_back(XPosition, YPosition);
}

void window_t::IconifyCallback(GLFWwindow* Window, int Iconified)
{
    Instance(Window).iconifyEvents.emplace_back(Iconified);
}

void window_t::MaximizeCallback(GLFWwindow* Window, int Maximized)
{
    Instance(Window).maximizeEvents.emplace_back(Maximized);
}

void window_t::FocusCallback(GLFWwindow* Window, int Focused)
{
    Instance(Window).focusEvents.emplace_back(Focused);
}

void window_t::RefreshCallback(GLFWwindow* Window)
{
    Instance(Window).refreshEvents++;
}

int window_t::WindowAttribute(int Attribute) const
{
    return glfwGetWindowAttrib(this->windowHandle, Attribute);
}

void window_t::WindowAttribute(int Attribute, int Value) const
{
    glfwSetWindowAttrib(this->windowHandle, Attribute, Value);
}

window_t::window_t(const window_info& Window_Info)
{
    // Initialize GVW if it hasn't been initialized yet.
    if (INSTANCE_COUNT == 0) {
        Init(Window_Info.gvwErrorCallback,
             Window_Info.glfwErrorCallback,
             Window_Info.sharedInitHints,
             Window_Info.macosInitHints);
    }
    INSTANCE_COUNT++;

    // Apply window creation hints
    Window_Info.generalHints.Apply();
    Window_Info.framebufferHints.Apply();
    Window_Info.monitorHints.Apply();
    Window_Info.contextHints.Apply();
    Window_Info.macosWindowHints.Apply();
    Window_Info.linuxWindowHints.Apply();

    // Briefly hide the window if an initial position was specified. If this is
    // not done the window may be in the wrong position for one frame (the
    // default position set by the operating system).
    if (Window_Info.position.has_value()) {
        glfwWindowHint(Window_Info.generalHints.visible.HINT, GLFW_FALSE);
    }

    // Create the window
    this->windowHandle = glfwCreateWindow(Window_Info.size.width,
                                          Window_Info.size.height,
                                          Window_Info.title,
                                          Window_Info.fullScreenMonitor,
                                          Window_Info.parentWindow);

    // Link this window object with the underlying GLFW window object
    glfwSetWindowUserPointer(this->windowHandle, this);

    // Set an initial position if one was specified.
    if (Window_Info.position.has_value()) {
        this->Position(Window_Info.position.value());
        // Show the window now that it has been moved to the correct position.
        this->Show();
    }

    /// @todo Audit the utility of this->resetPosition and this->resetSize. They
    /// may be redundant bloat.
    // Set the reset position and size of the window.
    this->resetPosition = this->Position();
    this->resetSize = this->Size();

    // Set callbacks
    this->InputEventCallbacks(Window_Info.inputEventCallbacks);
    this->WindowEventCallbacks(Window_Info.windowEventCallbacks);
}

window_t::~window_t()
{
    // Destroy the window
    glfwDestroyWindow(this->windowHandle);

    INSTANCE_COUNT--;

    // Terminate GVW if there aren't any instances left to manage.
    if (INSTANCE_COUNT == 0) {
        // Terminate GLFW
        glfwTerminate();
    }
}

void window_t::InputEventCallbacks(
    const glfw_input_event_callbacks& InputEventCallbacks)
{
    glfwSetKeyCallback(this->windowHandle, InputEventCallbacks.keyCallback);
    glfwSetCharCallback(this->windowHandle,
                        InputEventCallbacks.characterCallback);
    glfwSetCursorPosCallback(this->windowHandle,
                             InputEventCallbacks.cursorPositionCallback);
    glfwSetCursorEnterCallback(this->windowHandle,
                               InputEventCallbacks.cursorEnterCallback);
    glfwSetMouseButtonCallback(this->windowHandle,
                               InputEventCallbacks.mouseButtonCallback);
    glfwSetScrollCallback(this->windowHandle,
                          InputEventCallbacks.scrollCallback);
    glfwSetDropCallback(this->windowHandle,
                        InputEventCallbacks.fileDropCallback);
}

void window_t::WindowEventCallbacks(
    const glfw_window_event_callbacks& WindowEventCallbacks)
{
    glfwSetWindowCloseCallback(this->windowHandle,
                               WindowEventCallbacks.closeCallback);
    glfwSetWindowSizeCallback(this->windowHandle,
                              WindowEventCallbacks.sizeCallback);
    glfwSetFramebufferSizeCallback(
        this->windowHandle, WindowEventCallbacks.framebufferSizeCallback);
    glfwSetWindowContentScaleCallback(
        this->windowHandle, WindowEventCallbacks.contentScaleCallback);
    glfwSetWindowPosCallback(this->windowHandle,
                             WindowEventCallbacks.positionCallback);
    glfwSetWindowIconifyCallback(this->windowHandle,
                                 WindowEventCallbacks.iconifyCallback);
    glfwSetWindowMaximizeCallback(this->windowHandle,
                                  WindowEventCallbacks.maximizeCallback);
    glfwSetWindowFocusCallback(this->windowHandle,
                               WindowEventCallbacks.focusCallback);
    glfwSetWindowRefreshCallback(this->windowHandle,
                                 WindowEventCallbacks.refreshCallback);
}

GLFWwindow* window_t::Handle() const noexcept
{
    return this->windowHandle;
}

std::vector<key_event>& window_t::KeyEvents() noexcept
{
    return this->keyEvents;
}

std::vector<character_event>& window_t::CharacterEvents() noexcept
{
    return this->characterEvents;
}

std::vector<cursor_position_event>& window_t::CursorPositionEvents() noexcept
{
    return this->cursorPositionEvents;
}

std::vector<cursor_enter_event>& window_t::CursorEnterEvents() noexcept
{
    return this->cursorEnterEvents;
}

std::vector<mouse_button_event>& window_t::MouseButtonEvents() noexcept
{
    return this->mouseButtonEvents;
}

std::vector<scroll_event>& window_t::ScrollEvents() noexcept
{
    return this->scrollEvents;
}

std::vector<file_drop_event>& window_t::FileDropEvents() noexcept
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

void window_t::ClearInputEvents() noexcept
{
    this->ClearKeyEvents();
    this->ClearCharacterEvents();
    this->ClearCursorPositionEvents();
    this->ClearCursorEnterEvents();
    this->ClearMouseButtonEvents();
    this->ClearScrollEvents();
    this->ClearFileDropEvents();
}

[[nodiscard]] size_t window_t::CloseEvents() const noexcept
{
    return this->closeEvents;
}

[[nodiscard]] std::vector<size_event>& window_t::SizeEvents() noexcept
{
    return this->sizeEvents;
}

[[nodiscard]] std::vector<framebuffer_size_event>&
window_t::FramebufferSizeEvents() noexcept
{
    return this->framebufferSizeEvents;
}

[[nodiscard]] std::vector<content_scale_event>&
window_t::ContentScaleEvents() noexcept
{
    return this->contentScaleEvents;
}

[[nodiscard]] std::vector<position_event>& window_t::PositionEvents() noexcept
{
    return this->positionEvents;
}

[[nodiscard]] std::vector<iconify_event>& window_t::IconifyEvents() noexcept
{
    return this->iconifyEvents;
}

[[nodiscard]] std::vector<maximize_event>& window_t::MaximizeEvents() noexcept
{
    return this->maximizeEvents;
}

[[nodiscard]] std::vector<focus_event>& window_t::FocusEvents() noexcept
{
    return this->focusEvents;
}

[[nodiscard]] size_t window_t::RefreshEvents() const noexcept
{
    return this->refreshEvents;
}

void window_t::ClearCloseEvents() noexcept
{
    this->closeEvents = 0;
}

void window_t::ClearSizeEvents() noexcept
{
    this->sizeEvents.clear();
}

void window_t::ClearFramebufferSizeEvents() noexcept
{
    this->framebufferSizeEvents.clear();
}

void window_t::ClearContentScaleEvents() noexcept
{
    this->contentScaleEvents.clear();
}

void window_t::ClearPositionEvents() noexcept
{
    this->positionEvents.clear();
}

void window_t::ClearIconifyEvents() noexcept
{
    this->iconifyEvents.clear();
}

void window_t::ClearMaximizeEvents() noexcept
{
    this->maximizeEvents.clear();
}

void window_t::ClearFocusEvents() noexcept
{
    this->focusEvents.clear();
}

void window_t::ClearRefreshEvents() noexcept
{
    this->refreshEvents = 0;
}

void window_t::ClearWindowEvents() noexcept
{
    this->ClearCloseEvents();
    this->ClearSizeEvents();
    this->ClearFramebufferSizeEvents();
    this->ClearContentScaleEvents();
    this->ClearPositionEvents();
    this->ClearIconifyEvents();
    this->ClearMaximizeEvents();
    this->ClearFocusEvents();
    this->ClearRefreshEvents();
}

void window_t::ClearEvents() noexcept
{
    this->ClearInputEvents();
    this->ClearWindowEvents();
}

bool window_t::ShouldClose() const
{
    return (glfwWindowShouldClose(this->windowHandle) != GLFW_FALSE);
}

void window_t::ShouldClose(bool State) const
{
    glfwSetWindowShouldClose(this->windowHandle, static_cast<int>(State));
}

area<int> window_t::Size() const
{
    area<int> size = { 0, 0 };
    glfwGetWindowSize(this->windowHandle, &size.width, &size.height);
    return size;
}

void window_t::Size(const area<int>& Size) const
{
    glfwSetWindowSize(this->windowHandle, Size.width, Size.height);
}

area<int> window_t::SizeInPixels() const
{
    area<int> size = { 0, 0 };
    glfwGetFramebufferSize(this->windowHandle, &size.width, &size.height);
    return size;
}

coordinate<int> window_t::Position() const
{
    coordinate<int> position = { 0, 0 };
    glfwGetWindowPos(this->windowHandle, &position.x, &position.y);
    return position;
}

void window_t::Position(const coordinate<int>& Position) const
{
    glfwSetWindowPos(this->windowHandle, Position.x, Position.y);
}

coordinate<float> window_t::ContentScale() const
{
    coordinate<float> contentScale = { 0.0F, 0.0F };
    glfwGetWindowContentScale(
        this->windowHandle, &contentScale.x, &contentScale.y);
    return contentScale;
}

void window_t::SizeLimits(const area<int>& Minimum_Size,
                          const area<int>& Maximum_Size) const
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
                         Full_Screen_Monitor.Handle(),
                         GLFW_FALSE,
                         GLFW_FALSE,
                         videoMode->width,
                         videoMode->height,
                         videoMode->refreshRate);
}

void window_t::ExitFullScreen(const std::optional<coordinate<int>>& Position,
                              const std::optional<area<int>>& Size)
{
    /// @todo Resolve this->resetPosition and this->resetSize.
    const coordinate<int>& position =
        Position.value_or(this->resetPosition.value());
    const area<int>& size = Size.value_or(this->resetSize.value());
    glfwSetWindowMonitor(this->windowHandle,
                         nullptr,
                         position.x,
                         position.y,
                         size.width,
                         size.height,
                         GLFW_FALSE);
}

} // namespace gvw