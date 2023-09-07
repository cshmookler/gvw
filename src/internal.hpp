#pragma once

/**
 * @file internal.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief Internal type, constant, and function declarations.
 * @date 2023-09-01
 */

// Standard includes
#include <concepts>

// Leading local includes
#include "gvw.hpp"

namespace gvw::internal {

/// @brief Creates a derived type with a public constructor.
template<typename T>
struct public_constructor;

/// @brief Deletes the copy constructor, move constructor, copy assignment
/// operator, and move assignment operator of derived types.
/// @todo Address the issue below.
/// @remark Defines a default constructor so designated initializers are
/// disabled for derived types.
struct uncopyable_unmovable;

/// @brief Manages destruction of non-unique objects.
template<typename... Args>
class terminator;

enum struct glfw_bool;

/// @brief Returns a vector containing all the items present in both arrays.
template<typename Type1,
         typename Type2 = Type1,
         typename Out = Type1,
         typename CallableIdentical,
         typename CallableGet>
std::vector<Out> GetCommonElements(const std::vector<Type1>& Arr_1,
                                   const std::vector<Type2>& Arr_2,
                                   CallableIdentical Identical,
                                   CallableGet Get) requires
    std::is_invocable_r_v<bool, CallableIdentical, Type1, Type2> &&
    std::is_invocable_r_v<Out, CallableGet, Type1, Type2>;

template<typename Type1, typename Type2 = Type1, typename CallableIdentical>
std::vector<Type1> GetCommonElementsInArr1(const std::vector<Type1>& Arr_1,
                                           const std::vector<Type2>& Arr_2,
                                           CallableIdentical Identical) requires
    std::is_invocable_r_v<bool, CallableIdentical, Type1, Type2>;

template<typename Type1, typename Type2 = Type1, typename CallableIdentical>
std::vector<Type1> GetUncommonElementsInArr1(
    const std::vector<Type1>& Arr_1,
    const std::vector<Type2>& Arr_2,
    CallableIdentical Identical) requires
    std::is_invocable_r_v<bool, CallableIdentical, Type1, Type2>;

/*********************************    Hints    ********************************/
/// @brief GLFW hint ID and default value.
/// @tparam T The value type of the hint. Almost always int. Sometimes
/// const char*.
template<typename T>
struct glfw_hint;

/// @brief GLFW hint function signatures.
using int_hint_function = void (*)(int, int);
using string_hint_function = void (*)(int, const char*);

/// @brief The base struct of the *_init_hints and *_hints structs.
/// @tparam The integer hint function used to apply the integer hints.
/// @tparam The string hint function used to apply the string hints.
/// @tparam IntHints The number of hints with cooresponding integer values
/// (int).
/// @tparam StringHints The number of hints with cooresponding string
/// values (const char*).
template<int_hint_function IntHintFunc,
         string_hint_function StringHintFunc,
         size_t IntHints,
         size_t StringHints>
struct glfw_hints;

/*********************************    Image    ********************************/
using image_public_constructor = public_constructor<image>;

/*****************************    GVW Instance    *****************************/
using instance_public_constructor = public_constructor<instance>;

template<instance_debug_utils_messenger_callback_print_function PrintFunction>
VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessengerCallbackTemplate(
    VkDebugUtilsMessageSeverityFlagBitsEXT Message_Severity,
    VkDebugUtilsMessageTypeFlagsEXT Message_Type,
    const VkDebugUtilsMessengerCallbackDataEXT* P_Callback_Data,
    void* P_User_Data);

using callback_print_function = void (*)(const char*);

template<callback_print_function PrintFunction>
void VerboseCallbackTemplate(const char* Message);
template<callback_print_function PrintFunction>
void InfoCallbackTemplate(const char* Message);
template<callback_print_function PrintFunction>
void WarningCallbackTemplate(const char* Message);
template<callback_print_function PrintFunction>
void ErrorCallbackTemplate(const char* Message);

template<callback_print_function PrintFunction>
void GlfwErrorCallbackTemplate(int Error_Code, const char* Message);

/// @brief Pass a message to the GVW error callback if GVW is not initialized.
void AssertInitialization();

/********************************    Monitor    *******************************/
using monitor_public_constructor = public_constructor<monitor>;

/********************************    Window    ********************************/
using window_public_constructor = public_constructor<window>;

enum struct window_input_mode;
enum struct window_input_mode_cursor;
using window_input_mode_sticky_keys = internal::glfw_bool;
using window_input_mode_sticky_mouse_buttons = internal::glfw_bool;

/// @brief Returns the GLFW window user pointer for a specific window.
/// @warning GLFW must be initialized.
[[nodiscard]] void* GetUserPointer(GLFWwindow* Window);

/********************************    Cursor    ********************************/
using cursor_public_constructor = public_constructor<cursor>;

/********************************    Shader    ********************************/
using shader_public_constructor = public_constructor<shader>;

using vertex_shader_public_constructor = public_constructor<vertex_shader>;

using fragment_shader_public_constructor = public_constructor<fragment_shader>;

/********************************    Buffer    ********************************/
using buffer_public_constructor = public_constructor<buffer>;

/******************************    Render Pass    *****************************/
using render_pass_public_constructor = public_constructor<render_pass>;

/*******************************    Swapchain    ******************************/
using swapchain_public_constructor = public_constructor<swapchain>;

/*******************************    Pipeline    *******************************/
using pipeline_public_constructor = public_constructor<pipeline>;

/********************************    Device    ********************************/
using device_public_constructor = public_constructor<device>;

/********************************    Global    ********************************/
namespace global {
extern instance_ptr GVW_INSTANCE;
extern std::mutex GLFW_MUTEX;
/// @todo Use internal::global::CONSOLE_MUTEX.
extern std::mutex CONSOLE_MUTEX;
extern instance_verbose_callback VERBOSE_CALLBACK;
extern std::mutex VERBOSE_CALLBACK_MUTEX;
extern instance_info_callback INFO_CALLBACK;
extern std::mutex INFO_CALLBACK_MUTEX;
extern instance_warning_callback WARNING_CALLBACK;
extern std::mutex WARNING_CALLBACK_MUTEX;
extern instance_error_callback ERROR_CALLBACK;
extern std::mutex ERROR_CALLBACK_MUTEX;
extern instance_glfw_error_callback GLFW_ERROR_CALLBACK;
extern std::vector<instance_joystick_event> JOYSTICK_EVENTS;
extern std::mutex JOYSTICK_EVENTS_MUTEX;
} // namespace global

} // namespace gvw::internal

// Trailing local includes
#include "internal.ipp"