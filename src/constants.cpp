// Standard includes
#include <iostream>
#include <stdexcept>

// Local includes
#include "gvw.hpp"

namespace gvw {

/*******************************    Instance    *******************************/
const instance_creation_flags instance_creation_flags_config::NONE = {};
const instance_creation_flags
    instance_creation_flags_config::ENUMERATE_PORTABILITY_KHR =
        vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;

const instance_creation_hints_info instance_creation_hints_info_config::DEFAULT;

const instance_creation_hints instance_creation_hints_config::DEFAULT;

const instance_application_info instance_application_info_config::DEFAULT;

const instance_debug_utils_messenger_info
    instance_debug_utils_messenger_info_config::DEFAULT;

const instance_debug_utils_messenger_callback
    instance_debug_utils_messenger_callback_config::NONE = nullptr;
const instance_debug_utils_messenger_callback
    instance_debug_utils_messenger_callback_config::
        FORWARD_TO_WARNING_AND_ERROR_CALLBACKS =
            internal::DebugUtilsMessengerCallbackTemplate<
                [](VkDebugUtilsMessageSeverityFlagBitsEXT Message_Severity,
                   const char* Message) {
                    switch (Message_Severity) {
                        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                        default:
                            std::cout << Message << "\n";
                            break;
                        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                            WarningCallback(Message);
                            break;
                        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                            ErrorCallback(Message);
                            break;
                    }
                }>;

const instance_glfw_error_callback instance_glfw_error_callback_config::NONE =
    nullptr;
const instance_glfw_error_callback
    instance_glfw_error_callback_config::FORWARD_TO_WARNING_CALLBACK =
        internal::GlfwErrorCallbackTemplate<[](const char* Message) {
            WarningCallback(Message);
        }>;
const instance_glfw_error_callback
    instance_glfw_error_callback_config::FORWARD_TO_ERROR_CALLBACK =
        internal::GlfwErrorCallbackTemplate<[](const char* Message) {
            ErrorCallback(Message);
        }>;

const instance_layers instance_layers_config::NONE;
const instance_layers instance_layers_config::VALIDATION = {
#ifdef GVW_VULKAN_VALIDATION_LAYERS
    "VK_LAYER_KHRONOS_validation"
#endif
};

const instance_extensions instance_extensions_config::NONE;
const instance_extensions
    instance_extensions_config::PORTABILITY_AND_DEBUG_UTILS = {
        VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };

const instance_warning_callback instance_warning_callback_config::NONE =
    nullptr;
const instance_warning_callback instance_warning_callback_config::COUT =
    internal::WarningCallbackTemplate<[](const char* Message) {
        std::scoped_lock<std::mutex> lock(internal::global::CONSOLE_MUTEX);
        std::cout << Message << std::endl;
    }>;
const instance_warning_callback instance_warning_callback_config::CLOG =
    internal::WarningCallbackTemplate<[](const char* Message) {
        std::scoped_lock<std::mutex> lock(internal::global::CONSOLE_MUTEX);
        std::clog << Message << std::endl;
    }>;
const instance_warning_callback instance_warning_callback_config::CERR =
    internal::WarningCallbackTemplate<[](const char* Message) {
        std::scoped_lock<std::mutex> lock(internal::global::CONSOLE_MUTEX);
        std::cerr << Message << "\n";
    }>;
const instance_warning_callback instance_warning_callback_config::COUT_THROW =
    internal::WarningCallbackTemplate<[](const char* Message) {
        std::scoped_lock<std::mutex> lock(internal::global::CONSOLE_MUTEX);
        std::cout << Message << std::endl;
        throw std::runtime_error(Message);
    }>;
const instance_warning_callback instance_warning_callback_config::CLOG_THROW =
    internal::WarningCallbackTemplate<[](const char* Message) {
        std::scoped_lock<std::mutex> lock(internal::global::CONSOLE_MUTEX);
        std::clog << Message << std::endl;
        throw std::runtime_error(Message);
    }>;
const instance_warning_callback instance_warning_callback_config::CERR_THROW =
    internal::WarningCallbackTemplate<[](const char* Message) {
        std::scoped_lock<std::mutex> lock(internal::global::CONSOLE_MUTEX);
        std::cerr << Message << "\n";
        throw std::runtime_error(Message);
    }>;

const instance_error_callback instance_error_callback_config::NONE = nullptr;
const instance_error_callback instance_error_callback_config::COUT =
    internal::ErrorCallbackTemplate<[](const char* Message) {
        std::scoped_lock<std::mutex> lock(internal::global::CONSOLE_MUTEX);
        std::cout << Message << std::endl;
    }>;
const instance_error_callback instance_error_callback_config::CLOG =
    internal::ErrorCallbackTemplate<[](const char* Message) {
        std::scoped_lock<std::mutex> lock(internal::global::CONSOLE_MUTEX);
        std::clog << Message << std::endl;
    }>;
const instance_error_callback instance_error_callback_config::CERR =
    internal::ErrorCallbackTemplate<[](const char* Message) {
        std::scoped_lock<std::mutex> lock(internal::global::CONSOLE_MUTEX);
        std::cerr << Message << "\n";
    }>;
const instance_error_callback instance_error_callback_config::COUT_THROW =
    internal::ErrorCallbackTemplate<[](const char* Message) {
        std::scoped_lock<std::mutex> lock(internal::global::CONSOLE_MUTEX);
        std::cout << Message << std::endl;
        throw std::runtime_error(Message);
    }>;
const instance_error_callback instance_error_callback_config::CLOG_THROW =
    internal::ErrorCallbackTemplate<[](const char* Message) {
        std::scoped_lock<std::mutex> lock(internal::global::CONSOLE_MUTEX);
        std::clog << Message << std::endl;
        throw std::runtime_error(Message);
    }>;
const instance_error_callback instance_error_callback_config::CERR_THROW =
    internal::ErrorCallbackTemplate<[](const char* Message) {
        std::scoped_lock<std::mutex> lock(internal::global::CONSOLE_MUTEX);
        std::cerr << Message << "\n";
        throw std::runtime_error(Message);
    }>;

const instance_joystick_event_callback
    instance_joystick_event_callback_config::NONE = nullptr;
const instance_joystick_event_callback
    instance_joystick_event_callback_config::APPEND_TO_JOYSTICK_EVENT_BUFFER =
        (instance_joystick_event_callback)[](int JID, int Event)
{
    std::scoped_lock<std::mutex> lock(internal::global::GLFW_MUTEX);
    internal::global::JOYSTICK_EVENTS.emplace_back(JID, Event);
};

const instance_info instance_info_config::DEFAULT;

/********************************    Monitor    *******************************/

const monitor_gamma monitor_gamma_config::DEFAULT = 1.0F;

const monitor_info monitor_info_config::DEFAULT = nullptr;

/********************************    Window    ********************************/
const window_key_event_callback window_key_event_callback_config::NONE =
    nullptr;
const window_key_event_callback
    window_key_event_callback_config::APPEND_TO_KEY_EVENT_BUFFER =
        (window_key_event_callback)[](GLFWwindow * Window_Handle,
                                      int Key,
                                      int Scancode,
                                      int Action,
                                      int Mods)
{
    auto* windowPtr =
        static_cast<window*>(internal::GetUserPointerNoMutex(Window_Handle));
    std::scoped_lock<std::mutex> lock(windowPtr->keyEventsMutex);
    windowPtr->keyEvents.emplace_back(Key, Scancode, Action, Mods);
};

const window_character_event_callback
    window_character_event_callback_config::NONE = nullptr;
const window_character_event_callback
    window_character_event_callback_config::APPEND_TO_CHARACTER_EVENT_BUFFER =
        (window_character_event_callback)[](GLFWwindow * Window_Handle,
                                            unsigned int Code_Point)
{
    auto* windowPtr =
        static_cast<window*>(internal::GetUserPointerNoMutex(Window_Handle));
    std::scoped_lock<std::mutex> lock(windowPtr->characterEventsMutex);
    windowPtr->characterEvents.emplace_back(Code_Point);
};

const window_cursor_position_event_callback
    window_cursor_position_event_callback_config::NONE = nullptr;
const window_cursor_position_event_callback
    window_cursor_position_event_callback_config::
        APPEND_TO_CURSOR_POSITION_EVENT_BUFFER =
            (window_cursor_position_event_callback)[](GLFWwindow *
                                                          Window_Handle,
                                                      double X_Position,
                                                      double Y_Position)
{
    auto* windowPtr =
        static_cast<window*>(internal::GetUserPointerNoMutex(Window_Handle));
    std::scoped_lock<std::mutex> lock(windowPtr->cursorPositionEventsMutex);
    windowPtr->cursorPositionEvents.emplace_back(X_Position, Y_Position);
};

const window_cursor_enter_event_callback
    window_cursor_enter_event_callback_config::NONE = nullptr;
const window_cursor_enter_event_callback
    window_cursor_enter_event_callback_config::
        APPEND_TO_CURSOR_ENTER_EVENT_BUFFER =
            (window_cursor_enter_event_callback)[](GLFWwindow * Window_Handle,
                                                   int Entered)
{
    auto* windowPtr =
        static_cast<window*>(internal::GetUserPointerNoMutex(Window_Handle));
    std::scoped_lock<std::mutex> lock(windowPtr->cursorEnterEventsMutex);
    windowPtr->cursorEnterEvents.emplace_back(Entered);
};

const window_mouse_button_event_callback
    window_mouse_button_event_callback_config::NONE = nullptr;
const window_mouse_button_event_callback
    window_mouse_button_event_callback_config::
        APPEND_TO_MOUSE_BUTTON_EVENT_BUFFER =
            (window_mouse_button_event_callback)[](GLFWwindow * Window_Handle,
                                                   int Button,
                                                   int Action,
                                                   int Mods)
{
    auto* windowPtr =
        static_cast<window*>(internal::GetUserPointerNoMutex(Window_Handle));
    std::scoped_lock<std::mutex> lock(windowPtr->mouseButtonEventsMutex);
    windowPtr->mouseButtonEvents.emplace_back(Button, Action, Mods);
};

const window_scroll_event_callback window_scroll_event_callback_config::NONE =
    nullptr;
const window_scroll_event_callback
    window_scroll_event_callback_config::APPEND_TO_SCROLL_EVENT_BUFFER =
        (window_scroll_event_callback)[](GLFWwindow * Window_Handle,
                                         double X_Offset,
                                         double Y_Offset)
{
    auto* windowPtr =
        static_cast<window*>(internal::GetUserPointerNoMutex(Window_Handle));
    std::scoped_lock<std::mutex> lock(windowPtr->scrollEventsMutex);
    windowPtr->scrollEvents.emplace_back(X_Offset, Y_Offset);
};

const window_file_drop_event_callback
    window_file_drop_event_callback_config::NONE = nullptr;
const window_file_drop_event_callback
    window_file_drop_event_callback_config::APPEND_TO_FILE_DROP_EVENT_BUFFER =
        (window_file_drop_event_callback)[](GLFWwindow * Window_Handle,
                                            int Count,
                                            const char** Paths)
{
    auto* windowPtr =
        static_cast<window*>(internal::GetUserPointerNoMutex(Window_Handle));
    std::scoped_lock<std::mutex> lock(windowPtr->fileDropEventsMutex);
    windowPtr->fileDropEvents.emplace_back(Count, Paths);
};

const window_close_event_callback window_close_event_callback_config::NONE =
    nullptr;
const window_close_event_callback
    window_close_event_callback_config::APPEND_TO_CLOSE_EVENT_BUFFER =
        (window_close_event_callback)[](GLFWwindow * Window_Handle)
{
    auto* windowPtr =
        static_cast<window*>(internal::GetUserPointerNoMutex(Window_Handle));
    std::scoped_lock<std::mutex> lock(windowPtr->closeEventsMutex);
    windowPtr->closeEvents++;
};

const window_size_event_callback window_size_event_callback_config::NONE =
    nullptr;
const window_size_event_callback
    window_size_event_callback_config::APPEND_TO_SIZE_EVENT_BUFFER =
        (window_size_event_callback)[](GLFWwindow * Window_Handle,
                                       int Width,
                                       int Height)
{
    auto* windowPtr =
        static_cast<window*>(internal::GetUserPointerNoMutex(Window_Handle));
    std::scoped_lock<std::mutex> lock(windowPtr->sizeEventsMutex);
    windowPtr->sizeEvents.emplace_back(Width, Height);
};

const window_framebuffer_size_event_callback
    window_framebuffer_size_event_callback_config::NONE = nullptr;
const window_framebuffer_size_event_callback
    window_framebuffer_size_event_callback_config::
        APPEND_TO_FRAMEBUFFER_SIZE_EVENT_BUFFER =
            (window_framebuffer_size_event_callback)[](GLFWwindow *
                                                           Window_Handle,
                                                       int Width,
                                                       int Height)
{
    auto* windowPtr =
        static_cast<window*>(internal::GetUserPointerNoMutex(Window_Handle));
    std::scoped_lock<std::mutex> lock(windowPtr->framebufferSizeEventsMutex);
    windowPtr->framebufferSizeEvents.emplace_back(Width, Height);
};

const window_content_scale_event_callback
    window_content_scale_event_callback_config::NONE = nullptr;
const window_content_scale_event_callback
    window_content_scale_event_callback_config::
        APPEND_TO_CONTENT_SCALE_EVENT_BUFFER =
            (window_content_scale_event_callback)[](GLFWwindow * Window_Handle,
                                                    float X_Scale,
                                                    float Y_Scale)
{
    auto* windowPtr =
        static_cast<window*>(internal::GetUserPointerNoMutex(Window_Handle));
    std::scoped_lock<std::mutex> lock(windowPtr->contentScaleEventsMutex);
    windowPtr->contentScaleEvents.emplace_back(X_Scale, Y_Scale);
};

const window_position_event_callback
    window_position_event_callback_config::NONE = nullptr;
const window_position_event_callback
    window_position_event_callback_config::APPEND_TO_POSITION_EVENT_BUFFER =
        (window_position_event_callback)[](GLFWwindow * Window_Handle,
                                           int X_Position,
                                           int Y_Position)
{
    auto* windowPtr =
        static_cast<window*>(internal::GetUserPointerNoMutex(Window_Handle));
    std::scoped_lock<std::mutex> lock(windowPtr->positionEventsMutex);
    windowPtr->positionEvents.emplace_back(X_Position, Y_Position);
};

const window_iconify_event_callback window_iconify_event_callback_config::NONE =
    nullptr;
const window_iconify_event_callback
    window_iconify_event_callback_config::APPEND_TO_ICONIFY_EVENT_BUFFER =
        (window_iconify_event_callback)[](GLFWwindow * Window_Handle,
                                          int Iconified)
{
    auto* windowPtr =
        static_cast<window*>(internal::GetUserPointerNoMutex(Window_Handle));
    std::scoped_lock<std::mutex> lock(windowPtr->iconifyEventsMutex);
    windowPtr->iconifyEvents.emplace_back(Iconified);
};

const window_maximize_event_callback
    window_maximize_event_callback_config::NONE = nullptr;
const window_maximize_event_callback
    window_maximize_event_callback_config::APPEND_TO_MAXIMIZE_EVENT_BUFFER =
        (window_maximize_event_callback)[](GLFWwindow * Window_Handle,
                                           int Maximized)
{
    auto* windowPtr =
        static_cast<window*>(internal::GetUserPointerNoMutex(Window_Handle));
    std::scoped_lock<std::mutex> lock(windowPtr->maximizeEventsMutex);
    windowPtr->maximizeEvents.emplace_back(Maximized);
};

const window_focus_event_callback window_focus_event_callback_config::NONE =
    nullptr;
const window_focus_event_callback
    window_focus_event_callback_config::APPEND_TO_FOCUS_EVENT_BUFFER =
        (window_focus_event_callback)[](GLFWwindow * Window_Handle, int Focused)
{
    auto* windowPtr =
        static_cast<window*>(internal::GetUserPointerNoMutex(Window_Handle));
    std::scoped_lock<std::mutex> lock(windowPtr->focusEventsMutex);
    windowPtr->focusEvents.emplace_back(Focused);
};

const window_refresh_event_callback window_refresh_event_callback_config::NONE =
    nullptr;
const window_refresh_event_callback
    window_refresh_event_callback_config::APPEND_TO_REFRESH_EVENT_BUFFER =
        (window_refresh_event_callback)[](GLFWwindow * Window_Handle)
{
    auto* windowPtr =
        static_cast<window*>(internal::GetUserPointerNoMutex(Window_Handle));
    std::scoped_lock<std::mutex> lock(windowPtr->refreshEventsMutex);
    windowPtr->refreshEvents++;
};

const window_event_callbacks window_event_callbacks_config::NONE;
const window_event_callbacks
    window_event_callbacks_config::APPEND_TO_EVENT_BUFFERS = {
        .keyCallback =
            window_key_event_callback_config::APPEND_TO_KEY_EVENT_BUFFER,
        .characterCallback = window_character_event_callback_config::
            APPEND_TO_CHARACTER_EVENT_BUFFER,
        .cursorPositionCallback = window_cursor_position_event_callback_config::
            APPEND_TO_CURSOR_POSITION_EVENT_BUFFER,
        .cursorEnterCallback = window_cursor_enter_event_callback_config::
            APPEND_TO_CURSOR_ENTER_EVENT_BUFFER,
        .mouseButtonCallback = window_mouse_button_event_callback_config::
            APPEND_TO_MOUSE_BUTTON_EVENT_BUFFER,
        .scrollCallback =
            window_scroll_event_callback_config::APPEND_TO_SCROLL_EVENT_BUFFER,
        .fileDropCallback = window_file_drop_event_callback_config::
            APPEND_TO_FILE_DROP_EVENT_BUFFER,
        .closeCallback =
            window_close_event_callback_config::APPEND_TO_CLOSE_EVENT_BUFFER,
        .sizeCallback =
            window_size_event_callback_config::APPEND_TO_SIZE_EVENT_BUFFER,
        .framebufferSizeCallback =
            window_framebuffer_size_event_callback_config::
                APPEND_TO_FRAMEBUFFER_SIZE_EVENT_BUFFER,
        .contentScaleCallback = window_content_scale_event_callback_config::
            APPEND_TO_CONTENT_SCALE_EVENT_BUFFER,
        .positionCallback = window_position_event_callback_config::
            APPEND_TO_POSITION_EVENT_BUFFER,
        .iconifyCallback = window_iconify_event_callback_config::
            APPEND_TO_ICONIFY_EVENT_BUFFER,
        .maximizeCallback = window_maximize_event_callback_config::
            APPEND_TO_MAXIMIZE_EVENT_BUFFER,
        .focusCallback =
            window_focus_event_callback_config::APPEND_TO_FOCUS_EVENT_BUFFER,
        .refreshCallback =
            window_refresh_event_callback_config::APPEND_TO_REFRESH_EVENT_BUFFER
    };

const window_creation_hints_info window_creation_hints_info_config::DEFAULT;

const window_creation_hints window_creation_hints_config::DEFAULT;

const window_title window_title_config::UNTITLED = "";
const window_title window_title_config::BLANK = " ";

const window_size window_size_config::W_500_H_500 = { { 500, 500 } };
const window_size window_size_config::W_640_H_360 = { { 640, 360 } };

const window_size_limit window_size_limit_config::NO_MINIMUM = {
    { GLFW_DONT_CARE, GLFW_DONT_CARE }
};
const window_size_limit window_size_limit_config::NO_MAXIMUM = {
    { GLFW_DONT_CARE, GLFW_DONT_CARE }
};

const window_info window_info_config::DEFAULT;

/********************************    Cursor    ********************************/
const cursor_hotspot cursor_hotspot_config::DEFAULT = { 0, 0 };

const standard_cursor_info standard_cursor_info_config::DEFAULT;

const custom_cursor_info custom_cursor_info_config::DEFAULT;

/********************************    Shader    ********************************/
const shader_info shader_info_config::DEFAULT;

const vertex_shader_info vertex_shader_info_config::DEFAULT;

const fragment_shader_info fragment_shader_info_config::DEFAULT;

/********************************    Buffer    ********************************/
const buffer_info buffer_info_config::DEFAULT;

/******************************    Render Pass    *****************************/
const render_pass_info render_pass_info_config::DEFAULT;

/*******************************    Swapchain    ******************************/
const swapchain_surface_format swapchain_surface_format_config::STANDARD = {
    .format = vk::Format::eB8G8R8A8Srgb,
    .colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear
};

const swapchain_surface_formats swapchain_surface_formats_config::STANDARD = {
    swapchain_surface_format_config::STANDARD
};

const swapchain_present_mode swapchain_present_mode_config::FIFO = {
    vk::PresentModeKHR::eFifo
};
const swapchain_present_mode swapchain_present_mode_config::MAILBOX = {
    vk::PresentModeKHR::eMailbox
};

const swapchain_present_modes swapchain_present_modes_config::FIFO = {
    swapchain_present_mode_config::FIFO
};
const swapchain_present_modes swapchain_present_modes_config::MAILBOX = {
    swapchain_present_mode_config::MAILBOX
};
const swapchain_present_modes
    swapchain_present_modes_config::MAILBOX_OR_FIFO = {
        swapchain_present_mode_config::MAILBOX,
        swapchain_present_mode_config::FIFO
    };

const swapchain_info swapchain_info_config::DEFAULT;

/*******************************    Pipeline    *******************************/
const pipeline_shaders pipeline_shaders_config::NONE;

const pipeline_dynamic_states pipeline_dynamic_states_config::VIEWPORT = {
    vk::DynamicState::eViewport
};
const pipeline_dynamic_states
    pipeline_dynamic_states_config::VIEWPORT_AND_SCISSOR = {
        vk::DynamicState::eViewport,
        vk::DynamicState::eScissor
    };

const pipeline_info pipeline_info_config::DEFAULT;

/********************************    Device    ********************************/
const device_selection_function device_selection_function_config::NONE =
    nullptr;
const device_selection_function
    device_selection_function_config::MINIMUM_FOR_PRESENTATION =
        (device_selection_function)[]( // NOLINT
            const std::vector<device_selection_parameter>&
                Physical_Device_Infos,
            const std::optional<vk::SurfaceKHR>& Window_Surface)
            ->std::vector<device_info>
{
    vk::PhysicalDevice selectedPhysicalDevice;
    vk::SurfaceFormatKHR selectedSurfaceFormat;
    vk::PresentModeKHR selectedPresentMode = {};
    std::vector<device_selection_queue_family_info> selectedQueueFamilyInfos(1);

    int selectedPhysicalDeviceScore = -1;
    int currentPhysicalDeviceScore = 0;
    for (const auto& physicalDeviceInfo : Physical_Device_Infos) {
        const auto& physicalDevice = physicalDeviceInfo.physicalDevice;
        const auto& surfaceFormats = physicalDeviceInfo.surfaceFormats;
        const auto& presentModes = physicalDeviceInfo.presentModes;

        // Score different types of devices based on assumed performance.
        vk::PhysicalDeviceProperties physicalDeviceProperties =
            physicalDevice.getProperties();
        switch (physicalDeviceProperties.deviceType) {
            case vk::PhysicalDeviceType::eDiscreteGpu:
                currentPhysicalDeviceScore = 4;
                break;
            case vk::PhysicalDeviceType::eIntegratedGpu:
                currentPhysicalDeviceScore = 3;
                break;
            case vk::PhysicalDeviceType::eVirtualGpu:
                currentPhysicalDeviceScore = 2;
                break;
            case vk::PhysicalDeviceType::eOther:
                currentPhysicalDeviceScore = 1;
                break;
            case vk::PhysicalDeviceType::eCpu:
                currentPhysicalDeviceScore = 0;
                break;
        }

        // Select the first available surface format.
        vk::SurfaceFormatKHR selectedPhysicalDeviceSurfaceFormat = {};
        if (surfaceFormats.has_value()) {
            selectedPhysicalDeviceSurfaceFormat = surfaceFormats.value().at(0);
        }

        vk::PresentModeKHR selectedPhysicalDevicePresentMode = {};
        if (presentModes.has_value()) {
            int selectedPresentModeScore = -1;
            vk::PresentModeKHR currentPhysicalDevicePresentMode = {};
            int currentPresentModeScore = 0;
            for (const auto& presentMode : presentModes.value()) {
                switch (presentMode) {
                    case vk::PresentModeKHR::eMailbox:
                        currentPresentModeScore = 5; // NOLINT
                        currentPhysicalDevicePresentMode =
                            vk::PresentModeKHR::eMailbox;
                        break;
                    case vk::PresentModeKHR::eFifo:
                        currentPresentModeScore = 4;
                        currentPhysicalDevicePresentMode =
                            vk::PresentModeKHR::eFifo;
                        break;
                    case vk::PresentModeKHR::eFifoRelaxed:
                        currentPresentModeScore = 3;
                        currentPhysicalDevicePresentMode =
                            vk::PresentModeKHR::eFifoRelaxed;
                        break;
                    case vk::PresentModeKHR::eImmediate:
                        currentPresentModeScore = 2;
                        currentPhysicalDevicePresentMode =
                            vk::PresentModeKHR::eImmediate;
                        break;
                    case vk::PresentModeKHR::eSharedContinuousRefresh:
                        currentPresentModeScore = 1;
                        currentPhysicalDevicePresentMode =
                            vk::PresentModeKHR::eSharedContinuousRefresh;
                        break;
                    case vk::PresentModeKHR::eSharedDemandRefresh:
                        currentPresentModeScore = 0;
                        currentPhysicalDevicePresentMode =
                            vk::PresentModeKHR::eSharedDemandRefresh;
                        break;
                }

                if (currentPresentModeScore > selectedPresentModeScore) {
                    selectedPresentModeScore = currentPresentModeScore;
                    selectedPhysicalDevicePresentMode =
                        currentPhysicalDevicePresentMode;
                }
            }
        }

        std::vector<vk::QueueFamilyProperties> queueFamilyProperties =
            physicalDevice.getQueueFamilyProperties();
        if (queueFamilyProperties.empty()) {
            continue;
        }

        // Find queue families that supports both graphics and presentation.
        std::optional<uint32_t> viableGraphicsQueueFamilyIndex;
        std::optional<uint32_t> viablePresentationQueueFamilyIndex;
        for (uint32_t i = 0; i < uint32_t(queueFamilyProperties.size()); ++i) {
            vk::QueueFlags queueFlags = queueFamilyProperties.at(i).queueFlags;
            if (viableGraphicsQueueFamilyIndex.has_value() == false) {
                if (bool(queueFlags & vk::QueueFlagBits::eGraphics)) {
                    viableGraphicsQueueFamilyIndex = i;
                }
            }
            if (Window_Surface.has_value() &&
                (viablePresentationQueueFamilyIndex.has_value() == false)) {
                if (physicalDevice.getSurfaceSupportKHR(
                        i, Window_Surface.value()) != VK_FALSE) {
                    viablePresentationQueueFamilyIndex = i;
                }
            }

            if (viableGraphicsQueueFamilyIndex.has_value() &&
                viablePresentationQueueFamilyIndex.has_value()) {
                if (viableGraphicsQueueFamilyIndex.value() ==
                    viablePresentationQueueFamilyIndex.value()) {
                    break;
                }
            }
        }
        if ((viableGraphicsQueueFamilyIndex.has_value() == false) ||
            (viablePresentationQueueFamilyIndex.has_value() == false)) {
            continue;
        }

        if (currentPhysicalDeviceScore > selectedPhysicalDeviceScore) {
            selectedPhysicalDeviceScore = currentPhysicalDeviceScore;
            selectedPhysicalDevice = physicalDevice;
            selectedSurfaceFormat = selectedPhysicalDeviceSurfaceFormat;
            selectedPresentMode = selectedPhysicalDevicePresentMode;

            if (viableGraphicsQueueFamilyIndex.value() ==
                viablePresentationQueueFamilyIndex.value()) {
                selectedQueueFamilyInfos.resize(1);
            }

            selectedQueueFamilyInfos.at(0) = {
                .createInfo = { .queueFamilyIndex =
                                    viableGraphicsQueueFamilyIndex.value(),
                                .queueCount = 1,
                                .pQueuePriorities =
                                    &device_queue_priority_config::HIGH },
                .properties = queueFamilyProperties.at(
                    viableGraphicsQueueFamilyIndex.value())
            };

            if (viableGraphicsQueueFamilyIndex !=
                viablePresentationQueueFamilyIndex) {
                selectedQueueFamilyInfos.resize(2);
                selectedQueueFamilyInfos.at(1) = {
                    .createInfo = { .queueFamilyIndex =
                                        viablePresentationQueueFamilyIndex
                                            .value(),
                                    .queueCount = 1,
                                    .pQueuePriorities =
                                        &device_queue_priority_config::HIGH },
                    .properties = queueFamilyProperties.at(
                        viablePresentationQueueFamilyIndex.value())
                };
            }
        }
    }

    return { device_info{ .physicalDevice = selectedPhysicalDevice,
                          .surfaceFormat = selectedSurfaceFormat,
                          .presentMode = selectedPresentMode,
                          .queueFamilyInfos = selectedQueueFamilyInfos } };
};

const device_features device_features_config::NONE;

const device_extensions device_extensions_config::NONE;
const device_extensions device_extensions_config::SWAPCHAIN = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const device_queue_priority device_queue_priority_config::HIGH = 1.0F;

const device_info device_info_config::DEFAULT;

const device_selection_info device_selection_info_config::DEFAULT;

const std::vector<vk::VertexInputBindingDescription>
    NO_VERTEX_BINDING_DESCRIPTIONS;

const std::vector<vk::VertexInputAttributeDescription>
    NO_VERTEX_ATTRIBUTE_DESCRIPTIONS;

const std::vector<vertex> NO_VERTICES;

} // namespace gvw