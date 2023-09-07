#include <iostream>
#include <concepts>
#include "../../gvw/gvw.hpp"
#include "../../utils/unit-test/unit-test.hpp"

template<typename EnumType>
void ThrowIfUnequal(EnumType Enum_Value, int Glfw_Value) requires
    std::is_same_v<decltype(static_cast<int>(Enum_Value)), int>
{
    if (static_cast<int>(Enum_Value) != Glfw_Value) {
        throw std::runtime_error(
            "Enumeration value: " +
            std::to_string(static_cast<int>(Enum_Value)) +
            " does not equal the GLFW value: " + std::to_string(Glfw_Value));
    }
}

void CreationHintClientApi()
{
    // Enum QA: creation_hint_client_api
    ThrowIfUnequal(gvw::creation_hint_client_api::eOpenGL, GLFW_OPENGL_API);
    ThrowIfUnequal(gvw::creation_hint_client_api::eOpenGLES,
                   GLFW_OPENGL_ES_API);
    ThrowIfUnequal(gvw::creation_hint_client_api::eNone, GLFW_NO_API);
}

void CreationHintContextCreationApi()
{
    // Enum QA: creation_hint_context_creation_api
    ThrowIfUnequal(gvw::creation_hint_context_creation_api::eNativeContext,
                   GLFW_NATIVE_CONTEXT_API);
    ThrowIfUnequal(gvw::creation_hint_context_creation_api::eEGLContext,
                   GLFW_EGL_CONTEXT_API);
    ThrowIfUnequal(gvw::creation_hint_context_creation_api::eOSMesaContext,
                   GLFW_OSMESA_CONTEXT_API);
}

void CreationHintContextRobustness()
{
    // Enum QA: creation_hint_context_robustness
    ThrowIfUnequal(gvw::creation_hint_context_robustness::eNone,
                   GLFW_NO_ROBUSTNESS);
    ThrowIfUnequal(gvw::creation_hint_context_robustness::eNoResetNotification,
                   GLFW_NO_RESET_NOTIFICATION);
    ThrowIfUnequal(gvw::creation_hint_context_robustness::eLoseContextOnReset,
                   GLFW_LOSE_CONTEXT_ON_RESET);
}

void CreationHintContextReleaseBehavior()
{
    // Enum QA: creation_hint_context_release_behavior
    ThrowIfUnequal(gvw::creation_hint_context_release_behavior::eAny,
                   GLFW_ANY_RELEASE_BEHAVIOR);
    ThrowIfUnequal(gvw::creation_hint_context_release_behavior::eFlush,
                   GLFW_RELEASE_BEHAVIOR_FLUSH);
    ThrowIfUnequal(gvw::creation_hint_context_release_behavior::eNone,
                   GLFW_RELEASE_BEHAVIOR_NONE);
};

void CreationHintOpenGLProfile()
{
    // Enum QA: creation_hint_opengl_profile
    ThrowIfUnequal(gvw::creation_hint_opengl_profile::eAny,
                   GLFW_OPENGL_ANY_PROFILE);
    ThrowIfUnequal(gvw::creation_hint_opengl_profile::eCore,
                   GLFW_OPENGL_CORE_PROFILE);
    ThrowIfUnequal(gvw::creation_hint_opengl_profile::eCompatible,
                   GLFW_OPENGL_COMPAT_PROFILE);
}

void WindowKey()
{
    // Enum QA: window_key
    ThrowIfUnequal(gvw::window_key::eUnknown, GLFW_KEY_UNKNOWN);
    ThrowIfUnequal(gvw::window_key::eSpace, GLFW_KEY_SPACE);
    ThrowIfUnequal(gvw::window_key::eApostrophe, GLFW_KEY_APOSTROPHE);
    ThrowIfUnequal(gvw::window_key::eComma, GLFW_KEY_COMMA);
    ThrowIfUnequal(gvw::window_key::eMinus, GLFW_KEY_MINUS);
    ThrowIfUnequal(gvw::window_key::ePeriod, GLFW_KEY_PERIOD);
    ThrowIfUnequal(gvw::window_key::eSlash, GLFW_KEY_SLASH);
    ThrowIfUnequal(gvw::window_key::e0, GLFW_KEY_0);
    ThrowIfUnequal(gvw::window_key::e1, GLFW_KEY_1);
    ThrowIfUnequal(gvw::window_key::e2, GLFW_KEY_2);
    ThrowIfUnequal(gvw::window_key::e3, GLFW_KEY_3);
    ThrowIfUnequal(gvw::window_key::e4, GLFW_KEY_4);
    ThrowIfUnequal(gvw::window_key::e5, GLFW_KEY_5);
    ThrowIfUnequal(gvw::window_key::e6, GLFW_KEY_6);
    ThrowIfUnequal(gvw::window_key::e7, GLFW_KEY_7);
    ThrowIfUnequal(gvw::window_key::e8, GLFW_KEY_8);
    ThrowIfUnequal(gvw::window_key::e9, GLFW_KEY_9);
    ThrowIfUnequal(gvw::window_key::eSemicolon, GLFW_KEY_SEMICOLON);
    ThrowIfUnequal(gvw::window_key::eEqual, GLFW_KEY_EQUAL);
    ThrowIfUnequal(gvw::window_key::eA, GLFW_KEY_A);
    ThrowIfUnequal(gvw::window_key::eB, GLFW_KEY_B);
    ThrowIfUnequal(gvw::window_key::eC, GLFW_KEY_C);
    ThrowIfUnequal(gvw::window_key::eD, GLFW_KEY_D);
    ThrowIfUnequal(gvw::window_key::eE, GLFW_KEY_E);
    ThrowIfUnequal(gvw::window_key::eF, GLFW_KEY_F);
    ThrowIfUnequal(gvw::window_key::eG, GLFW_KEY_G);
    ThrowIfUnequal(gvw::window_key::eH, GLFW_KEY_H);
    ThrowIfUnequal(gvw::window_key::eI, GLFW_KEY_I);
    ThrowIfUnequal(gvw::window_key::eJ, GLFW_KEY_J);
    ThrowIfUnequal(gvw::window_key::eK, GLFW_KEY_K);
    ThrowIfUnequal(gvw::window_key::eL, GLFW_KEY_L);
    ThrowIfUnequal(gvw::window_key::eM, GLFW_KEY_M);
    ThrowIfUnequal(gvw::window_key::eN, GLFW_KEY_N);
    ThrowIfUnequal(gvw::window_key::eO, GLFW_KEY_O);
    ThrowIfUnequal(gvw::window_key::eP, GLFW_KEY_P);
    ThrowIfUnequal(gvw::window_key::eQ, GLFW_KEY_Q);
    ThrowIfUnequal(gvw::window_key::eR, GLFW_KEY_R);
    ThrowIfUnequal(gvw::window_key::eS, GLFW_KEY_S);
    ThrowIfUnequal(gvw::window_key::eT, GLFW_KEY_T);
    ThrowIfUnequal(gvw::window_key::eU, GLFW_KEY_U);
    ThrowIfUnequal(gvw::window_key::eV, GLFW_KEY_V);
    ThrowIfUnequal(gvw::window_key::eW, GLFW_KEY_W);
    ThrowIfUnequal(gvw::window_key::eX, GLFW_KEY_X);
    ThrowIfUnequal(gvw::window_key::eY, GLFW_KEY_Y);
    ThrowIfUnequal(gvw::window_key::eZ, GLFW_KEY_Z);
    ThrowIfUnequal(gvw::window_key::eLeftBracket, GLFW_KEY_LEFT_BRACKET);
    ThrowIfUnequal(gvw::window_key::eRightBracket, GLFW_KEY_RIGHT_BRACKET);
    ThrowIfUnequal(gvw::window_key::eGraveAccent, GLFW_KEY_GRAVE_ACCENT);
    ThrowIfUnequal(gvw::window_key::eWorld1, GLFW_KEY_WORLD_1);
    ThrowIfUnequal(gvw::window_key::eWorld2, GLFW_KEY_WORLD_2);
    ThrowIfUnequal(gvw::window_key::eEscape, GLFW_KEY_ESCAPE);
    ThrowIfUnequal(gvw::window_key::eEnter, GLFW_KEY_ENTER);
    ThrowIfUnequal(gvw::window_key::eTab, GLFW_KEY_TAB);
    ThrowIfUnequal(gvw::window_key::eBackspace, GLFW_KEY_BACKSPACE);
    ThrowIfUnequal(gvw::window_key::eInsert, GLFW_KEY_INSERT);
    ThrowIfUnequal(gvw::window_key::eDelete, GLFW_KEY_DELETE);
    ThrowIfUnequal(gvw::window_key::eRight, GLFW_KEY_RIGHT);
    ThrowIfUnequal(gvw::window_key::eLeft, GLFW_KEY_LEFT);
    ThrowIfUnequal(gvw::window_key::eDown, GLFW_KEY_DOWN);
    ThrowIfUnequal(gvw::window_key::eUp, GLFW_KEY_UP);
    ThrowIfUnequal(gvw::window_key::ePageUp, GLFW_KEY_PAGE_UP);
    ThrowIfUnequal(gvw::window_key::ePageDown, GLFW_KEY_PAGE_DOWN);
    ThrowIfUnequal(gvw::window_key::eHome, GLFW_KEY_HOME);
    ThrowIfUnequal(gvw::window_key::eEnd, GLFW_KEY_END);
    ThrowIfUnequal(gvw::window_key::eCapsLock, GLFW_KEY_CAPS_LOCK);
    ThrowIfUnequal(gvw::window_key::eScrollLock, GLFW_KEY_SCROLL_LOCK);
    ThrowIfUnequal(gvw::window_key::eNumLock, GLFW_KEY_NUM_LOCK);
    ThrowIfUnequal(gvw::window_key::ePrintScreen, GLFW_KEY_PRINT_SCREEN);
    ThrowIfUnequal(gvw::window_key::ePause, GLFW_KEY_PAUSE);
    ThrowIfUnequal(gvw::window_key::eF1, GLFW_KEY_F1);
    ThrowIfUnequal(gvw::window_key::eF2, GLFW_KEY_F2);
    ThrowIfUnequal(gvw::window_key::eF3, GLFW_KEY_F3);
    ThrowIfUnequal(gvw::window_key::eF4, GLFW_KEY_F4);
    ThrowIfUnequal(gvw::window_key::eF5, GLFW_KEY_F5);
    ThrowIfUnequal(gvw::window_key::eF6, GLFW_KEY_F6);
    ThrowIfUnequal(gvw::window_key::eF7, GLFW_KEY_F7);
    ThrowIfUnequal(gvw::window_key::eF8, GLFW_KEY_F8);
    ThrowIfUnequal(gvw::window_key::eF9, GLFW_KEY_F9);
    ThrowIfUnequal(gvw::window_key::eF10, GLFW_KEY_F10);
    ThrowIfUnequal(gvw::window_key::eF11, GLFW_KEY_F11);
    ThrowIfUnequal(gvw::window_key::eF12, GLFW_KEY_F12);
    ThrowIfUnequal(gvw::window_key::eF13, GLFW_KEY_F13);
    ThrowIfUnequal(gvw::window_key::eF14, GLFW_KEY_F14);
    ThrowIfUnequal(gvw::window_key::eF15, GLFW_KEY_F15);
    ThrowIfUnequal(gvw::window_key::eF16, GLFW_KEY_F16);
    ThrowIfUnequal(gvw::window_key::eF17, GLFW_KEY_F17);
    ThrowIfUnequal(gvw::window_key::eF18, GLFW_KEY_F18);
    ThrowIfUnequal(gvw::window_key::eF19, GLFW_KEY_F19);
    ThrowIfUnequal(gvw::window_key::eF20, GLFW_KEY_F20);
    ThrowIfUnequal(gvw::window_key::eF21, GLFW_KEY_F21);
    ThrowIfUnequal(gvw::window_key::eF22, GLFW_KEY_F22);
    ThrowIfUnequal(gvw::window_key::eF23, GLFW_KEY_F23);
    ThrowIfUnequal(gvw::window_key::eF24, GLFW_KEY_F24);
    ThrowIfUnequal(gvw::window_key::eF25, GLFW_KEY_F25);
    ThrowIfUnequal(gvw::window_key::eNumPad0, GLFW_KEY_KP_0);
    ThrowIfUnequal(gvw::window_key::eNumPad1, GLFW_KEY_KP_1);
    ThrowIfUnequal(gvw::window_key::eNumPad2, GLFW_KEY_KP_2);
    ThrowIfUnequal(gvw::window_key::eNumPad3, GLFW_KEY_KP_3);
    ThrowIfUnequal(gvw::window_key::eNumPad4, GLFW_KEY_KP_4);
    ThrowIfUnequal(gvw::window_key::eNumPad5, GLFW_KEY_KP_5);
    ThrowIfUnequal(gvw::window_key::eNumPad6, GLFW_KEY_KP_6);
    ThrowIfUnequal(gvw::window_key::eNumPad7, GLFW_KEY_KP_7);
    ThrowIfUnequal(gvw::window_key::eNumPad8, GLFW_KEY_KP_8);
    ThrowIfUnequal(gvw::window_key::eNumPad9, GLFW_KEY_KP_9);
    ThrowIfUnequal(gvw::window_key::eNumPadDecimal, GLFW_KEY_KP_DECIMAL);
    ThrowIfUnequal(gvw::window_key::eNumPadDivide, GLFW_KEY_KP_DIVIDE);
    ThrowIfUnequal(gvw::window_key::eNumPadMultiply, GLFW_KEY_KP_MULTIPLY);
    ThrowIfUnequal(gvw::window_key::eNumPadSubtract, GLFW_KEY_KP_SUBTRACT);
    ThrowIfUnequal(gvw::window_key::eNumPadAdd, GLFW_KEY_KP_ADD);
    ThrowIfUnequal(gvw::window_key::eNumPadEnter, GLFW_KEY_KP_ENTER);
    ThrowIfUnequal(gvw::window_key::eNumPadEqual, GLFW_KEY_KP_EQUAL);
    ThrowIfUnequal(gvw::window_key::eLeftShift, GLFW_KEY_LEFT_SHIFT);
    ThrowIfUnequal(gvw::window_key::eLeftControl, GLFW_KEY_LEFT_CONTROL);
    ThrowIfUnequal(gvw::window_key::eLeftAlt, GLFW_KEY_LEFT_ALT);
    ThrowIfUnequal(gvw::window_key::eLeftSuper, GLFW_KEY_LEFT_SUPER);
    ThrowIfUnequal(gvw::window_key::eRightShift, GLFW_KEY_RIGHT_SHIFT);
    ThrowIfUnequal(gvw::window_key::eRightControl, GLFW_KEY_RIGHT_CONTROL);
    ThrowIfUnequal(gvw::window_key::eRightAlt, GLFW_KEY_RIGHT_ALT);
    ThrowIfUnequal(gvw::window_key::eRightSuper, GLFW_KEY_RIGHT_SUPER);
    ThrowIfUnequal(gvw::window_key::eMenu, GLFW_KEY_MENU);
}

void WindowKeyAction()
{
    // Enum QA: window_key_action
    ThrowIfUnequal(gvw::window_key_action::eRelease, GLFW_RELEASE);
    ThrowIfUnequal(gvw::window_key_action::ePress, GLFW_PRESS);
    ThrowIfUnequal(gvw::window_key_action::eRepeat, GLFW_REPEAT);
}

void CursorStandardShape()
{
    // Enum QA: cursor_standard_shape
    ThrowIfUnequal(gvw::cursor_standard_shape::eArrow, GLFW_ARROW_CURSOR);
    ThrowIfUnequal(gvw::cursor_standard_shape::eIBeam, GLFW_IBEAM_CURSOR);
    ThrowIfUnequal(gvw::cursor_standard_shape::eCrossHair,
                   GLFW_CROSSHAIR_CURSOR);
    ThrowIfUnequal(gvw::cursor_standard_shape::eHand, GLFW_HAND_CURSOR);
    ThrowIfUnequal(gvw::cursor_standard_shape::eHResize, GLFW_HRESIZE_CURSOR);
    ThrowIfUnequal(gvw::cursor_standard_shape::eVResize, GLFW_VRESIZE_CURSOR);
}

void WindowInputMode()
{
    // Enum QA: window_input_mode
    ThrowIfUnequal(gvw::internal::window_input_mode::eCursor, GLFW_CURSOR);
    ThrowIfUnequal(gvw::internal::window_input_mode::eStickyKeys,
                   GLFW_STICKY_KEYS);
    ThrowIfUnequal(gvw::internal::window_input_mode::eStickyMouseButtons,
                   GLFW_STICKY_MOUSE_BUTTONS);
}

void WindowInputModeCursor()
{
    // Enum QA: window_input_mode_cursor
    ThrowIfUnequal(gvw::internal::window_input_mode_cursor::eNormal,
                   GLFW_CURSOR_NORMAL);
    ThrowIfUnequal(gvw::internal::window_input_mode_cursor::eHidden,
                   GLFW_CURSOR_HIDDEN);
    ThrowIfUnequal(gvw::internal::window_input_mode_cursor::eDisabled,
                   GLFW_CURSOR_DISABLED);
}

int main()
{
    test::ForThrow("Enum QA: creation_hint_client_api", CreationHintClientApi);
    test::ForThrow("Enum QA: creation_hint_context_creation_api",
                   CreationHintContextCreationApi);
    test::ForThrow("Enum QA: creation_hint_context_robustness",
                   CreationHintContextRobustness);
    test::ForThrow("Enum QA: creation_hint_context_release_behavior",
                   CreationHintContextReleaseBehavior);
    test::ForThrow("Enum QA: creation_hint_opengl_profile",
                   CreationHintOpenGLProfile);
    test::ForThrow("Enum QA: window_key", WindowKey);
    test::ForThrow("Enum QA: window_key_action", WindowKeyAction);
    test::ForThrow("Enum QA: cursor_standard_shape", CursorStandardShape);
    test::ForThrow("Enum QA: window_input_mode", WindowInputMode);
    test::ForThrow("Enum QA: window_input_mode_cursor", WindowInputModeCursor);
}