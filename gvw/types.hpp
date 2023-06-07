#pragma once

/**
 * @file types.hpp
 * @author Caden Shmookler (cadenshmookler@gmail.com)
 * @brief Custom type declarations.
 * @date 2023-05-23
 */

// Standard includes
#include <array>
#include <functional>

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Local includes

namespace gvw {

//////////////////////////////////////////////////
//            Structs and Other Types           //
//////////////////////////////////////////////////

/// @brief Error code and human readable description
struct error
{
    int code;
    const char* description;
};

/// @brief Signature for GVW error callbacks.
using gvw_error_callback = void(error Error);

/// @brief Version major, minor, and revision.
struct version
{
    int major;
    int minor;
    int revision;
};

/// @brief GLFW hint ID and default value.
/// @tparam T The value type of the hint. Almost always int. Sometimes
/// const char*.
template<typename T>
struct glfw_hint // NOLINT
{
    const int HINT; // NOLINT
    T value;

    glfw_hint(int Hint, T Value);
    glfw_hint(const glfw_hint<T>& GLFW_Hint);
    glfw_hint(glfw_hint<T>&& GLFW_Hint) noexcept;
    ~glfw_hint() = default;
    glfw_hint<T>& operator=(T Value);
};

/// @brief The base struct of the *_init_hints and *_hints structs.
/// @tparam IntHints The number of hints with cooresponding integer values
/// (int).
/// @tparam StringHints The number of hints with cooresponding string
/// values (const char*).
template<size_t IntHints, size_t StringHints>
struct glfw_hints
{
    std::array<glfw_hint<int>, IntHints> intHints;
    std::array<glfw_hint<const char*>, StringHints> stringHints;

    const std::function<void(int, int)> INT_HINT_FUNC;            // NOLINT
    const std::function<void(int, const char*)> STRING_HINT_FUNC; // NOLINT

    glfw_hints(std::function<void(int, int)>&& Int_Hint_Func,
               std::function<void(int, const char*)>&& String_Hint_Func,
               std::array<glfw_hint<int>, IntHints>&& Int_Hints,
               std::array<glfw_hint<const char*>, StringHints>&& String_Hints);
    glfw_hints(const glfw_hints&) = delete;
    glfw_hints(glfw_hints&&) noexcept = delete;
    ~glfw_hints() = default;
    glfw_hints& operator=(const glfw_hints& GLFW_Hints);
    glfw_hints& operator=(glfw_hints&& GLFW_Hints) noexcept;

    /// @brief Apply int and string hints using `INT_HINT_FUNC` and
    /// `STRING_HINT_FUNC`.
    /// @gvw_errors None.
    /// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_INVALID_ENUM`.
    /// @thread_safety Must be called from the main thread.
    void Apply() const;
};

/// @brief Shared initialization hints for GLFW.
struct glfw_shared_init_hints : public glfw_hints<1, 0>
{
    // NOLINTBEGIN
    glfw_hint<int>& joystickHatButtons = intHints.at(0);
    // NOLINTEND

    glfw_shared_init_hints(int Joystick_Hat_Buttons = GLFW_TRUE);
};

/// @brief MacOS specific initialization hints for GLFW.
struct glfw_macos_init_hints : public glfw_hints<2, 0>
{
    // NOLINTBEGIN
    glfw_hint<int>& cocoaChdirResources = intHints.at(0);
    glfw_hint<int>& cocoaMenubar = intHints.at(1);
    // NOLINTEND

    glfw_macos_init_hints(int Cocoa_CHDIR_Resources = GLFW_TRUE,
                          int Cocoa_Menubar = GLFW_TRUE);
};

/// @brief General window hints for GLFW window creation.
struct glfw_general_hints : public glfw_hints<11, 0> // NOLINT
{
    // NOLINTBEGIN
    glfw_hint<int>& resizable = intHints.at(0);
    glfw_hint<int>& visible = intHints.at(1);
    glfw_hint<int>& decorated = intHints.at(2);
    glfw_hint<int>& focused = intHints.at(3);
    glfw_hint<int>& autoIconify = intHints.at(4);
    glfw_hint<int>& floating = intHints.at(5);
    glfw_hint<int>& maximized = intHints.at(6);
    glfw_hint<int>& centerCursor = intHints.at(7);
    glfw_hint<int>& transparentFramebuffer = intHints.at(8);
    glfw_hint<int>& focusOnShow = intHints.at(9);
    glfw_hint<int>& scaleToMonitor = intHints.at(10);
    // NOLINTEND

    glfw_general_hints(int Resizable = GLFW_TRUE,
                       int Visible = GLFW_TRUE,
                       int Decorated = GLFW_TRUE,
                       int Focused = GLFW_TRUE,
                       int Auto_Iconify = GLFW_TRUE,
                       int Floating = GLFW_FALSE,
                       int Maximized = GLFW_FALSE,
                       int Center_Cursor = GLFW_TRUE,
                       int Transparent_Framebuffer = GLFW_FALSE,
                       int Focus_On_Show = GLFW_TRUE,
                       int Scale_To_Monitor = GLFW_FALSE);
};

/// @brief Framebuffer hints for GLFW window creation.
struct glfw_framebuffer_hints : public glfw_hints<11, 0> // NOLINT
{
    // NOLINTBEGIN
    glfw_hint<int>& redBits = intHints.at(0);
    glfw_hint<int>& greenBits = intHints.at(1);
    glfw_hint<int>& blueBits = intHints.at(2);
    glfw_hint<int>& alphaBits = intHints.at(3);
    glfw_hint<int>& depthBits = intHints.at(4);
    glfw_hint<int>& stencilBits = intHints.at(5);
    glfw_hint<int>& auxBuffers = intHints.at(6);
    glfw_hint<int>& stereo = intHints.at(7);
    glfw_hint<int>& samples = intHints.at(8);
    glfw_hint<int>& srgbCapable = intHints.at(9);
    glfw_hint<int>& doubleBuffer = intHints.at(10);
    // NOLINTEND

    glfw_framebuffer_hints(int Red_Bits = 8,     // NOLINT
                           int Green_Bits = 8,   // NOLINT
                           int Blue_Bits = 8,    // NOLINT
                           int Alpha_Bits = 8,   // NOLINT
                           int Depth_Bits = 24,  // NOLINT
                           int Stencil_Bits = 8, // NOLINT
                           int AUX_Buffers = 0,
                           int Stereo = GLFW_FALSE,
                           int Samples = 0,
                           int SRGB_Capable = GLFW_FALSE,
                           int Double_Buffer = GLFW_TRUE);
};

/// @brief Monitor hints for GLFW window creation.
struct glfw_monitor_hints : public glfw_hints<1, 0>
{
    // NOLINTBEGIN
    glfw_hint<int>& refreshRate = intHints.at(0);
    // NOLINTEND

    glfw_monitor_hints(int Refresh_Rate = GLFW_DONT_CARE);
};

/// @brief Context hints for GLFW window creation.
struct glfw_context_hints : public glfw_hints<10, 0> // NOLINT
{
    // NOLINTBEGIN
    glfw_hint<int>& clientApi = intHints.at(0);
    glfw_hint<int>& contextCreationApi = intHints.at(1);
    glfw_hint<int>& contextVersionMajor = intHints.at(2);
    glfw_hint<int>& contextVersionMinor = intHints.at(3);
    glfw_hint<int>& openglForwardCompatible = intHints.at(4);
    glfw_hint<int>& openglDebugContext = intHints.at(5);
    glfw_hint<int>& openglProfile = intHints.at(6);
    glfw_hint<int>& contextRobustness = intHints.at(7);
    glfw_hint<int>& contextReleaseBehavior = intHints.at(8);
    glfw_hint<int>& contextNoError = intHints.at(9);
    // NOLINTEND

    glfw_context_hints(int Client_API = GLFW_NO_API,
                       int Context_Creation_API = GLFW_NATIVE_CONTEXT_API,
                       int Context_Version_Major = 3,
                       int Context_Version_Minor = 6, // NOLINT
                       int OpenGL_Forward_Compatible = GLFW_FALSE,
                       int OpenGL_Debug_Context = GLFW_FALSE,
                       int OpenGL_Profile = GLFW_OPENGL_CORE_PROFILE,
                       int Context_Robustness = GLFW_NO_ROBUSTNESS,
                       int Context_Release_Behavior = GLFW_ANY_RELEASE_BEHAVIOR,
                       int Context_No_Error = GLFW_FALSE);
};

/// @brief MacOS specific window hints for GLFW window creation.
struct glfw_macos_window_hints : public glfw_hints<2, 1>
{
    // NOLINTBEGIN
    glfw_hint<int>& cocoaRetinaFramebuffer = intHints.at(0);
    glfw_hint<int>& cocoaGraphicsSwitching = intHints.at(1);
    glfw_hint<const char*>& cocoaFrameName = stringHints.at(0);
    // NOLINTEND

    glfw_macos_window_hints(int Cocoa_Retina_Framebuffer = GLFW_TRUE,
                            int Cocoa_Graphics_Switching = GLFW_FALSE,
                            const char* Cocoa_Frame_Name = "");
};

/// @brief Linux specific window hints for GLFW window creation.
struct glfw_linux_window_hints : public glfw_hints<0, 2>
{
    // NOLINTBEGIN
    glfw_hint<const char*>& x11ClassName = stringHints.at(0);
    glfw_hint<const char*>& x11InstanceName = stringHints.at(1);
    // NOLINTEND

    glfw_linux_window_hints(const char* X11_Class_Name = "",
                            const char* X11_Instance_Name = "");
};

// Forward declaration of the `area` object.
template<typename T>
struct area;

/// @brief An x and y value pair. Structurally identical to `gvw::area`.
/// @tparam T The type of the x and y values.
template<typename T>
struct coordinate
{
    T x;
    T y;

    coordinate() = default;
    coordinate(T X, T Y); // NOLINT
    coordinate(const coordinate<T>& Coordinate);
    coordinate(const area<T>& Area);
    coordinate(coordinate<T>&& Coordinate) noexcept;
    coordinate(area<T>&& Area) noexcept;
    ~coordinate() = default;
    coordinate<T>& operator=(const coordinate<T>& Coordinate);
    coordinate<T>& operator=(const area<T>& Area);
    coordinate<T>& operator=(coordinate<T>&& Coordinate) noexcept;
    coordinate<T>& operator=(area<T>&& Area) noexcept;

    bool operator==(const coordinate<T>& Coordinate) const;
    bool operator!=(const coordinate<T>& Coordinate) const;
};

/// @brief A width and height value pair. Structurally identical to
/// `gvw::coordinate`.
/// @tparam T The type of the width and height values.
template<typename T>
struct area
{
    T width;
    T height;

    area() = default;
    area(T Width, T Height);
    area(const area<T>& Area);
    area(const coordinate<T>& Coordinate);
    area(area<T>&& Area) noexcept;
    area(coordinate<T>&& Coordinate) noexcept;
    ~area() = default;
    area<T>& operator=(const area<T>& Area);
    area<T>& operator=(const coordinate<T>& Coordinate);
    area<T>& operator=(area<T>&& Area) noexcept;
    area<T>& operator=(coordinate<T>&& Coordinate) noexcept;

    bool operator==(const area<T>& Area) const;
    bool operator!=(const area<T>& Area) const;
};

/// @brief The key, scancode, action, and mods of a key event.
struct key_event
{
    int key;
    int scancode;
    int action;
    int mods;

    key_event(int Key, int Scancode, int Action, int Mods);
};

/// @brief A character event stored as an unsigned int. Can be interpreted
/// as a UTF-8, UTF-16, or UTF-32 character.
using character_event = unsigned int;

/// @brief A cursor position event stored as a `coordinate` of type double.
using cursor_position_event = coordinate<double>;

/// @brief A cursor enter/leave event stored as an integer. Can be
/// interpreted as a boolean.
using cursor_enter_event = int;

/// @brief The button, action, and mods of a mouse button event.
struct mouse_button_event
{
    int button;
    int action;
    int mods;

    mouse_button_event(int Button, int Action, int Mods);
};

/// @brief A scroll event stored as a `coordinate` of type double.
using scroll_event = coordinate<double>;

/// @brief File count and file paths from a file drop event.
struct file_drop_event
{
    int count;
    const char** paths;

    file_drop_event(int Count, const char** Paths);
};

/// @brief Joystick ID and event type of a joystick event.
struct joystick_event
{
    int jid;
    int event;

    joystick_event(int JID, int Event);
};

} // namespace gvw