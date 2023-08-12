#pragma once

/**
 * @file gvw.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief GVW root header file.
 * @date 2023-07-05
 */

// Standard includes
#include <vector>
#include <list>
#include <optional>
#include <mutex>
#include <memory>
#include <atomic>
#include <iostream>
#include <fstream>

// External includes
#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <glslang/Include/glslang_c_interface.h>
#include <glm/glm.hpp>

// Leading local includes

/// @brief All GVW constants, functions, and classes are contained within this
/// namespace.
class gvw
{
    ////////////////////////////////////////////////////////////
    ///                     Private Types                    ///
    ////////////////////////////////////////////////////////////

    /// @brief Implicitly deletes the default constructor of derived types.
    struct no_constructor
    {
        no_constructor() = delete;
    };

    // Variants of classes but with public constructors instead of private
    // constructors.
    class gvw_public_constructor;
    class window_public_constructor;
    class monitor_public_constructor;
    class device_public_constructor;

    /// @brief Compiles GLSL source code to SPIR-V binary code.
    struct glsl_compiler;

  public:
    ////////////////////////////////////////////////////////////
    ///                     Public Types                     ///
    ////////////////////////////////////////////////////////////

    /// @brief Version major, minor, and revision.
    struct version;

    // Function signatures.
    using gvw_error_callback = void (*)(const char*);
    using int_hint_function = void (*)(int, int);
    using string_hint_function = void (*)(int, const char*);

    /// @brief GLFW hint ID and default value.
    /// @tparam T The value type of the hint. Almost always int. Sometimes
    /// const char*.
    template<typename T>
    struct glfw_hint;

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

    /// @brief An x and y value pair. Structurally identical to `gvw::area`.
    /// @tparam T The type of the x and y values.
    template<typename T>
    struct coordinate;

    /// @brief A width and height value pair. Structurally identical to
    /// `gvw::coordinate`.
    /// @tparam T The type of the width and height values.
    template<typename T>
    struct area;

    /// @brief Joystick ID and event type of a joystick event.
    struct joystick_event;

    /// @brief Application information for Vulkan instance creation.
    struct application_info;

    /// @brief Initialization information for creating the Vulkan debug utility
    /// messenger.
    struct debug_utils_messenger_info;

    /// @brief Initialization hints for GLFW.
    struct init_hints;

    // Smart pointer to a gvw instance.
    using ptr = std::shared_ptr<gvw>;

    // Public create info struct.
    struct info;

    /***************** Monitor Related Types ******************/

    /// @brief Wrapper class for a GLFW monitor.
    class monitor;
    using monitor_ptr = std::shared_ptr<monitor>;

    /****************** Window Related Types ******************/

    /// @brief Wrapper class for a GLFW window.
    class window;
    using window_ptr = std::shared_ptr<window>;

    /// @brief Creation information for a window.
    struct window_info;

    /// @brief GLFW event callbacks.
    struct window_event_callbacks;

    /// @brief GLFW window creation hints.
    struct window_creation_hints;

    /// @brief GLFW window events.
    struct window_event : no_constructor
    {
        /// @brief The key, scancode, action, and mods of a key event.
        struct key;

        /// @brief A character event stored as an unsigned int. Can be
        /// interpreted as a UTF-8, UTF-16, or UTF-32 character.
        using character = unsigned int;

        /// @brief A cursor position event stored as a `coordinate` of type
        /// double.
        using cursor_position = coordinate<double>;

        /// @brief A cursor enter/leave event stored as an integer. Can be
        /// interpreted as a boolean.
        using cursor_enter = int;

        /// @brief The button, action, and mods of a mouse button event.
        struct mouse_button;

        /// @brief A scroll event stored as a `coordinate` of type double.
        using scroll = coordinate<double>;

        /// @brief File count and file paths from a file drop event.
        struct file_drop;

        /// @brief A size event stored as an `area` of type int.
        using size = area<int>;

        /// @brief A framebuffer event stored as an `area` of type int.
        using framebuffer_size = area<int>;

        /// @brief A content scale event stored as a `coordinate` of type float.
        using content_scale = coordinate<float>;

        /// @brief A position event stored as an `area` of type int.
        using position = coordinate<int>;

        /// @brief An iconify event stored as an integer. Can be interpreted as
        /// a boolean.
        using iconify = int;

        /// @brief A maximize event stored as an integer. Can be interpreted as
        /// a boolean.
        using maximize = int;

        /// @brief A focus event stored as an integer. Can be interpreted as a
        /// boolean.
        using focus = int;
    };

    /****************** Device Related Types ******************/

    /// @brief Wrapper class for a Vulkan logical device.
    class device;
    using device_ptr = std::shared_ptr<device>;

    /// @brief Creation information for a device.
    struct device_info;

    /// @brief Physical device selection.
    struct device_selection : no_constructor
    {
        struct queue_family_info;
        struct parameters;
        struct returns;

        using signature =
            std::vector<returns> (*)(const std::vector<parameters>&,
                                     const std::optional<vk::SurfaceKHR>&);

        [[nodiscard]] static std::vector<returns> MinimumForPresentation(
            const std::vector<parameters>& Physical_Device_Infos,
            const std::optional<vk::SurfaceKHR>& Window_Surface);
    };

    /// @brief Device Related create info structs.
    struct shader_info;
    struct buffer_info;
    struct render_pass_info;
    struct swapchain_info;
    struct pipeline_info;

    /// @brief Device Related internal classes.
    struct shader;
    struct buffer;
    struct vertex;
    struct swapchain;
    struct pipeline;

    /// @brief Shared pointers to device related internal classes.
    using swapchain_ptr = std::shared_ptr<swapchain>;
    using pipeline_ptr = std::shared_ptr<pipeline>;

    ////////////////////////////////////////////////////////////
    ///                       Constants                      ///
    ////////////////////////////////////////////////////////////

    /// @brief Default initialization information for GVW.
    static const info DEFAULT_INIT_INFO;

    /// @brief Default Vulkan application information for GVW initialization.
    static const application_info DEFAULT_APPLICATION_INFO;

    /// @brief Default debug utils messenger information for creating the vulkan
    /// debug messenger.
    static const debug_utils_messenger_info DEFAULT_UTILS_MESSENGER_INFO;

    /// @brief Default initialization hints as initially set by GLFW.
    static const init_hints DEFAULT_INIT_HINTS;

    /// @brief Vulkan instance layer presets.
    struct instance_layers : no_constructor
    {
        static const std::vector<const char*> VALIDATION;
    };

    /// @brief Vulkan instance extension presets.
    struct instance_extensions : no_constructor
    {
        static const std::vector<const char*> PORTABILITY_AND_DEBUG_UTILS;
    };

    /*************** Monitor Related Constants ****************/

    /// @brief Default gamma value for monitors.
    static const float DEFAULT_MONITOR_GAMMA;

    /**************** Window Related Constants ****************/

    /// @brief Default initialization information for a GLFW window.
    static const window_info DEFAULT_WINDOW_INFO;

    /// @brief No event callbacks.
    static const window_event_callbacks NO_WINDOW_EVENT_CALLBACKS;

    /// @brief Default creation hints as initially set by GLFW.
    static const window_creation_hints DEFAULT_WINDOW_CREATION_HINTS;

    /// @brief Window title presets.
    struct window_title : no_constructor
    {
        static const char* const BLANK;
        static const char* const UNTITLED;
    };

    /// @brief Window size presets.
    struct window_size : no_constructor
    {
        static const area<int> W_500_H_500;
        static const area<int> W_640_H_360;
    };

    /// @brief Window size limit presets.
    struct window_size_limit : no_constructor
    {
        static const area<int> NO_MINIMUM;
        static const area<int> NO_MAXIMUM;
    };

    /**************** Device Related Constants ****************/

    /// @brief Default logical device information.
    static const device_info DEFAULT_DEVICE_INFO;

    /// @brief Default buffer information.
    static const buffer_info DEFAULT_BUFFER_INFO;

    /// @brief Default render pass information.
    static const render_pass_info DEFAULT_RENDER_PASS_INFO;

    /// @brief Default swapchain information.
    static const swapchain_info DEFAULT_SWAPCHAIN_INFO;

    /// @brief Default pipeline information.
    static const pipeline_info DEFAULT_PIPELINE_INFO;

    /// @brief Window surface formats presets.
    struct surface_formats : no_constructor
    {
        // static const std::optional<std::vector<vk::SurfaceFormatKHR>>
        // STANDARD;
        static const std::vector<vk::SurfaceFormatKHR> STANDARD;
    };

    /// @brief Present modes presets.
    struct present_modes : no_constructor
    {
        static const std::vector<vk::PresentModeKHR> FIFO;
        static const std::vector<vk::PresentModeKHR> MAILBOX;
        static const std::vector<vk::PresentModeKHR> MAILBOX_OR_FIFO;
    };

    /// @brief Physical device features presets.
    struct physical_device_features : no_constructor
    {
        static const vk::PhysicalDeviceFeatures NONE;
    };

    /// @brief Logical device extension presets.
    struct logical_device_extensions : no_constructor
    {
        static const std::vector<const char*> SWAPCHAIN;
    };

    /// @brief Queue priority presets.
    struct queue_priority : no_constructor
    {
        static const float HIGH;
    };

    /// @brief Dynamic states presets.
    struct dynamic_states : no_constructor
    {
        static const std::vector<vk::DynamicState> VIEWPORT;
        static const std::vector<vk::DynamicState> VIEWPORT_AND_SCISSOR;
    };

    static const std::vector<shader> NO_SHADERS;

    static const std::vector<vk::VertexInputBindingDescription>
        NO_VERTEX_BINDING_DESCRIPTIONS;

    static const std::vector<vk::VertexInputAttributeDescription>
        NO_VERTEX_ATTRIBUTE_DESCRIPTIONS;

  private:
    ////////////////////////////////////////////////////////////
    ///                   Private Variables                  ///
    ////////////////////////////////////////////////////////////
    // Some variables are static so they're accessible by static functions.
    // NOLINTBEGIN

    static std::mutex glfwMutex;
    static std::mutex consoleMutex;

    /// @brief Pointer to the GVW error callback.
    static gvw_error_callback gvwErrorCallback;
    static std::mutex gvwErrorCallbackMutex;

    /// @brief Dynamic array of joystick events.
    static std::vector<joystick_event> joystickEvents;
    static std::mutex joystickEventsMutex;

    // NOLINTEND

    /// @todo Determine if these vulkan related variables need to be
    /// thread-safe.

    std::vector<const char*> vulkanInstanceExtensions;
    std::vector<const char*> vulkanInstanceLayers;

    vk::UniqueInstance vulkanInstance;
    vk::DispatchLoaderDynamic vulkanDispatchLoaderDynamic;
    vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic>
        vulkanDebugUtilsMessenger;

    ////////////////////////////////////////////////////////////
    ///               Private Static Functions               ///
    ////////////////////////////////////////////////////////////

    /// @brief Returns a vector containing all the items present in the user
    /// array that were missing in the vulkan array.
    template<typename UserT, typename VulkanT>
    static std::vector<UserT> UserItemsMissingInVulkanArray(
        const std::vector<UserT>& User_Array,
        const std::vector<VulkanT>& Vulkan_Array,
        bool (*Is_Identical)(UserT, VulkanT));

    /// @brief Returns a vector containing all the items present in the user
    /// array that were found in the vulkan array.
    template<typename UserT, typename VulkanT>
    static std::vector<UserT> UserItemsFoundInVulkanArray(
        const std::vector<UserT>& User_Array,
        const std::vector<VulkanT>& Vulkan_Array,
        bool (*Is_Identical)(UserT, VulkanT));

  public:
    ////////////////////////////////////////////////////////////
    ///                Public Static Functions               ///
    ////////////////////////////////////////////////////////////

    /// @brief Default gvw error callback. Throws an exception for each error.
    static void ThrowOnGvwError(const char* Description);

    /// @brief Default error callback for handling GLFW errors. Forwards all
    /// GLFW errors to the GVW error callback.
    static void ThrowOnGlfwError(int Error_Code, const char* Description);

    /// @brief Returns the GLFW version used at runtime.
    [[nodiscard]] static version GetGlfwRuntimeVersion() noexcept;

    /// @brief Returns the GLFW version used to compile GVW.
    [[nodiscard]] static version GetGlfwCompiletimeVersion() noexcept;

    /// @brief Sets the GVW error callback. If no argument is provided,
    /// `DefaultErrorCallback` is set as the GVW error callback.
    static void SetGvwErrorCallback(
        gvw_error_callback GVW_Error_Callback = ThrowOnGvwError) noexcept;

    /// @brief Sets the GLFW error callback. If no argument is provided,
    /// `DefaultGlfwErrorCallback` is set as the GLFW error callback.
    static void SetGlfwErrorCallback(
        GLFWerrorfun GLFW_Error_Callback = ThrowOnGlfwError) noexcept;

    /// @remark This function is intended to only be called by GLFW.
    static void AppendToJoystickEventBuffer(int JID, int Event);

    /// @brief Returns the GVW instance associated with a window handle.
    [[nodiscard]] static window& WindowInstance(GLFWwindow* Window);

    /// @brief The key event callback.
    static void AppendToKeyEventBuffer(GLFWwindow* Window,
                                       int Key,
                                       int Scancode,
                                       int Action,
                                       int Mods);

    /// @brief The character event callback.
    static void AppendToCharacterEventBuffer(GLFWwindow* Window,
                                             unsigned int Code_Point);

    /// @brief The cursor position event callback.
    static void AppendToCursorPositionEventBuffer(GLFWwindow* Window,
                                                  double X_Position,
                                                  double Y_Position);

    /// @brief The cursor enter event callback.
    static void AppendToCursorEnterEventBuffer(GLFWwindow* Window, int Entered);

    /// @brief The mouse button event callback.
    static void AppendToMouseButtonEventBuffer(GLFWwindow* Window,
                                               int Button,
                                               int Action,
                                               int Mods);

    /// @brief The scroll event callback.
    static void AppendToScrollEventBuffer(GLFWwindow* Window,
                                          double X_Offset,
                                          double Y_Offset);

    /// @brief The file drop event callback.
    static void AppendToFileDropEventBuffer(GLFWwindow* Window,
                                            int Count,
                                            const char** Paths);

    /// @brief The close event callback.
    static void AppendToCloseEventBuffer(GLFWwindow* Window);

    /// @brief The size event callback.
    static void AppendToSizeEventBuffer(GLFWwindow* Window,
                                        int Width,
                                        int Height);

    /// @brief The framebuffer size event callback.
    static void AppendToFramebufferSizeEventBuffer(GLFWwindow* Window,
                                                   int Width,
                                                   int Height);

    /// @brief The content scale event callback.
    static void AppendToContentScaleEventBuffer(GLFWwindow* Window,
                                                float XScale,
                                                float YScale);

    /// @brief The position event callback.
    static void AppendToPositionEventBuffer(GLFWwindow* Window,
                                            int XPosition,
                                            int YPosition);

    /// @brief The iconify event callback.
    static void AppendToIconifyEventBuffer(GLFWwindow* Window, int Iconified);

    /// @brief The maximize event callback.
    static void AppendToMaximizeEventBuffer(GLFWwindow* Window, int Maximized);

    /// @brief The focus event callback.
    static void AppendToFocusEventBuffer(GLFWwindow* Window, int Focused);

    /// @brief The refresh event callback.
    static void AppendToRefreshEventBuffer(GLFWwindow* Window);

    /// @brief Reads a file from the file system.
    template<typename T>
    static T ReadFile(const char* Absolute_Path);

    /// @remark This function is intended to only be called by Vulkan.
    static VKAPI_ATTR VkBool32 VKAPI_CALL PrintDebugMessagesToConsole(
        VkDebugUtilsMessageSeverityFlagBitsEXT Message_Severity,
        VkDebugUtilsMessageTypeFlagsEXT Message_Type,
        const VkDebugUtilsMessengerCallbackDataEXT* P_Callback_Data,
        void* P_User_Data);

  private:
    ////////////////////////////////////////////////////////////
    ///       Constructors, Operators, and Destructors       ///
    ////////////////////////////////////////////////////////////

    /// @brief Initializes GVW.
    /// @remark This constructor is made private to prevent it from being called
    /// from outside of this class.
    gvw(const info& Init_Info = DEFAULT_INIT_INFO);

  public:
    /// @brief Returns a reference to the global instance of this object.
    /// @remark All arguments are ignored if this function has been called once
    /// before during execution.
    static ptr Get(const info& Init_Info = DEFAULT_INIT_INFO);

    // Delete the copy constructor, move constructor, copy assignment operator,
    // and move assignment operator. It should not be possible to copy or move
    // this object.
    gvw(const gvw&) = delete;
    gvw(gvw&&) noexcept = delete;
    gvw& operator=(const gvw&) = delete;
    gvw& operator=(gvw&&) noexcept = delete;

    /// @brief The destructor is public so as to allow explicit destruction
    /// using the delete operator.
    ~gvw();

    ////////////////////////////////////////////////////////////
    ///                Public Member Functions               ///
    ////////////////////////////////////////////////////////////

    /// @brief Sets the joystick event callback.
    void SetJoystickEventCallback(
        GLFWjoystickfun Joystick_Event_Callback = AppendToJoystickEventBuffer);

    /// @brief Returns the joystick event buffer.
    const std::vector<joystick_event>& JoystickEvents();

    /// @brief Clears the joystick event buffer.
    void ClearJoystickEvents();

    /// @brief Polls events for all windows.
    void PollEvents();

    /// @brief Waits until a window event is received, then polls events for all
    /// windows.
    void WaitThenPollEvents();

    /// @brief Waits until either the timeout (measured in milliseconds) expires
    /// or a window event is received, then polls events for all windows.
    void WaitThenPollEvents(double Timeout);

    /// @brief Posts an empty event. Causes `WaitThenPollEvents` to poll events.
    void PostEmptyEvent();

    /// @brief Applies window creation hints.
    void ApplyWindowCreationHints(
        const window_creation_hints& Window_Creation_Hints =
            DEFAULT_WINDOW_CREATION_HINTS);

    /// @brief Creates a window.
    [[nodiscard]] window_ptr CreateWindow(
        const window_info& Window_Info = DEFAULT_WINDOW_INFO);

    /// @brief Selects physical devices for graphics processing.
    [[nodiscard]] std::vector<gvw::device_ptr> SelectPhysicalDevices(
        const device_info& Device_Info = DEFAULT_DEVICE_INFO,
        const std::optional<vk::SurfaceKHR>& Window_Surface = std::nullopt);

    /// @brief Creates a monitor object.
    [[nodiscard]] monitor_ptr Monitor(GLFWmonitor* Monitor_Handle);

    /// @brief Returns a `monitor` object cooresponding to the primary monitor.
    /// @remark The primary monitor, on most operating systems, contains the
    /// taskbar.
    [[nodiscard]] monitor_ptr PrimaryMonitor();

    /// @brief Returns a vector of `monitor` objects that each coorespond to a
    /// physical monitor.
    [[nodiscard]] std::vector<monitor_ptr> AllMonitors();
};

// Trailing local includes
#include "gvw.tpp"
#include "gvw-types.hpp"
#include "monitor.hpp"
#include "window.hpp"
#include "device.hpp"