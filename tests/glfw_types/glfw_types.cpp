// Standard includes
#include <iostream>
#include <concepts>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Local includes
#include "../../gvw/gvw.hpp"
#include "../../utils/unit-test/unit-test.hpp"

template<typename EnumType>
constexpr bool Equal(const EnumType ENUM_VALUE, const int GLFW_VALUE) requires
    std::is_same_v<decltype(static_cast<int>(ENUM_VALUE)), int>
{
    return static_cast<int>(ENUM_VALUE) == GLFW_VALUE;
}

void AssertMatchingEnums()
{
    // Enum QA: creation_hint_client_api
    static_assert(
        Equal(gvw::creation_hint_client_api::eOpenGL, GLFW_OPENGL_API));
    static_assert(
        Equal(gvw::creation_hint_client_api::eOpenGL, GLFW_OPENGL_API));
    static_assert(
        Equal(gvw::creation_hint_client_api::eOpenGLES, GLFW_OPENGL_ES_API));
    static_assert(Equal(gvw::creation_hint_client_api::eNone, GLFW_NO_API));

    // Enum QA: creation_hint_context_creation_api
    static_assert(Equal(gvw::creation_hint_context_creation_api::eNativeContext,
                        GLFW_NATIVE_CONTEXT_API));
    static_assert(Equal(gvw::creation_hint_context_creation_api::eEGLContext,
                        GLFW_EGL_CONTEXT_API));
    static_assert(Equal(gvw::creation_hint_context_creation_api::eOSMesaContext,
                        GLFW_OSMESA_CONTEXT_API));

    // Enum QA: creation_hint_context_robustness
    static_assert(Equal(gvw::creation_hint_context_robustness::eNone,
                        GLFW_NO_ROBUSTNESS));
    static_assert(
        Equal(gvw::creation_hint_context_robustness::eNoResetNotification,
              GLFW_NO_RESET_NOTIFICATION));
    static_assert(
        Equal(gvw::creation_hint_context_robustness::eLoseContextOnReset,
              GLFW_LOSE_CONTEXT_ON_RESET));

    // Enum QA: creation_hint_context_release_behavior
    static_assert(Equal(gvw::creation_hint_context_release_behavior::eAny,
                        GLFW_ANY_RELEASE_BEHAVIOR));
    static_assert(Equal(gvw::creation_hint_context_release_behavior::eFlush,
                        GLFW_RELEASE_BEHAVIOR_FLUSH));
    static_assert(Equal(gvw::creation_hint_context_release_behavior::eNone,
                        GLFW_RELEASE_BEHAVIOR_NONE));

    // Enum QA: creation_hint_opengl_profile
    static_assert(Equal(gvw::creation_hint_opengl_profile::eAny,
                        GLFW_OPENGL_ANY_PROFILE));
    static_assert(Equal(gvw::creation_hint_opengl_profile::eCore,
                        GLFW_OPENGL_CORE_PROFILE));
    static_assert(Equal(gvw::creation_hint_opengl_profile::eCompatible,
                        GLFW_OPENGL_COMPAT_PROFILE));

    // Enum QA: window_key
    static_assert(Equal(gvw::window_key::eUnknown, GLFW_KEY_UNKNOWN));
    static_assert(Equal(gvw::window_key::eSpace, GLFW_KEY_SPACE));
    static_assert(Equal(gvw::window_key::eApostrophe, GLFW_KEY_APOSTROPHE));
    static_assert(Equal(gvw::window_key::eComma, GLFW_KEY_COMMA));
    static_assert(Equal(gvw::window_key::eMinus, GLFW_KEY_MINUS));
    static_assert(Equal(gvw::window_key::ePeriod, GLFW_KEY_PERIOD));
    static_assert(Equal(gvw::window_key::eSlash, GLFW_KEY_SLASH));
    static_assert(Equal(gvw::window_key::e0, GLFW_KEY_0));
    static_assert(Equal(gvw::window_key::e1, GLFW_KEY_1));
    static_assert(Equal(gvw::window_key::e2, GLFW_KEY_2));
    static_assert(Equal(gvw::window_key::e3, GLFW_KEY_3));
    static_assert(Equal(gvw::window_key::e4, GLFW_KEY_4));
    static_assert(Equal(gvw::window_key::e5, GLFW_KEY_5));
    static_assert(Equal(gvw::window_key::e6, GLFW_KEY_6));
    static_assert(Equal(gvw::window_key::e7, GLFW_KEY_7));
    static_assert(Equal(gvw::window_key::e8, GLFW_KEY_8));
    static_assert(Equal(gvw::window_key::e9, GLFW_KEY_9));
    static_assert(Equal(gvw::window_key::eSemicolon, GLFW_KEY_SEMICOLON));
    static_assert(Equal(gvw::window_key::eEqual, GLFW_KEY_EQUAL));
    static_assert(Equal(gvw::window_key::eA, GLFW_KEY_A));
    static_assert(Equal(gvw::window_key::eB, GLFW_KEY_B));
    static_assert(Equal(gvw::window_key::eC, GLFW_KEY_C));
    static_assert(Equal(gvw::window_key::eD, GLFW_KEY_D));
    static_assert(Equal(gvw::window_key::eE, GLFW_KEY_E));
    static_assert(Equal(gvw::window_key::eF, GLFW_KEY_F));
    static_assert(Equal(gvw::window_key::eG, GLFW_KEY_G));
    static_assert(Equal(gvw::window_key::eH, GLFW_KEY_H));
    static_assert(Equal(gvw::window_key::eI, GLFW_KEY_I));
    static_assert(Equal(gvw::window_key::eJ, GLFW_KEY_J));
    static_assert(Equal(gvw::window_key::eK, GLFW_KEY_K));
    static_assert(Equal(gvw::window_key::eL, GLFW_KEY_L));
    static_assert(Equal(gvw::window_key::eM, GLFW_KEY_M));
    static_assert(Equal(gvw::window_key::eN, GLFW_KEY_N));
    static_assert(Equal(gvw::window_key::eO, GLFW_KEY_O));
    static_assert(Equal(gvw::window_key::eP, GLFW_KEY_P));
    static_assert(Equal(gvw::window_key::eQ, GLFW_KEY_Q));
    static_assert(Equal(gvw::window_key::eR, GLFW_KEY_R));
    static_assert(Equal(gvw::window_key::eS, GLFW_KEY_S));
    static_assert(Equal(gvw::window_key::eT, GLFW_KEY_T));
    static_assert(Equal(gvw::window_key::eU, GLFW_KEY_U));
    static_assert(Equal(gvw::window_key::eV, GLFW_KEY_V));
    static_assert(Equal(gvw::window_key::eW, GLFW_KEY_W));
    static_assert(Equal(gvw::window_key::eX, GLFW_KEY_X));
    static_assert(Equal(gvw::window_key::eY, GLFW_KEY_Y));
    static_assert(Equal(gvw::window_key::eZ, GLFW_KEY_Z));
    static_assert(Equal(gvw::window_key::eLeftBracket, GLFW_KEY_LEFT_BRACKET));
    static_assert(
        Equal(gvw::window_key::eRightBracket, GLFW_KEY_RIGHT_BRACKET));
    static_assert(Equal(gvw::window_key::eGraveAccent, GLFW_KEY_GRAVE_ACCENT));
    static_assert(Equal(gvw::window_key::eWorld1, GLFW_KEY_WORLD_1));
    static_assert(Equal(gvw::window_key::eWorld2, GLFW_KEY_WORLD_2));
    static_assert(Equal(gvw::window_key::eEscape, GLFW_KEY_ESCAPE));
    static_assert(Equal(gvw::window_key::eEnter, GLFW_KEY_ENTER));
    static_assert(Equal(gvw::window_key::eTab, GLFW_KEY_TAB));
    static_assert(Equal(gvw::window_key::eBackspace, GLFW_KEY_BACKSPACE));
    static_assert(Equal(gvw::window_key::eInsert, GLFW_KEY_INSERT));
    static_assert(Equal(gvw::window_key::eDelete, GLFW_KEY_DELETE));
    static_assert(Equal(gvw::window_key::eRight, GLFW_KEY_RIGHT));
    static_assert(Equal(gvw::window_key::eLeft, GLFW_KEY_LEFT));
    static_assert(Equal(gvw::window_key::eDown, GLFW_KEY_DOWN));
    static_assert(Equal(gvw::window_key::eUp, GLFW_KEY_UP));
    static_assert(Equal(gvw::window_key::ePageUp, GLFW_KEY_PAGE_UP));
    static_assert(Equal(gvw::window_key::ePageDown, GLFW_KEY_PAGE_DOWN));
    static_assert(Equal(gvw::window_key::eHome, GLFW_KEY_HOME));
    static_assert(Equal(gvw::window_key::eEnd, GLFW_KEY_END));
    static_assert(Equal(gvw::window_key::eCapsLock, GLFW_KEY_CAPS_LOCK));
    static_assert(Equal(gvw::window_key::eScrollLock, GLFW_KEY_SCROLL_LOCK));
    static_assert(Equal(gvw::window_key::eNumLock, GLFW_KEY_NUM_LOCK));
    static_assert(Equal(gvw::window_key::ePrintScreen, GLFW_KEY_PRINT_SCREEN));
    static_assert(Equal(gvw::window_key::ePause, GLFW_KEY_PAUSE));
    static_assert(Equal(gvw::window_key::eF1, GLFW_KEY_F1));
    static_assert(Equal(gvw::window_key::eF2, GLFW_KEY_F2));
    static_assert(Equal(gvw::window_key::eF3, GLFW_KEY_F3));
    static_assert(Equal(gvw::window_key::eF4, GLFW_KEY_F4));
    static_assert(Equal(gvw::window_key::eF5, GLFW_KEY_F5));
    static_assert(Equal(gvw::window_key::eF6, GLFW_KEY_F6));
    static_assert(Equal(gvw::window_key::eF7, GLFW_KEY_F7));
    static_assert(Equal(gvw::window_key::eF8, GLFW_KEY_F8));
    static_assert(Equal(gvw::window_key::eF9, GLFW_KEY_F9));
    static_assert(Equal(gvw::window_key::eF10, GLFW_KEY_F10));
    static_assert(Equal(gvw::window_key::eF11, GLFW_KEY_F11));
    static_assert(Equal(gvw::window_key::eF12, GLFW_KEY_F12));
    static_assert(Equal(gvw::window_key::eF13, GLFW_KEY_F13));
    static_assert(Equal(gvw::window_key::eF14, GLFW_KEY_F14));
    static_assert(Equal(gvw::window_key::eF15, GLFW_KEY_F15));
    static_assert(Equal(gvw::window_key::eF16, GLFW_KEY_F16));
    static_assert(Equal(gvw::window_key::eF17, GLFW_KEY_F17));
    static_assert(Equal(gvw::window_key::eF18, GLFW_KEY_F18));
    static_assert(Equal(gvw::window_key::eF19, GLFW_KEY_F19));
    static_assert(Equal(gvw::window_key::eF20, GLFW_KEY_F20));
    static_assert(Equal(gvw::window_key::eF21, GLFW_KEY_F21));
    static_assert(Equal(gvw::window_key::eF22, GLFW_KEY_F22));
    static_assert(Equal(gvw::window_key::eF23, GLFW_KEY_F23));
    static_assert(Equal(gvw::window_key::eF24, GLFW_KEY_F24));
    static_assert(Equal(gvw::window_key::eF25, GLFW_KEY_F25));
    static_assert(Equal(gvw::window_key::eNumPad0, GLFW_KEY_KP_0));
    static_assert(Equal(gvw::window_key::eNumPad1, GLFW_KEY_KP_1));
    static_assert(Equal(gvw::window_key::eNumPad2, GLFW_KEY_KP_2));
    static_assert(Equal(gvw::window_key::eNumPad3, GLFW_KEY_KP_3));
    static_assert(Equal(gvw::window_key::eNumPad4, GLFW_KEY_KP_4));
    static_assert(Equal(gvw::window_key::eNumPad5, GLFW_KEY_KP_5));
    static_assert(Equal(gvw::window_key::eNumPad6, GLFW_KEY_KP_6));
    static_assert(Equal(gvw::window_key::eNumPad7, GLFW_KEY_KP_7));
    static_assert(Equal(gvw::window_key::eNumPad8, GLFW_KEY_KP_8));
    static_assert(Equal(gvw::window_key::eNumPad9, GLFW_KEY_KP_9));
    static_assert(Equal(gvw::window_key::eNumPadDecimal, GLFW_KEY_KP_DECIMAL));
    static_assert(Equal(gvw::window_key::eNumPadDivide, GLFW_KEY_KP_DIVIDE));
    static_assert(
        Equal(gvw::window_key::eNumPadMultiply, GLFW_KEY_KP_MULTIPLY));
    static_assert(
        Equal(gvw::window_key::eNumPadSubtract, GLFW_KEY_KP_SUBTRACT));
    static_assert(Equal(gvw::window_key::eNumPadAdd, GLFW_KEY_KP_ADD));
    static_assert(Equal(gvw::window_key::eNumPadEnter, GLFW_KEY_KP_ENTER));
    static_assert(Equal(gvw::window_key::eNumPadEqual, GLFW_KEY_KP_EQUAL));
    static_assert(Equal(gvw::window_key::eLeftShift, GLFW_KEY_LEFT_SHIFT));
    static_assert(Equal(gvw::window_key::eLeftControl, GLFW_KEY_LEFT_CONTROL));
    static_assert(Equal(gvw::window_key::eLeftAlt, GLFW_KEY_LEFT_ALT));
    static_assert(Equal(gvw::window_key::eLeftSuper, GLFW_KEY_LEFT_SUPER));
    static_assert(Equal(gvw::window_key::eRightShift, GLFW_KEY_RIGHT_SHIFT));
    static_assert(
        Equal(gvw::window_key::eRightControl, GLFW_KEY_RIGHT_CONTROL));
    static_assert(Equal(gvw::window_key::eRightAlt, GLFW_KEY_RIGHT_ALT));
    static_assert(Equal(gvw::window_key::eRightSuper, GLFW_KEY_RIGHT_SUPER));
    static_assert(Equal(gvw::window_key::eMenu, GLFW_KEY_MENU));

    // Enum QA: window_key_action
    static_assert(Equal(gvw::window_key_action::eRelease, GLFW_RELEASE));
    static_assert(Equal(gvw::window_key_action::ePress, GLFW_PRESS));
    static_assert(Equal(gvw::window_key_action::eRepeat, GLFW_REPEAT));

    // Enum QA: cursor_standard_shape
    static_assert(Equal(gvw::cursor_standard_shape::eArrow, GLFW_ARROW_CURSOR));
    static_assert(Equal(gvw::cursor_standard_shape::eIBeam, GLFW_IBEAM_CURSOR));
    static_assert(
        Equal(gvw::cursor_standard_shape::eCrossHair, GLFW_CROSSHAIR_CURSOR));
    static_assert(Equal(gvw::cursor_standard_shape::eHand, GLFW_HAND_CURSOR));
    static_assert(
        Equal(gvw::cursor_standard_shape::eHResize, GLFW_HRESIZE_CURSOR));
    static_assert(
        Equal(gvw::cursor_standard_shape::eVResize, GLFW_VRESIZE_CURSOR));

    // Enum QA: glfw_bool
    static_assert(Equal(gvw::internal::glfw_bool::eFalse, GLFW_FALSE));
    static_assert(Equal(gvw::internal::glfw_bool::eTrue, GLFW_TRUE));

    // Enum QA: window_input_mode
    static_assert(
        Equal(gvw::internal::window_input_mode::eCursor, GLFW_CURSOR));
    static_assert(
        Equal(gvw::internal::window_input_mode::eStickyKeys, GLFW_STICKY_KEYS));
    static_assert(Equal(gvw::internal::window_input_mode::eStickyMouseButtons,
                        GLFW_STICKY_MOUSE_BUTTONS));

    // Enum QA: window_input_mode_cursor
    static_assert(Equal(gvw::internal::window_input_mode_cursor::eNormal,
                        GLFW_CURSOR_NORMAL));
    static_assert(Equal(gvw::internal::window_input_mode_cursor::eHidden,
                        GLFW_CURSOR_HIDDEN));
    static_assert(Equal(gvw::internal::window_input_mode_cursor::eDisabled,
                        GLFW_CURSOR_DISABLED));
}

void AssertMatchingFunctionSignatures()
{
    // Function Signature QA: instance
    static_assert(
        std::is_same_v<gvw::instance_glfw_error_callback, GLFWerrorfun>);
    static_assert(
        std::is_same_v<gvw::instance_joystick_event_callback, GLFWjoystickfun>);

    // Function Signature QA: window
    static_assert(std::is_same_v<gvw::window_key_event_callback, GLFWkeyfun>);
    static_assert(
        std::is_same_v<gvw::window_character_event_callback, GLFWcharfun>);
    static_assert(std::is_same_v<gvw::window_cursor_position_event_callback,
                                 GLFWcursorposfun>);
    static_assert(std::is_same_v<gvw::window_cursor_enter_event_callback,
                                 GLFWcursorenterfun>);
    static_assert(std::is_same_v<gvw::window_mouse_button_event_callback,
                                 GLFWmousebuttonfun>);
    static_assert(
        std::is_same_v<gvw::window_scroll_event_callback, GLFWscrollfun>);
    static_assert(
        std::is_same_v<gvw::window_file_drop_event_callback, GLFWdropfun>);
    static_assert(
        std::is_same_v<gvw::window_close_event_callback, GLFWwindowclosefun>);
    static_assert(
        std::is_same_v<gvw::window_size_event_callback, GLFWwindowsizefun>);
    static_assert(std::is_same_v<gvw::window_framebuffer_size_event_callback,
                                 GLFWframebuffersizefun>);
    static_assert(std::is_same_v<gvw::window_content_scale_event_callback,
                                 GLFWwindowcontentscalefun>);
    static_assert(
        std::is_same_v<gvw::window_position_event_callback, GLFWwindowposfun>);
    static_assert(std::is_same_v<gvw::window_iconify_event_callback,
                                 GLFWwindowiconifyfun>);
    static_assert(std::is_same_v<gvw::window_maximize_event_callback,
                                 GLFWwindowmaximizefun>);
    static_assert(
        std::is_same_v<gvw::window_focus_event_callback, GLFWwindowfocusfun>);
    static_assert(std::is_same_v<gvw::window_refresh_event_callback,
                                 GLFWwindowrefreshfun>);
}

int main()
{
    // Enumerations
    AssertMatchingEnums();

    // Function Signatures
    AssertMatchingFunctionSignatures();
}