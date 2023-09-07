#pragma once

/**
 * @file gvw.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief Global type, constant, and function declarations.
 * @date 2023-07-05
 */

// Standard includes
#include <cstddef>
#include <memory>
#include <vector>
#include <optional>
#include <mutex>

// External includes
#define VULKAN_HPP_NAMESPACE vk
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace gvw {

/// @brief An x and y value pair. Structurally identical to `gvw::area`.
/// @tparam T The type of the x and y values.
template<typename T>
struct coordinate;

/// @brief A width and height value pair. Structurally identical to
/// `gvw::coordinate`.
/// @tparam T The type of the width and height values.
template<typename T>
struct area;

/// @brief Version major, minor, and revision.
struct version;

/// @brief Creation hint types.
class creation_hint_bool;
class creation_hint_int;
using creation_hint_string = const char*;
enum struct creation_hint_client_api;
enum struct creation_hint_context_creation_api;
enum struct creation_hint_context_robustness;
enum struct creation_hint_context_release_behavior;
enum struct creation_hint_opengl_profile;

/// @brief GLM type aliases.
using xy = glm::vec2;
using xyz = glm::vec3;
using rgb = glm::vec3;
using rgba = glm::vec4;
using xy_rgb = std::pair<xy, rgb>;
using xyz_rgb = std::pair<xyz, rgb>;
using xy_rgba = std::pair<xy, rgba>;
using xyz_rgba = std::pair<xyz, rgba>;

/// @todo Figure out what to do with these "temporary" types.
extern const std::vector<vk::VertexInputBindingDescription>
    NO_VERTEX_BINDING_DESCRIPTIONS;
extern const std::vector<vk::VertexInputAttributeDescription>
    NO_VERTEX_ATTRIBUTE_DESCRIPTIONS;
extern const std::vector<xy_rgb> NO_VERTICES;

/// @brief Reads a file from the file system.
/// @tparam T Output buffer type. Almost always `char`.
template<typename T = char>
[[nodiscard]] std::vector<T> ReadFile(const char* Absolute_Path);

/// @brief Returns the GLFW version used at runtime.
[[nodiscard]] version GetGlfwRuntimeVersion() noexcept;

/// @brief Returns the GLFW version used to compile GVW.
[[nodiscard]] version GetGlfwCompiletimeVersion() noexcept;

/*********************************    Image    ********************************/
class image;
using image_ptr = std::shared_ptr<image>;
struct image_file_info;
struct image_memory_info;

template<typename T>
[[nodiscard]] image_ptr CreateImage(const T& Info);

/*****************************    GVW Instance    *****************************/
class instance;
using instance_ptr = std::shared_ptr<instance>;
struct instance_info;
namespace instance_info_config {
extern const instance_info DEFAULT;
} // namespace instance_info_config

/// @brief Vulkan instance creation flags.
using instance_creation_flags = vk::InstanceCreateFlagBits;
namespace instance_creation_flags_config {
extern const instance_creation_flags NONE;
extern const instance_creation_flags ENUMERATE_PORTABILITY_KHR;
} // namespace instance_creation_flags_config

/// @brief Initialization hints for GLFW.
struct instance_creation_hints_info;
namespace instance_creation_hints_info_config {
extern const instance_creation_hints_info DEFAULT;
} // namespace instance_creation_hints_info_config
struct instance_creation_hints;
namespace instance_creation_hints_config {
extern const instance_creation_hints DEFAULT;
} // namespace instance_creation_hints_config

/// @brief Application information for Vulkan instance creation.
struct instance_application_info;
namespace instance_application_info_config {
extern const instance_application_info DEFAULT;
} // namespace instance_application_info_config

/// @todo Add `application` class.

using instance_debug_utils_message_severity =
    vk::DebugUtilsMessageSeverityFlagsEXT;
namespace instance_debug_utils_message_severity_config {
extern const instance_debug_utils_message_severity NONE;
extern const instance_debug_utils_message_severity ERROR;
extern const instance_debug_utils_message_severity ERROR_WARNING;
extern const instance_debug_utils_message_severity ERROR_WARNING_INFO;
extern const instance_debug_utils_message_severity ERROR_WARNING_INFO_VERBOSE;
extern const instance_debug_utils_message_severity ALL;
} // namespace instance_debug_utils_message_severity_config

using instance_debug_utils_message_type = vk::DebugUtilsMessageTypeFlagsEXT;
namespace instance_debug_utils_message_type_config {
extern const instance_debug_utils_message_type NONE;
extern const instance_debug_utils_message_type VALIDATION;
extern const instance_debug_utils_message_type VALIDATION_GENERAL;
extern const instance_debug_utils_message_type VALIDATION_GENERAL_PERFORMANCE;
extern const instance_debug_utils_message_type
    VALIDATION_GENERAL_PERFORMANCE_DEVICE_ADDRESS_BINDING;
extern const instance_debug_utils_message_type ALL;
} // namespace instance_debug_utils_message_type_config

/// @brief Vulkan debug utility messenger callbacks.
/// @remark These functions should only be called by Vulkan.
using instance_debug_utils_messenge_callback =
    PFN_vkDebugUtilsMessengerCallbackEXT;
namespace instance_debug_utils_messenge_callback_config {
extern const instance_debug_utils_messenge_callback NONE;
extern const instance_debug_utils_messenge_callback FORWARD_TO_GVW_CALLBACKS;
} // namespace instance_debug_utils_messenge_callback_config

/// @brief Initialization information for the Vulkan debug utility messenger.
struct instance_debug_utils_messenger_info;
namespace instance_debug_utils_messenger_info_config {
extern const instance_debug_utils_messenger_info DEFAULT;
} // namespace instance_debug_utils_messenger_info_config

/// @brief Debug utils messenger callback template.
using instance_debug_utils_messenger_callback_print_function =
    void (*)(VkDebugUtilsMessageSeverityFlagBitsEXT, const char*);

/// @brief Vulkan instance layer.
using instance_layer = const char*;
namespace instance_layer_config {
extern const instance_layer NONE;
/// @todo Add more instance_layer configs.
} // namespace instance_layer_config
using instance_layers = std::vector<instance_layer>;
namespace instance_layers_config {
extern const instance_layers NONE;
extern const instance_layers VALIDATION;
} // namespace instance_layers_config

/// @brief Vulkan instance extensions.
using instance_extension = const char*;
namespace instance_extension_config {
extern const instance_extension NONE;
/// @todo Add more instance_extension configs.
} // namespace instance_extension_config
using instance_extensions = std::vector<instance_extension>;
namespace instance_extensions_config {
extern const instance_extensions NONE;
extern const instance_extensions PORTABILITY_AND_DEBUG_UTILS;
} // namespace instance_extensions_config

/// @brief GVW callbacks.
using instance_verbose_callback = void (*)(const char*);
namespace instance_verbose_callback_config {
extern const instance_verbose_callback NONE;
extern const instance_verbose_callback COUT;
extern const instance_verbose_callback CLOG;
extern const instance_verbose_callback CERR;
extern const instance_verbose_callback COUT_THROW;
extern const instance_verbose_callback CLOG_THROW;
extern const instance_verbose_callback CERR_THROW;
} // namespace instance_verbose_callback_config
void SetVerboseCallback(instance_verbose_callback Verbose_Callback);
void VerboseCallback(const char* Message);

using instance_info_callback = instance_verbose_callback;
namespace instance_info_callback_config {
extern const instance_info_callback NONE;
extern const instance_info_callback COUT;
extern const instance_info_callback CLOG;
extern const instance_info_callback CERR;
extern const instance_info_callback COUT_THROW;
extern const instance_info_callback CLOG_THROW;
extern const instance_info_callback CERR_THROW;
} // namespace instance_info_callback_config
void SetInfoCallback(instance_info_callback Info_Callback);
void InfoCallback(const char* Message);

using instance_warning_callback = instance_info_callback;
namespace instance_warning_callback_config {
extern const instance_warning_callback NONE;
extern const instance_warning_callback COUT;
extern const instance_warning_callback CLOG;
extern const instance_warning_callback CERR;
extern const instance_warning_callback COUT_THROW;
extern const instance_warning_callback CLOG_THROW;
extern const instance_warning_callback CERR_THROW;
} // namespace instance_warning_callback_config
void SetWarningCallback(instance_warning_callback Warning_Callback);
void WarningCallback(const char* Message);

using instance_error_callback = instance_warning_callback;
namespace instance_error_callback_config {
extern const instance_error_callback NONE;
extern const instance_error_callback COUT;
extern const instance_error_callback CLOG;
extern const instance_error_callback CERR;
extern const instance_error_callback COUT_THROW;
extern const instance_error_callback CLOG_THROW;
extern const instance_error_callback CERR_THROW;
} // namespace instance_error_callback_config
void SetErrorCallback(instance_error_callback Error_Callback);
void ErrorCallback(const char* Message);

using instance_glfw_error_callback = void (*)(int, const char*); // GLFWerrorfun
namespace instance_glfw_error_callback_config {
extern const instance_glfw_error_callback NONE;
extern const instance_glfw_error_callback FORWARD_TO_VERBOSE_CALLBACK;
extern const instance_glfw_error_callback FORWARD_TO_INFO_CALLBACK;
extern const instance_glfw_error_callback FORWARD_TO_WARNING_CALLBACK;
extern const instance_glfw_error_callback FORWARD_TO_ERROR_CALLBACK;
} // namespace instance_glfw_error_callback_config
void SetGlfwCallback(instance_glfw_error_callback GLFW_Error_Callback);
void GlfwErrorCallback(int Error_Code, const char* Message);

/// @brief Joystick ID and event type of a joystick event.
struct instance_joystick_event;

using instance_joystick_event_callback = void (*)(int, int); // GLFWjoystickfun
namespace instance_joystick_event_callback_config {
extern const instance_joystick_event_callback NONE;
extern const instance_joystick_event_callback APPEND_TO_JOYSTICK_EVENT_BUFFER;
} // namespace instance_joystick_event_callback_config

/// @brief Initialize GVW with the default configuration.
/// @remark Immediately returns if GVW is already initialized.
instance_ptr CreateInstance();

/// @brief Initialize GVW with a custom configuration.
/// @remark Immediately returns if GVW is already initialized.
instance_ptr CreateInstance(const instance_info& Instance_Info);

/// @brief Returns the GVW instance.
/// @remark Returns nullptr if GVW is not initialized.
instance_ptr GetInstance();

/********************************    Monitor    *******************************/
class monitor;
using monitor_ptr = std::shared_ptr<monitor>;
using monitor_info = GLFWmonitor*;
namespace monitor_info_config {
extern const monitor_info DEFAULT;
} // namespace monitor_info_config

/// @brief Gamma value for monitors.
using monitor_gamma = float;
namespace monitor_gamma_config {
extern const monitor_gamma DEFAULT;
} // namespace monitor_gamma_config

/********************************    Window    ********************************/
class window;
using window_ptr = std::shared_ptr<window>;
struct window_info;
namespace window_info_config {
extern const window_info DEFAULT;
} // namespace window_info_config

enum struct window_key;

enum struct window_key_action;

struct window_key_event;
using window_character_event = unsigned int;
using window_cursor_position_event = coordinate<double>;
using window_cursor_enter_event = int;
struct window_mouse_button_event;
using window_scroll_event = coordinate<double>;
struct window_file_drop_event;
using window_size_event = area<int>;
using window_framebuffer_size_event = area<int>;
using window_content_scale_event = coordinate<float>;
using window_position_event = coordinate<int>;
using window_iconify_event = int;
using window_maximize_event = int;
using window_focus_event = int;

using window_key_event_callback =
    void (*)(GLFWwindow*, int, int, int, int); // GLFWkeyfun
namespace window_key_event_callback_config {
extern const window_key_event_callback NONE;
extern const window_key_event_callback APPEND_TO_KEY_EVENT_BUFFER;
} // namespace window_key_event_callback_config
using window_character_event_callback = void (*)(GLFWwindow*,
                                                 unsigned int); // GLFWcharfun
namespace window_character_event_callback_config {
extern const window_character_event_callback NONE;
extern const window_character_event_callback APPEND_TO_CHARACTER_EVENT_BUFFER;
} // namespace window_character_event_callback_config
using window_cursor_position_event_callback =
    void (*)(GLFWwindow*, double, double); // GLFWcursorposfun
namespace window_cursor_position_event_callback_config {
extern const window_cursor_position_event_callback NONE;
extern const window_cursor_position_event_callback
    APPEND_TO_CURSOR_POSITION_EVENT_BUFFER;
} // namespace window_cursor_position_event_callback_config
using window_cursor_enter_event_callback = void (*)(GLFWwindow*,
                                                    int); // GLFWcursorenterfun
namespace window_cursor_enter_event_callback_config {
extern const window_cursor_enter_event_callback NONE;
extern const window_cursor_enter_event_callback
    APPEND_TO_CURSOR_ENTER_EVENT_BUFFER;
} // namespace window_cursor_enter_event_callback_config
using window_mouse_button_event_callback = void (*)(GLFWwindow*,
                                                    int,
                                                    int,
                                                    int); // GLFWmousebuttonfun
namespace window_mouse_button_event_callback_config {
extern const window_mouse_button_event_callback NONE;
extern const window_mouse_button_event_callback
    APPEND_TO_MOUSE_BUTTON_EVENT_BUFFER;
} // namespace window_mouse_button_event_callback_config
using window_scroll_event_callback = void (*)(GLFWwindow*,
                                              double,
                                              double); // GLFWscrollfun
namespace window_scroll_event_callback_config {
extern const window_scroll_event_callback NONE;
extern const window_scroll_event_callback APPEND_TO_SCROLL_EVENT_BUFFER;
} // namespace window_scroll_event_callback_config
using window_file_drop_event_callback = void (*)(GLFWwindow*,
                                                 int,
                                                 const char**); // GLFWdropfun
namespace window_file_drop_event_callback_config {
extern const window_file_drop_event_callback NONE;
extern const window_file_drop_event_callback APPEND_TO_FILE_DROP_EVENT_BUFFER;
} // namespace window_file_drop_event_callback_config
using window_close_event_callback = void (*)(GLFWwindow*); // GLFWwindowclosefun
namespace window_close_event_callback_config {
extern const window_close_event_callback NONE;
extern const window_close_event_callback APPEND_TO_CLOSE_EVENT_BUFFER;
} // namespace window_close_event_callback_config
using window_size_event_callback = void (*)(GLFWwindow*,
                                            int,
                                            int); // GLFWwindowsizefun
namespace window_size_event_callback_config {
extern const window_size_event_callback NONE;
extern const window_size_event_callback APPEND_TO_SIZE_EVENT_BUFFER;
} // namespace window_size_event_callback_config
using window_framebuffer_size_event_callback =
    void (*)(GLFWwindow*, int, int); // GLFWframebuffersizefun
namespace window_framebuffer_size_event_callback_config {
extern const window_framebuffer_size_event_callback NONE;
extern const window_framebuffer_size_event_callback
    APPEND_TO_FRAMEBUFFER_SIZE_EVENT_BUFFER;
} // namespace window_framebuffer_size_event_callback_config
using window_content_scale_event_callback =
    void (*)(GLFWwindow*, float, float); // GLFWwindowcontentscalefun
namespace window_content_scale_event_callback_config {
extern const window_content_scale_event_callback NONE;
extern const window_content_scale_event_callback
    APPEND_TO_CONTENT_SCALE_EVENT_BUFFER;
} // namespace window_content_scale_event_callback_config
using window_position_event_callback = void (*)(GLFWwindow*,
                                                int,
                                                int); // GLFWwindowposfun
namespace window_position_event_callback_config {
extern const window_position_event_callback NONE;
extern const window_position_event_callback APPEND_TO_POSITION_EVENT_BUFFER;
} // namespace window_position_event_callback_config
using window_iconify_event_callback = void (*)(GLFWwindow*,
                                               int); // GLFWwindowiconifyfun
namespace window_iconify_event_callback_config {
extern const window_iconify_event_callback NONE;
extern const window_iconify_event_callback APPEND_TO_ICONIFY_EVENT_BUFFER;
} // namespace window_iconify_event_callback_config
using window_maximize_event_callback = void (*)(GLFWwindow*,
                                                int); // GLFWwindowmaximizefun
namespace window_maximize_event_callback_config {
extern const window_maximize_event_callback NONE;
extern const window_maximize_event_callback APPEND_TO_MAXIMIZE_EVENT_BUFFER;
} // namespace window_maximize_event_callback_config
using window_focus_event_callback = void (*)(GLFWwindow*,
                                             int); // GLFWwindowfocusfun
namespace window_focus_event_callback_config {
extern const window_focus_event_callback NONE;
extern const window_focus_event_callback APPEND_TO_FOCUS_EVENT_BUFFER;
} // namespace window_focus_event_callback_config
using window_refresh_event_callback =
    void (*)(GLFWwindow*); // GLFWwindowrefreshfun
namespace window_refresh_event_callback_config {
extern const window_refresh_event_callback NONE;
extern const window_refresh_event_callback APPEND_TO_REFRESH_EVENT_BUFFER;
} // namespace window_refresh_event_callback_config

struct window_event_callbacks;
namespace window_event_callbacks_config {
extern const window_event_callbacks NONE;
extern const window_event_callbacks APPEND_TO_EVENT_BUFFERS;
} // namespace window_event_callbacks_config

struct window_creation_hints_info;
namespace window_creation_hints_info_config {
extern const window_creation_hints_info DEFAULT;
} // namespace window_creation_hints_info_config
struct window_creation_hints;
namespace window_creation_hints_config {
extern const window_creation_hints DEFAULT;
} // namespace window_creation_hints_config

using window_title = const char*;
namespace window_title_config {
extern const window_title UNTITLED;
extern const window_title BLANK;
} // namespace window_title_config

using window_size = area<int>;
namespace window_size_config {
extern const window_size W_500_H_500;
extern const window_size W_640_H_360;
} // namespace window_size_config

using window_size_limit = area<int>;
namespace window_size_limit_config {
extern const window_size_limit NO_MINIMUM;
extern const window_size_limit NO_MAXIMUM;
} // namespace window_size_limit_config

/********************************    Cursor    ********************************/
class cursor;
using cursor_ptr = std::shared_ptr<cursor>;

using cursor_hotspot = coordinate<int>;
namespace cursor_hotspot_config {
extern const cursor_hotspot DEFAULT;
} // namespace cursor_hotspot_config

enum struct cursor_standard_shape;

struct cursor_custom_shape_info;
namespace cursor_custom_shape_info_config {
extern const cursor_custom_shape_info DEFAULT;
} // namespace cursor_custom_shape_info_config

/********************************    Shader    ********************************/
class shader;
using shader_ptr = std::shared_ptr<shader>;
struct shader_info;
namespace shader_info_config {
extern const shader_info DEFAULT;
} // namespace shader_info_config

class vertex_shader;
using vertex_shader_ptr = std::shared_ptr<vertex_shader>;
struct vertex_shader_info;
namespace vertex_shader_info_config {
extern const vertex_shader_info DEFAULT;
} // namespace vertex_shader_info_config

class fragment_shader;
using fragment_shader_ptr = std::shared_ptr<fragment_shader>;
struct fragment_shader_info;
namespace fragment_shader_info_config {
extern const fragment_shader_info DEFAULT;
} // namespace fragment_shader_info_config

/********************************    Buffer    ********************************/
class buffer;
using buffer_ptr = std::shared_ptr<buffer>;
struct buffer_info;
namespace buffer_info_config {
extern const buffer_info DEFAULT;
} // namespace buffer_info_config

/******************************    Render Pass    *****************************/
class render_pass;
using render_pass_ptr = std::shared_ptr<render_pass>;
struct render_pass_info;
namespace render_pass_info_config {
extern const render_pass_info DEFAULT;
} // namespace render_pass_info_config

/*******************************    Swapchain    ******************************/
class swapchain;
using swapchain_ptr = std::shared_ptr<swapchain>;
struct swapchain_info;
namespace swapchain_info_config {
extern const swapchain_info DEFAULT;
} // namespace swapchain_info_config

/// @brief Window surface format.
using swapchain_surface_format = vk::SurfaceFormatKHR;
namespace swapchain_surface_format_config {
extern const swapchain_surface_format STANDARD;
} // namespace swapchain_surface_format_config
using swapchain_surface_formats = std::vector<swapchain_surface_format>;
namespace swapchain_surface_formats_config {
extern const swapchain_surface_formats STANDARD;
} // namespace swapchain_surface_formats_config

/// @brief Window present mode.
using swapchain_present_mode = vk::PresentModeKHR;
namespace swapchain_present_mode_config {
extern const swapchain_present_mode FIFO;
extern const swapchain_present_mode MAILBOX;
} // namespace swapchain_present_mode_config
using swapchain_present_modes = std::vector<swapchain_present_mode>;
namespace swapchain_present_modes_config {
extern const swapchain_present_modes FIFO;
extern const swapchain_present_modes MAILBOX;
extern const swapchain_present_modes MAILBOX_OR_FIFO;
} // namespace swapchain_present_modes_config

/*******************************    Pipeline    *******************************/
class pipeline;
using pipeline_ptr = std::shared_ptr<pipeline>;
struct pipeline_info;
namespace pipeline_info_config {
extern const pipeline_info DEFAULT;
} // namespace pipeline_info_config

/// @brief Collection of shaders for creating a pipeline.
struct pipeline_shaders;
namespace pipeline_shaders_config {
extern const pipeline_shaders NONE;
} // namespace pipeline_shaders_config

/// @brief Dynamic states.
using pipeline_dynamic_states = std::vector<vk::DynamicState>;
namespace pipeline_dynamic_states_config {
extern const pipeline_dynamic_states VIEWPORT;
extern const pipeline_dynamic_states VIEWPORT_AND_SCISSOR;
} // namespace pipeline_dynamic_states_config

/********************************    Device    ********************************/
class device;
using device_ptr = std::shared_ptr<device>;
struct device_info;
namespace device_info_config {
extern const device_info DEFAULT;
} // namespace device_info_config

struct device_selection_info;
namespace device_selection_info_config {
extern const device_selection_info DEFAULT;
} // namespace device_selection_info_config
struct device_selection_queue_family_info;
struct device_selection_parameter;
using device_selection_function =
    std::vector<device_info> (*)(const std::vector<device_selection_parameter>&,
                                 const std::optional<vk::SurfaceKHR>&);
namespace device_selection_function_config {
extern const device_selection_function NONE;
extern const device_selection_function MINIMUM_FOR_PRESENTATION;
} // namespace device_selection_function_config

/// @brief Physical device features.
/// @todo Consider naming this device_selection_features.
using device_features = vk::PhysicalDeviceFeatures;
namespace device_features_config {
extern const device_features NONE;
} // namespace device_features_config

/// @brief Logical device extensions.
/// @todo Consider naming this device_selection_extensions (if possible to
/// select for logical device extensions during physical device selection).
using device_extensions = std::vector<const char*>;
namespace device_extensions_config {
extern const device_extensions NONE;
extern const device_extensions SWAPCHAIN;
} // namespace device_extensions_config

/// @brief Logical device queue priority.
/// @todo Move this to within the `queue` class once it exists.
using device_queue_priority = float;
namespace device_queue_priority_config {
extern const device_queue_priority HIGH;
} // namespace device_queue_priority_config

} // namespace gvw

// Local definition includes
#include "internal.hpp"
#include "gvw.ipp"
#include "instance.hpp"
#include "monitor.hpp"
#include "window.hpp"
#include "device.hpp"