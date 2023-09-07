#pragma once

/**
 * @file gvw.ipp
 * @author Caden Shmookler (cadenshmookler@gmail.com)
 * @brief Public type definitions.
 * @date 2023-05-23
 */

// Standard includes
#include <fstream>

// Local includes
#include "gvw.hpp"

namespace gvw {

template<typename T>
struct coordinate
{
    T x;
    T y;

    coordinate() = default;
    coordinate(T X, T Y) // NOLINT
        : x(X)
        , y(Y)
    {
    }
    coordinate(const coordinate<T>&) = default;
    coordinate(coordinate<T>&&) noexcept = default;
    coordinate<T>& operator=(const coordinate<T>&) = default;
    coordinate<T>& operator=(coordinate<T>&&) noexcept = default;
    ~coordinate() = default;

    coordinate<T>& operator=(const area<T>& Area)
    {
        this->x = Area.width;
        this->y = Area.height;
        return *this;
    }
    coordinate<T>& operator=(area<T>&& Area) noexcept
    {
        this->x = std::move(Area.width);
        this->y = std::move(Area.height);
        return *this;
    }
    coordinate(const area<T>& Area)
        : x(Area.width)
        , y(Area.height)
    {
    }
    coordinate(area<T>&& Area) noexcept
        : x(std::move(Area.width))
        , y(std::move(Area.height))
    {
    }

    bool operator==(const coordinate<T>& Coordinate) const
    {
        return (this->x == Coordinate.x) && (this->y == Coordinate.y);
    }
    bool operator!=(const coordinate<T>& Coordinate) const
    {
        return (this->x != Coordinate.x) || (this->y != Coordinate.y);
    }

    coordinate<T> operator+(T Type) const
    {
        return { this->x + Type, this->y + Type };
    }
    coordinate<T> operator-(T Type) const
    {
        return { this->x - Type, this->y - Type };
    }
    coordinate<T> operator*(T Type) const
    {
        return { this->x * Type, this->y * Type };
    }
    coordinate<T> operator/(T Type) const
    {
        return { this->x / Type, this->y / Type };
    }
    coordinate<T> operator%(T Type) const
    {
        return { this->x % Type, this->y % Type };
    }

    coordinate<T>& operator+=(T Type)
    {
        this->x += Type;
        this->y += Type;
        return *this;
    }
    coordinate<T>& operator-=(T Type)
    {
        this->x -= Type;
        this->y -= Type;
        return *this;
    }
    coordinate<T>& operator*=(T Type)
    {
        this->x *= Type;
        this->y *= Type;
        return *this;
    }
    coordinate<T>& operator/=(T Type)
    {
        this->x /= Type;
        this->y /= Type;
        return *this;
    }
    coordinate<T>& operator%=(T Type)
    {
        this->x %= Type;
        this->y %= Type;
        return *this;
    }
};

template<typename T>
struct area
{
    T width;
    T height;

    area() = default;
    area(T Width, T Height)
        : width(Width)
        , height(Height)
    {
    }
    area(const area<T>&) = default;
    area(area<T>&&) noexcept = default;
    area<T>& operator=(const area<T>&) = default;
    area<T>& operator=(area<T>&&) noexcept = default;
    ~area() = default;

    area(const coordinate<T>& Coordinate)
        : width(Coordinate.x)
        , height(Coordinate.y)
    {
    }
    area(coordinate<T>&& Coordinate) noexcept
        : width(std::move(Coordinate.x))
        , height(std::move(Coordinate.y))
    {
    }
    area<T>& operator=(const coordinate<T>& Coordinate)
    {
        this->width = Coordinate.x;
        this->height = Coordinate.y;
        return *this;
    }
    area<T>& operator=(coordinate<T>&& Coordinate) noexcept
    {
        this->width = std::move(Coordinate.x);
        this->height = std::move(Coordinate.y);
        return *this;
    }

    bool operator==(const area<T>& Area) const
    {
        return (this->width == Area.width) && (this->height == Area.height);
    }
    bool operator!=(const area<T>& Area) const
    {
        return (this->width != Area.width) || (this->height != Area.height);
    }

    area<T> operator+(T Type) const
    {
        return { this->width + Type, this->height + Type };
    }
    area<T> operator-(T Type) const
    {
        return { this->width - Type, this->height - Type };
    }
    area<T> operator*(T Type) const
    {
        return { this->width * Type, this->height * Type };
    }
    area<T> operator/(T Type) const
    {
        return { this->width / Type, this->height / Type };
    }
    area<T> operator%(T Type) const
    {
        return { this->width % Type, this->height % Type };
    }

    area<T>& operator+=(T Type)
    {
        this->width += Type;
        this->height += Type;
        return *this;
    }
    area<T>& operator-=(T Type)
    {
        this->width -= Type;
        this->height -= Type;
        return *this;
    }
    area<T>& operator*=(T Type)
    {
        this->width *= Type;
        this->height *= Type;
        return *this;
    }
    area<T>& operator/=(T Type)
    {
        this->width /= Type;
        this->height /= Type;
        return *this;
    }
    area<T>& operator%=(T Type)
    {
        this->width %= Type;
        this->height %= Type;
        return *this;
    }
};

struct version
{
    int major;
    int minor;
    int revision;

    operator std::string() const
    {
        return std::to_string(major) + "." + std::to_string(minor) + "." +
               std::to_string(revision);
    }
};

bool operator==(const version& Lhs, const version& Rhs);
bool operator!=(const version& Lhs, const version& Rhs);
bool operator<(const version& Lhs, const version& Rhs);
bool operator>(const version& Lhs, const version& Rhs);
bool operator<=(const version& Lhs, const version& Rhs);
bool operator>=(const version& Lhs, const version& Rhs);

class creation_hint_bool
{
    friend instance_creation_hints;
    friend window_creation_hints;

  public:
    ////////////////////////////////////////////////////////////////////////////
    ///                Constructors, Operators, and Destructor               ///
    ////////////////////////////////////////////////////////////////////////////

    creation_hint_bool(bool Boolean);
    creation_hint_bool(const creation_hint_bool&) = default;
    creation_hint_bool(creation_hint_bool&&) noexcept = default;
    creation_hint_bool& operator=(const creation_hint_bool&) = default;
    creation_hint_bool& operator=(creation_hint_bool&&) noexcept = default;
    creation_hint_bool& operator=(bool Boolean);
    ~creation_hint_bool() = default;

  private:
    ////////////////////////////////////////////////////////////////////////////
    ///                           Private Variables                          ///
    ////////////////////////////////////////////////////////////////////////////

    bool boolean = false;

    [[nodiscard]] int ToInt() const;

  public:
    ////////////////////////////////////////////////////////////////////////////
    ///                        Public Member Functions                       ///
    ////////////////////////////////////////////////////////////////////////////

    [[nodiscard]] bool Get() const;
};

class creation_hint_int
{
    friend instance_creation_hints;
    friend window_creation_hints;

  public:
    ////////////////////////////////////////////////////////////////////////////
    ///                Constructors, Operators, and Destructor               ///
    ////////////////////////////////////////////////////////////////////////////

    creation_hint_int(int Integer);
    creation_hint_int(const creation_hint_int&) = default;
    creation_hint_int(creation_hint_int&&) noexcept = default;
    creation_hint_int& operator=(const creation_hint_int&) = default;
    creation_hint_int& operator=(creation_hint_int&&) noexcept = default;
    creation_hint_int& operator=(int Integer);
    ~creation_hint_int() = default;

  private:
    ////////////////////////////////////////////////////////////////////////////
    ///                           Private Variables                          ///
    ////////////////////////////////////////////////////////////////////////////

    int integer = GLFW_DONT_CARE;

    ////////////////////////////////////////////////////////////////////////////
    ///                        Private Member Functions                      ///
    ////////////////////////////////////////////////////////////////////////////

    void AssertWithinBounds(int Integer);

  public:
    ////////////////////////////////////////////////////////////////////////////
    ///                        Public Member Functions                       ///
    ////////////////////////////////////////////////////////////////////////////

    [[nodiscard]] int Get() const;
};

enum struct creation_hint_client_api
{
    // NOLINTBEGIN
    eOpenGL = GLFW_OPENGL_API,
    eOpenGLES = GLFW_OPENGL_ES_API,
    eNone = GLFW_NO_API
    // NOLINTEND
};

enum struct creation_hint_context_creation_api
{
    // NOLINTBEGIN
    eNativeContext = GLFW_NATIVE_CONTEXT_API,
    eEGLContext = GLFW_EGL_CONTEXT_API,
    eOSMesaContext = GLFW_OSMESA_CONTEXT_API
    // NOLINTEND
};

enum struct creation_hint_context_robustness
{
    // NOLINTBEGIN
    eNone = GLFW_NO_ROBUSTNESS,
    eNoResetNotification = GLFW_NO_RESET_NOTIFICATION,
    eLoseContextOnReset = GLFW_LOSE_CONTEXT_ON_RESET
    // NOLINTEND
};

enum struct creation_hint_context_release_behavior
{
    // NOLINTBEGIN
    eAny = GLFW_ANY_RELEASE_BEHAVIOR,
    eFlush = GLFW_RELEASE_BEHAVIOR_FLUSH,
    eNone = GLFW_RELEASE_BEHAVIOR_NONE
    // NOLINTEND
};

enum struct creation_hint_opengl_profile
{
    // NOLINTBEGIN
    eAny = GLFW_OPENGL_ANY_PROFILE,
    eCompatible = GLFW_OPENGL_COMPAT_PROFILE,
    eCore = GLFW_OPENGL_CORE_PROFILE
    // NOLINTEND
};

template<typename T>
std::vector<T> ReadFile(const char* Absolute_Path)
{
    std::ifstream file(Absolute_Path, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        ErrorCallback(("Failed to open shader file: \"" +
                       static_cast<std::string>(Absolute_Path) + "\"")
                          .c_str());
    }
    size_t fileSize = file.tellg();
    std::vector<T> charBuffer;
    charBuffer.resize(fileSize);
    file.seekg(0);
    file.read(charBuffer.data(),
              static_cast<std::streamsize>(charBuffer.size()));
    file.close();

    return charBuffer;
}

struct image_file_info
{
    const char* path = nullptr;
    int requestedColorComponentsPerPixel = 4;
};

struct image_memory_info
{
    std::vector<uint8_t> data;
    int requestedColorComponentsPerPixel = 4;
};

class image
{
    friend internal::image_public_constructor;

    friend cursor;
    friend window;

    ////////////////////////////////////////////////////////////////////////////
    ///                Constructors, Operators, and Destructor               ///
    ////////////////////////////////////////////////////////////////////////////

    image(const image_file_info& File_Info);
    image(const image_memory_info& Memory_Info);

  public:
    image(const image&) = delete;
    image(image&&) noexcept = delete;
    image& operator=(const image&) = delete;
    image& operator=(image&&) noexcept = delete;
    ~image();

  private:
    ////////////////////////////////////////////////////////////////////////////
    ///                           Private Variables                          ///
    ////////////////////////////////////////////////////////////////////////////

    uint8_t* data = nullptr;
    area<int> size = { 0, 0 };
    int colorComponentsPerPixel = 0;

  public:
    ////////////////////////////////////////////////////////////////////////////
    ///                        Public Member Functions                       ///
    ////////////////////////////////////////////////////////////////////////////

    [[nodiscard]] area<int> GetSize() const;
};

template<typename T>
image_ptr CreateImage(const T& Info)
{
    return std::make_shared<internal::image_public_constructor>(Info);
}

struct instance_creation_hints_info
{
    // Shared hints.
    creation_hint_bool joystickHatButtons = true;

    // MacOS hints.
    creation_hint_bool cocoaChdirResources = true;
    creation_hint_bool cocoaMenubar = true;
};

struct instance_creation_hints
    : public internal::glfw_hints<glfwInitHint, nullptr, 3, 0>
{
    // Shared hints.
    internal::glfw_hint<int>& joystickHatButtons = intHints.at(0);

    // MacOS hints.
    internal::glfw_hint<int>& cocoaChdirResources = intHints.at(1);
    internal::glfw_hint<int>& cocoaMenubar = intHints.at(2);

    instance_creation_hints(
        const instance_creation_hints_info& Creation_Hints_Info =
            instance_creation_hints_info_config::DEFAULT);
};

struct instance_application_info
{
    /// @todo Create configurations for these types.
    const char* pApplicationName = "";
    uint32_t applicationVersion = VK_MAKE_VERSION(0, 0, 0);
    const char* pEngineName = "No Engine";
    uint32_t engineVersion = VK_MAKE_VERSION(0, 0, 0);
    uint32_t apiVersion = VK_API_VERSION_1_3;
};

struct instance_debug_utils_messenger_info
{
    instance_debug_utils_message_severity messageSeverity =
        instance_debug_utils_message_severity_config::ALL;
    instance_debug_utils_message_type messageType =
        instance_debug_utils_message_type_config::ALL;
    instance_debug_utils_messenge_callback pfnUserCallback =
        instance_debug_utils_messenge_callback_config::FORWARD_TO_GVW_CALLBACKS;
    void* pUserData = nullptr;
};

struct instance_info
{
    const instance_creation_flags& creationFlags =
        instance_creation_flags_config::ENUMERATE_PORTABILITY_KHR;
    const instance_application_info& applicationInfo =
        instance_application_info_config::DEFAULT;
    const instance_layers& layers = instance_layers_config::VALIDATION;
    const instance_extensions& extensions =
        instance_extensions_config::PORTABILITY_AND_DEBUG_UTILS;
    const instance_verbose_callback& verboseCallback =
        instance_info_callback_config::CERR;
    const instance_info_callback& infoCallback =
        instance_info_callback_config::CERR;
    const instance_warning_callback& warningCallback =
        instance_warning_callback_config::CERR;
    const instance_error_callback& errorCallback =
        instance_error_callback_config::CERR_THROW;
    const instance_glfw_error_callback& glfwErrorCallback =
        instance_glfw_error_callback_config::FORWARD_TO_ERROR_CALLBACK;
    const instance_debug_utils_messenger_info& debugUtilsMessengerInfo =
        instance_debug_utils_messenger_info_config::DEFAULT;
    const instance_creation_hints& initHints =
        instance_creation_hints_config::DEFAULT;
};

struct instance_joystick_event
{
    int jid;
    int event;
};

enum struct window_key
{
    // NOLINTBEGIN
    eUnknown = GLFW_KEY_UNKNOWN,
    eSpace = GLFW_KEY_SPACE,
    eApostrophe = GLFW_KEY_APOSTROPHE,
    eComma = GLFW_KEY_COMMA,
    eMinus = GLFW_KEY_MINUS,
    ePeriod = GLFW_KEY_PERIOD,
    eSlash = GLFW_KEY_SLASH,
    e0 = GLFW_KEY_0,
    e1 = GLFW_KEY_1,
    e2 = GLFW_KEY_2,
    e3 = GLFW_KEY_3,
    e4 = GLFW_KEY_4,
    e5 = GLFW_KEY_5,
    e6 = GLFW_KEY_6,
    e7 = GLFW_KEY_7,
    e8 = GLFW_KEY_8,
    e9 = GLFW_KEY_9,
    eSemicolon = GLFW_KEY_SEMICOLON,
    eEqual = GLFW_KEY_EQUAL,
    eA = GLFW_KEY_A,
    eB = GLFW_KEY_B,
    eC = GLFW_KEY_C,
    eD = GLFW_KEY_D,
    eE = GLFW_KEY_E,
    eF = GLFW_KEY_F,
    eG = GLFW_KEY_G,
    eH = GLFW_KEY_H,
    eI = GLFW_KEY_I,
    eJ = GLFW_KEY_J,
    eK = GLFW_KEY_K,
    eL = GLFW_KEY_L,
    eM = GLFW_KEY_M,
    eN = GLFW_KEY_N,
    eO = GLFW_KEY_O,
    eP = GLFW_KEY_P,
    eQ = GLFW_KEY_Q,
    eR = GLFW_KEY_R,
    eS = GLFW_KEY_S,
    eT = GLFW_KEY_T,
    eU = GLFW_KEY_U,
    eV = GLFW_KEY_V,
    eW = GLFW_KEY_W,
    eX = GLFW_KEY_X,
    eY = GLFW_KEY_Y,
    eZ = GLFW_KEY_Z,
    eLeftBracket = GLFW_KEY_LEFT_BRACKET,
    eRightBracket = GLFW_KEY_RIGHT_BRACKET,
    eGraveAccent = GLFW_KEY_GRAVE_ACCENT,
    eWorld1 = GLFW_KEY_WORLD_1,
    eWorld2 = GLFW_KEY_WORLD_2,
    eEscape = GLFW_KEY_ESCAPE,
    eEnter = GLFW_KEY_ENTER,
    eTab = GLFW_KEY_TAB,
    eBackspace = GLFW_KEY_BACKSPACE,
    eInsert = GLFW_KEY_INSERT,
    eDelete = GLFW_KEY_DELETE,
    eRight = GLFW_KEY_RIGHT,
    eLeft = GLFW_KEY_LEFT,
    eDown = GLFW_KEY_DOWN,
    eUp = GLFW_KEY_UP,
    ePageUp = GLFW_KEY_PAGE_UP,
    ePageDown = GLFW_KEY_PAGE_DOWN,
    eHome = GLFW_KEY_HOME,
    eEnd = GLFW_KEY_END,
    eCapsLock = GLFW_KEY_CAPS_LOCK,
    eScrollLock = GLFW_KEY_SCROLL_LOCK,
    eNumLock = GLFW_KEY_NUM_LOCK,
    ePrintScreen = GLFW_KEY_PRINT_SCREEN,
    ePause = GLFW_KEY_PAUSE,
    eF1 = GLFW_KEY_F1,
    eF2 = GLFW_KEY_F2,
    eF3 = GLFW_KEY_F3,
    eF4 = GLFW_KEY_F4,
    eF5 = GLFW_KEY_F5,
    eF6 = GLFW_KEY_F6,
    eF7 = GLFW_KEY_F7,
    eF8 = GLFW_KEY_F8,
    eF9 = GLFW_KEY_F9,
    eF10 = GLFW_KEY_F10,
    eF11 = GLFW_KEY_F11,
    eF12 = GLFW_KEY_F12,
    eF13 = GLFW_KEY_F13,
    eF14 = GLFW_KEY_F14,
    eF15 = GLFW_KEY_F15,
    eF16 = GLFW_KEY_F16,
    eF17 = GLFW_KEY_F17,
    eF18 = GLFW_KEY_F18,
    eF19 = GLFW_KEY_F19,
    eF20 = GLFW_KEY_F20,
    eF21 = GLFW_KEY_F21,
    eF22 = GLFW_KEY_F22,
    eF23 = GLFW_KEY_F23,
    eF24 = GLFW_KEY_F24,
    eF25 = GLFW_KEY_F25,
    eNumPad0 = GLFW_KEY_KP_0,
    eNumPad1 = GLFW_KEY_KP_1,
    eNumPad2 = GLFW_KEY_KP_2,
    eNumPad3 = GLFW_KEY_KP_3,
    eNumPad4 = GLFW_KEY_KP_4,
    eNumPad5 = GLFW_KEY_KP_5,
    eNumPad6 = GLFW_KEY_KP_6,
    eNumPad7 = GLFW_KEY_KP_7,
    eNumPad8 = GLFW_KEY_KP_8,
    eNumPad9 = GLFW_KEY_KP_9,
    eNumPadDecimal = GLFW_KEY_KP_DECIMAL,
    eNumPadDivide = GLFW_KEY_KP_DIVIDE,
    eNumPadMultiply = GLFW_KEY_KP_MULTIPLY,
    eNumPadSubtract = GLFW_KEY_KP_SUBTRACT,
    eNumPadAdd = GLFW_KEY_KP_ADD,
    eNumPadEnter = GLFW_KEY_KP_ENTER,
    eNumPadEqual = GLFW_KEY_KP_EQUAL,
    eLeftShift = GLFW_KEY_LEFT_SHIFT,
    eLeftControl = GLFW_KEY_LEFT_CONTROL,
    eLeftAlt = GLFW_KEY_LEFT_ALT,
    eLeftSuper = GLFW_KEY_LEFT_SUPER,
    eRightShift = GLFW_KEY_RIGHT_SHIFT,
    eRightControl = GLFW_KEY_RIGHT_CONTROL,
    eRightAlt = GLFW_KEY_RIGHT_ALT,
    eRightSuper = GLFW_KEY_RIGHT_SUPER,
    eMenu = GLFW_KEY_MENU
    // NOLINTEND
};

enum struct window_key_action
{
    // NOLINTBEGIN
    ePress = GLFW_PRESS,
    eRelease = GLFW_RELEASE,
    eRepeat = GLFW_REPEAT
    // NOLINTEND
};

struct window_event_callbacks
{
    window_key_event_callback keyCallback =
        window_key_event_callback_config::NONE;
    window_character_event_callback characterCallback =
        window_character_event_callback_config::NONE;
    window_cursor_position_event_callback cursorPositionCallback =
        window_cursor_position_event_callback_config::NONE;
    window_cursor_enter_event_callback cursorEnterCallback =
        window_cursor_enter_event_callback_config::NONE;
    window_mouse_button_event_callback mouseButtonCallback =
        window_mouse_button_event_callback_config::NONE;
    window_scroll_event_callback scrollCallback =
        window_scroll_event_callback_config::NONE;
    window_file_drop_event_callback fileDropCallback =
        window_file_drop_event_callback_config::NONE;
    window_close_event_callback closeCallback =
        window_close_event_callback_config::NONE;
    window_size_event_callback sizeCallback =
        window_size_event_callback_config::NONE;
    window_framebuffer_size_event_callback framebufferSizeCallback =
        window_framebuffer_size_event_callback_config::NONE;
    window_content_scale_event_callback contentScaleCallback =
        window_content_scale_event_callback_config::NONE;
    window_position_event_callback positionCallback =
        window_position_event_callback_config::NONE;
    window_iconify_event_callback iconifyCallback =
        window_iconify_event_callback_config::NONE;
    window_maximize_event_callback maximizeCallback =
        window_maximize_event_callback_config::NONE;
    window_focus_event_callback focusCallback =
        window_focus_event_callback_config::NONE;
    window_refresh_event_callback refreshCallback =
        window_refresh_event_callback_config::NONE;
};

struct window_creation_hints_info
{
    // General hints.
    creation_hint_bool resizable = true;
    creation_hint_bool visible = true;
    creation_hint_bool decorated = true;
    creation_hint_bool focused = true;
    creation_hint_bool autoIconify = true;
    creation_hint_bool floating = false;
    creation_hint_bool maximized = false;
    creation_hint_bool centerCursor = true;
    creation_hint_bool transparentFramebuffer = false;
    creation_hint_bool focusOnShow = true;
    creation_hint_bool scaleToMonitor = false;

    // Framebuffer hints.
    creation_hint_int redBits = 8;     // NOLINT
    creation_hint_int greenBits = 8;   // NOLINT
    creation_hint_int blueBits = 8;    // NOLINT
    creation_hint_int alphaBits = 8;   // NOLINT
    creation_hint_int depthBits = 24;  // NOLINT
    creation_hint_int stencilBits = 8; // NOLINT
    creation_hint_int auxBuffers = 0;
    creation_hint_bool stereo = false;
    creation_hint_int samples = 0;
    creation_hint_bool srgbCapable = false;
    creation_hint_bool doubleBuffer = true;

    // Monitor hints.
    creation_hint_int refreshRate = GLFW_DONT_CARE;

    // Context hints.
    creation_hint_client_api clientApi = creation_hint_client_api::eNone;
    creation_hint_context_creation_api contextCreationApi =
        creation_hint_context_creation_api::eNativeContext;
    creation_hint_int contextVersionMajor = 3;
    creation_hint_int contextVersionMinor = 6; // NOLINT
    creation_hint_bool openglForwardCompatible = false;
    creation_hint_bool openglDebugContext = false;
    creation_hint_opengl_profile openglProfile =
        creation_hint_opengl_profile::eCore;
    creation_hint_context_robustness contextRobustness =
        creation_hint_context_robustness::eNone;
    creation_hint_context_release_behavior contextReleaseBehavior =
        creation_hint_context_release_behavior::eAny;
    creation_hint_bool contextNoError = false;

    // MacOS hints.
    creation_hint_bool cocoaRetinaFramebuffer = true;
    creation_hint_bool cocoaGraphicsSwitching = false;
    creation_hint_string cocoaFrameName = "";

    // Linux hints.
    creation_hint_string x11ClassName = "";
    creation_hint_string x11InstanceName = "";
};

struct window_creation_hints
    : public internal::
          glfw_hints<glfwWindowHint, glfwWindowHintString, 35, 3> // NOLINT
{
    // NOLINTBEGIN

    // General hints.
    internal::glfw_hint<int>& resizable = intHints.at(0);
    internal::glfw_hint<int>& visible = intHints.at(1);
    internal::glfw_hint<int>& decorated = intHints.at(2);
    internal::glfw_hint<int>& focused = intHints.at(3);
    internal::glfw_hint<int>& autoIconify = intHints.at(4);
    internal::glfw_hint<int>& floating = intHints.at(5);
    internal::glfw_hint<int>& maximized = intHints.at(6);
    internal::glfw_hint<int>& centerCursor = intHints.at(7);
    internal::glfw_hint<int>& transparentFramebuffer = intHints.at(8);
    internal::glfw_hint<int>& focusOnShow = intHints.at(9);
    internal::glfw_hint<int>& scaleToMonitor = intHints.at(10);

    // Framebuffer hints.
    internal::glfw_hint<int>& redBits = intHints.at(11);
    internal::glfw_hint<int>& greenBits = intHints.at(12);
    internal::glfw_hint<int>& blueBits = intHints.at(13);
    internal::glfw_hint<int>& alphaBits = intHints.at(14);
    internal::glfw_hint<int>& depthBits = intHints.at(15);
    internal::glfw_hint<int>& stencilBits = intHints.at(16);
    internal::glfw_hint<int>& auxBuffers = intHints.at(17);
    internal::glfw_hint<int>& stereo = intHints.at(18);
    internal::glfw_hint<int>& samples = intHints.at(19);
    internal::glfw_hint<int>& srgbCapable = intHints.at(20);
    internal::glfw_hint<int>& doubleBuffer = intHints.at(21);

    // Monitor hints.
    internal::glfw_hint<int>& refreshRate = intHints.at(22);

    // Context hints.
    internal::glfw_hint<int>& clientApi = intHints.at(23);
    internal::glfw_hint<int>& contextCreationApi = intHints.at(24);
    internal::glfw_hint<int>& contextVersionMajor = intHints.at(25);
    internal::glfw_hint<int>& contextVersionMinor = intHints.at(26);
    internal::glfw_hint<int>& openglForwardCompatible = intHints.at(27);
    internal::glfw_hint<int>& openglDebugContext = intHints.at(28);
    internal::glfw_hint<int>& openglProfile = intHints.at(29);
    internal::glfw_hint<int>& contextRobustness = intHints.at(30);
    internal::glfw_hint<int>& contextReleaseBehavior = intHints.at(31);
    internal::glfw_hint<int>& contextNoError = intHints.at(32);

    // MacOS hints.
    internal::glfw_hint<int>& cocoaRetinaFramebuffer = intHints.at(33);
    internal::glfw_hint<int>& cocoaGraphicsSwitching = intHints.at(34);
    internal::glfw_hint<const char*>& cocoaFrameName = stringHints.at(0);

    // Linux hints.
    internal::glfw_hint<const char*>& x11ClassName = stringHints.at(1);
    internal::glfw_hint<const char*>& x11InstanceName = stringHints.at(2);

    // NOLINTEND

    window_creation_hints(const window_creation_hints_info& Creation_Hints =
                              window_creation_hints_info_config::DEFAULT);
};

struct window_key_event
{
    window_key key;
    int scancode;
    window_key_action action;
    int mods;
};

struct window_mouse_button_event
{
    int button;
    int action;
    int mods;
};

struct window_file_drop_event
{
    int count;
    const char** paths;
};

enum struct cursor_standard_shape
{
    // NOLINTBEGIN
    eArrow = GLFW_ARROW_CURSOR,
    eIBeam = GLFW_IBEAM_CURSOR,
    eCrossHair = GLFW_CROSSHAIR_CURSOR,
    eHand = GLFW_HAND_CURSOR,
    eHResize = GLFW_HRESIZE_CURSOR,
    eVResize = GLFW_VRESIZE_CURSOR
    // NOLINTEND
};

struct cursor_custom_shape_info
{
    image_ptr image = nullptr;
    cursor_hotspot hotspot = cursor_hotspot_config::DEFAULT;
};

class cursor : internal::uncopyable_unmovable // NOLINT
{
    friend internal::cursor_public_constructor;

    friend window;

    ////////////////////////////////////////////////////////////////////////////
    ///                Constructors, Operators, and Destructor               ///
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Creates a standard GLFW cursor.
    /// @remark This constructor is made private to prevent it from being called
    /// from outside of this class.
    cursor(cursor_standard_shape Cursor_Standard_Shape =
               cursor_standard_shape::eArrow);

    /// @brief Creates a custom GLFW cursor.
    /// @remark This constructor is made private to prevent it from being called
    /// from outside of this class.
    cursor(const cursor_custom_shape_info& Cursor_Custom_Shape_Info);

  public:
    // The destructor is public to allow explicit destruction.
    ~cursor();

  private:
    ////////////////////////////////////////////////////////////////////////////
    ///                           Private Variables                          ///
    ////////////////////////////////////////////////////////////////////////////

    GLFWcursor* handle = nullptr;
};

struct shader_info
{
    const char* name = "";
    const char* code = "";
    vk::ShaderStageFlagBits stage = {};
    const char* entryPoint = "main";
};

class shader
{
    friend internal::shader_public_constructor;

  public:
    vk::UniqueShaderModule handle;
    vk::ShaderStageFlagBits stage;
    const char* entryPoint = "main";
};

struct vertex_shader_info
{
    shader_info general;
    const std::vector<vk::VertexInputBindingDescription>& bindingDescriptions =
        NO_VERTEX_BINDING_DESCRIPTIONS;
    const std::vector<vk::VertexInputAttributeDescription>&
        attributeDescriptions = NO_VERTEX_ATTRIBUTE_DESCRIPTIONS;
};

class vertex_shader
{
    friend internal::vertex_shader_public_constructor;

  public:
    vk::UniqueShaderModule handle;
    vk::ShaderStageFlagBits stage;
    const char* entryPoint;
    std::vector<vk::VertexInputBindingDescription> bindingDescriptions;
    std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
};

struct fragment_shader_info
{
    shader_info general;
};

class fragment_shader
{
    friend internal::fragment_shader_public_constructor;

  public:
    vk::UniqueShaderModule handle;
    vk::ShaderStageFlagBits stage;
    const char* entryPoint;
};

struct buffer_info
{
    vk::DeviceSize sizeInBytes = 0;
    vk::BufferUsageFlags usage = vk::BufferUsageFlagBits::eVertexBuffer;
    vk::MemoryPropertyFlags memoryProperties =
        vk::MemoryPropertyFlagBits::eHostVisible |
        vk::MemoryPropertyFlagBits::eHostCoherent;
};

class buffer
{
    friend internal::buffer_public_constructor;

  public:
    vk::DeviceSize size = {};
    vk::UniqueBuffer handle;
    vk::UniqueDeviceMemory memory;
};

struct render_pass_info
{
    vk::Format format = vk::Format::eB8G8R8A8Srgb;
    vk::SampleCountFlagBits samples = vk::SampleCountFlagBits::e1;
    uint32_t graphicsAttachment = 0;
    vk::ImageLayout graphicsLayout = vk::ImageLayout::eColorAttachmentOptimal;
};

class render_pass
{
    friend internal::render_pass_public_constructor;

  public:
    vk::UniqueRenderPass handle;
};

struct swapchain_info
{
    const window_size& framebufferSize = window_size_config::W_640_H_360;
    uint32_t graphicsQueueIndex = 0;
    uint32_t presentQueueIndex = 0;
    vk::SurfaceKHR surface;
    vk::RenderPass renderPass;
    swapchain_ptr oldSwapchain = nullptr;
};

class swapchain
{
    friend internal::swapchain_public_constructor;

  public:
    vk::Viewport viewport = { .x = 0.0F,
                              .y = 0.0F,
                              .width = 0.0F,
                              .height = 0.0F,
                              .minDepth = 0.0F,
                              .maxDepth = 1.0F };
    vk::Rect2D scissor = { .offset = { .x = 0, .y = 0 },
                           .extent = { .width = 0, .height = 0 } };
    vk::UniqueSwapchainKHR handle;
    std::vector<vk::Image> swapchainImages;
    std::vector<vk::UniqueImageView> swapchainImageViews;
    std::vector<vk::UniqueFramebuffer> swapchainFramebuffers;
};

struct pipeline_shaders
{
    vertex_shader_ptr vertex;
    fragment_shader_ptr fragment;

    [[nodiscard]] std::vector<vk::PipelineShaderStageCreateInfo>
    StageCreationInfos() const;

    static const pipeline_shaders NONE;
};

struct pipeline_info
{
    const pipeline_shaders& shaders = pipeline_shaders_config::NONE;
    const pipeline_dynamic_states& dynamicStates =
        pipeline_dynamic_states_config::VIEWPORT_AND_SCISSOR;
    vk::RenderPass renderPass;
};

class pipeline
{
    friend internal::pipeline_public_constructor;

  public:
    vk::UniquePipelineLayout layout;
    vk::UniquePipeline handle;
};

struct device_selection_queue_family_info
{
    vk::DeviceQueueCreateInfo createInfo;
    vk::QueueFamilyProperties properties;
};

struct device_selection_parameter
{
    vk::PhysicalDevice physicalDevice;
    swapchain_surface_formats surfaceFormats;
    swapchain_present_modes presentModes;
};

struct device_selection_info
{
    device_selection_function selectPhysicalDevicesAndQueues =
        device_selection_function_config::MINIMUM_FOR_PRESENTATION;
    swapchain_surface_formats surfaceFormats =
        swapchain_surface_formats_config::STANDARD;
    swapchain_present_modes presentModes = swapchain_present_modes_config::FIFO;
    device_features physicalDeviceFeatures = device_features_config::NONE;
    const device_extensions& logicalDeviceExtensions =
        device_extensions_config::SWAPCHAIN;
};

struct device_info
{
    /// @todo Create the default configuration.
    vk::PhysicalDevice physicalDevice = nullptr;
    swapchain_surface_format surfaceFormat =
        swapchain_surface_format_config::STANDARD;
    swapchain_present_mode presentMode = swapchain_present_mode_config::FIFO;
    device_extensions logicalDeviceExtensions =
        device_extensions_config::SWAPCHAIN;
    device_features physicalDeviceFeatures = device_features_config::NONE;
    std::vector<device_selection_queue_family_info> queueFamilyInfos = {};
};

struct window_info
{
    std::optional<const coordinate<int>> position = std::nullopt;
    const window_size& size = window_size_config::W_640_H_360;
    const window_title& title = window_title_config::UNTITLED;
    monitor_ptr fullScreenMonitor = nullptr;
    const window_creation_hints& creationHints =
        window_creation_hints_config::DEFAULT;
    const window_event_callbacks& eventCallbacks =
        window_event_callbacks_config::NONE;
    const device_selection_info& deviceSelectionInfo =
        device_selection_info_config::DEFAULT;
    device_ptr device = nullptr;
    render_pass_ptr renderPass = nullptr;
    const pipeline_shaders& shaders = pipeline_shaders_config::NONE;
    const std::vector<gvw::xy_rgb>& staticVertices = NO_VERTICES;
    vk::DeviceSize sizeOfDynamicDataVerticesInBytes = 0;
    pipeline_ptr pipeline = nullptr;
};

} // namespace gvw