#pragma once

/**
 * @file gvw-types.hpp
 * @author Caden Shmookler (cadenshmookler@gmail.com)
 * @brief Type definitions for the parent GVW class.
 * @date 2023-05-23
 */

// Leading local includes
#include "gvw.hpp"

struct gvw::version
{
    int major;
    int minor;
    int revision;
};

template<typename T>
struct gvw::glfw_hint
{
    const int HINT;
    T value;

    glfw_hint(int Hint, T Value);
    glfw_hint(const glfw_hint<T>& GLFW_Hint);
    glfw_hint(glfw_hint<T>&& GLFW_Hint) noexcept;
    glfw_hint<T>& operator=(const glfw_hint<T>&) = delete;
    glfw_hint<T>& operator=(glfw_hint<T>&&) = delete;
    ~glfw_hint() = default;
    glfw_hint<T>& operator=(T Value);
};

template<gvw::int_hint_function IntHintFunc,
         gvw::string_hint_function StringHintFunc,
         size_t IntHints,
         size_t StringHints>
struct gvw::glfw_hints
{
    friend class gvw;

    std::array<glfw_hint<int>, IntHints> intHints;
    std::array<glfw_hint<const char*>, StringHints> stringHints;

    glfw_hints(std::array<glfw_hint<int>, IntHints>&& Int_Hints,
               std::array<glfw_hint<const char*>, StringHints>&& String_Hints);
    glfw_hints(const glfw_hints& GLFW_Hints);
    glfw_hints(glfw_hints&& GLFW_Hints) noexcept = delete;
    ~glfw_hints() = default;
    glfw_hints& operator=(const glfw_hints& GLFW_Hints);
    glfw_hints& operator=(glfw_hints&& GLFW_Hints) noexcept;

  private:
    /// @brief Apply int and string hints.
    /// @warning This function is NOT thread safe.
    void Apply() const;
};

struct gvw::init_hints : public glfw_hints<glfwInitHint, nullptr, 3, 0>
{
    struct info
    {
        // Shared hints.
        int joystickHatButtons = GLFW_TRUE;

        // MacOS hints.
        int cocoaChdirResources = GLFW_TRUE;
        int cocoaMenubar = GLFW_TRUE;
    };

    static const info DEFAULT;

    // Shared hints.
    glfw_hint<int>& joystickHatButtons = intHints.at(0);

    // MacOS hints.
    glfw_hint<int>& cocoaChdirResources = intHints.at(1);
    glfw_hint<int>& cocoaMenubar = intHints.at(2);

    init_hints(const info& Init_Hints = DEFAULT);
};

template<typename T>
struct gvw::coordinate
{
    T x;
    T y;

    coordinate() = default;
    coordinate(T X, T Y); // NOLINT
    coordinate(const coordinate<T>&) = default;
    coordinate(coordinate<T>&&) noexcept = default;
    coordinate<T>& operator=(const coordinate<T>&) = default;
    coordinate<T>& operator=(coordinate<T>&&) noexcept = default;
    ~coordinate() = default;

    coordinate<T>& operator=(const area<T>& Area);
    coordinate<T>& operator=(area<T>&& Area) noexcept;
    coordinate(const area<T>& Area);
    coordinate(area<T>&& Area) noexcept;

    bool operator==(const coordinate<T>& Coordinate) const;
    bool operator!=(const coordinate<T>& Coordinate) const;
};

template<typename T>
struct gvw::area
{
    T width;
    T height;

    area() = default;
    area(T Width, T Height);
    area(const area<T>&) = default;
    area(area<T>&&) noexcept = default;
    area<T>& operator=(const area<T>&) = default;
    area<T>& operator=(area<T>&&) noexcept = default;
    ~area() = default;

    area(const coordinate<T>& Coordinate);
    area(coordinate<T>&& Coordinate) noexcept;
    area<T>& operator=(const coordinate<T>& Coordinate);
    area<T>& operator=(coordinate<T>&& Coordinate) noexcept;

    bool operator==(const area<T>& Area) const;
    bool operator!=(const area<T>& Area) const;
};

struct gvw::joystick_event
{
    int jid;
    int event;
};

struct gvw::application_info
{
    vk::StructureType sType = vk::StructureType::eApplicationInfo;
    const void* pNext = nullptr;
    const char* pApplicationName = "";
    uint32_t applicationVersion = VK_MAKE_VERSION(0, 0, 0);
    const char* pEngineName = "No Engine";
    uint32_t engineVersion = VK_MAKE_VERSION(0, 0, 0);
    uint32_t apiVersion = VK_API_VERSION_1_3;
};

struct gvw::debug_utils_messenger_info
{
    vk::StructureType sType =
        vk::StructureType::eDebugUtilsMessengerCreateInfoEXT;
    const void* pNext = nullptr;
    vk::DebugUtilsMessengerCreateFlagsEXT flags = {};
    vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity =
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
    vk::DebugUtilsMessageTypeFlagsEXT messageType =
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
        vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding;
    PFN_vkDebugUtilsMessengerCallbackEXT pfnUserCallback =
        PrintDebugMessagesToConsole;
    void* pUserData = nullptr;
};

struct gvw::info
{
    vk::InstanceCreateFlags instanceFlags =
        vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
    gvw::application_info applicationInfo = DEFAULT_APPLICATION_INFO;
    const std::vector<const char*>& instanceLayers =
        instance_layers::VALIDATION;
    const std::vector<const char*>& instanceExtensions =
        instance_extensions::PORTABILITY_AND_DEBUG_UTILS;
    const gvw::debug_utils_messenger_info& debugUtilsMessengerInfo =
        DEFAULT_UTILS_MESSENGER_INFO;
    gvw_error_callback gvwErrorCallback = ThrowOnGvwError;
    GLFWerrorfun glfwErrorCallback = ThrowOnGlfwError;
    const init_hints& initHints = DEFAULT_INIT_HINTS;
};

struct gvw::shader_info
{
    const char* name = "";
    const char* code = "";
    vk::ShaderStageFlagBits stage = {};
    const char* entryPoint = "main";
};

struct gvw::shader
{
    /// @brief Sample shaders.
    static const char* const VERTEX_BASIC;
    static const char* const FRAGMENT_BASIC;

    std::shared_ptr<vk::UniqueShaderModule> shaderModule;
    vk::PipelineShaderStageCreateInfo pipelineShaderStageCreateInfo;
};

struct gvw::vertex
{
    glm::vec2 position;
    glm::vec3 color;

    static vk::VertexInputBindingDescription BindingDescription()
    {
        vk::VertexInputBindingDescription bindingDescription = {
            .binding = 0,
            .stride = sizeof(vertex),
            .inputRate = vk::VertexInputRate::eVertex
        };
        return bindingDescription;
    }

    static std::array<vk::VertexInputAttributeDescription, 2>
    AttributeDescriptions()
    {
        std::array<vk::VertexInputAttributeDescription, 2>
            attributeDescriptions = {
                { { .location = 0,
                    .binding = 0,
                    .format = vk::Format::eR32G32Sfloat,
                    .offset = offsetof(vertex, position) },
                  { .location = 1,
                    .binding = 0,
                    .format = vk::Format::eR32G32B32Sfloat,
                    .offset = offsetof(vertex, color) } }
            };
        return attributeDescriptions;
    }
};

struct gvw::window_event_callbacks
{
    GLFWkeyfun keyCallback = AppendToKeyEventBuffer;
    GLFWcharfun characterCallback = AppendToCharacterEventBuffer;
    GLFWcursorposfun cursorPositionCallback = AppendToCursorPositionEventBuffer;
    GLFWcursorenterfun cursorEnterCallback = AppendToCursorEnterEventBuffer;
    GLFWmousebuttonfun mouseButtonCallback = AppendToMouseButtonEventBuffer;
    GLFWscrollfun scrollCallback = AppendToScrollEventBuffer;
    GLFWdropfun fileDropCallback = AppendToFileDropEventBuffer;
    GLFWwindowclosefun closeCallback = AppendToCloseEventBuffer;
    GLFWwindowsizefun sizeCallback = AppendToSizeEventBuffer;
    GLFWframebuffersizefun framebufferSizeCallback =
        AppendToFramebufferSizeEventBuffer;
    GLFWwindowcontentscalefun contentScaleCallback =
        AppendToContentScaleEventBuffer;
    GLFWwindowposfun positionCallback = AppendToPositionEventBuffer;
    GLFWwindowiconifyfun iconifyCallback = AppendToIconifyEventBuffer;
    GLFWwindowmaximizefun maximizeCallback = AppendToMaximizeEventBuffer;
    GLFWwindowfocusfun focusCallback = AppendToFocusEventBuffer;
    GLFWwindowrefreshfun refreshCallback = AppendToRefreshEventBuffer;
};

struct gvw::window_creation_hints
    : public glfw_hints<glfwWindowHint, glfwWindowHintString, 35, 3> // NOLINT
{
    struct info
    {
        // General hints.
        int resizable = GLFW_TRUE;
        int visible = GLFW_TRUE;
        int decorated = GLFW_TRUE;
        int focused = GLFW_TRUE;
        int autoIconify = GLFW_TRUE;
        int floating = GLFW_FALSE;
        int maximized = GLFW_FALSE;
        int centerCursor = GLFW_TRUE;
        int transparentFramebuffer = GLFW_FALSE;
        int focusOnShow = GLFW_TRUE;
        int scaleToMonitor = GLFW_FALSE;

        // Framebuffer hints.
        int redBits = 8;     // NOLINT
        int greenBits = 8;   // NOLINT
        int blueBits = 8;    // NOLINT
        int alphaBits = 8;   // NOLINT
        int depthBits = 24;  // NOLINT
        int stencilBits = 8; // NOLINT
        int auxBuffers = 0;
        int stereo = GLFW_FALSE;
        int samples = 0;
        int srgbCapable = GLFW_FALSE;
        int doubleBuffer = GLFW_TRUE;

        // Monitor hints.
        int refreshRate = GLFW_DONT_CARE;

        // Context hints.
        int clientApi = GLFW_NO_API;
        int contextCreationApi = GLFW_NATIVE_CONTEXT_API;
        int contextVersionMajor = 3;
        int contextVersionMinor = 6; // NOLINT
        int openglForwardCompatible = GLFW_FALSE;
        int openglDebugContext = GLFW_FALSE;
        int openglProfile = GLFW_OPENGL_CORE_PROFILE;
        int contextRobustness = GLFW_NO_ROBUSTNESS;
        int contextReleaseBehavior = GLFW_ANY_RELEASE_BEHAVIOR;
        int contextNoError = GLFW_FALSE;

        // MacOS hints.
        int cocoaRetinaFramebuffer = GLFW_TRUE;
        int cocoaGraphicsSwitching = GLFW_FALSE;
        const char* cocoaFrameName = "";

        // Linux hints.
        const char* x11ClassName = "";
        const char* x11InstanceName = "";
    };

    static const info DEFAULT;

    // NOLINTBEGIN

    // General hints.
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

    // Framebuffer hints.
    glfw_hint<int>& redBits = intHints.at(11);
    glfw_hint<int>& greenBits = intHints.at(12);
    glfw_hint<int>& blueBits = intHints.at(13);
    glfw_hint<int>& alphaBits = intHints.at(14);
    glfw_hint<int>& depthBits = intHints.at(15);
    glfw_hint<int>& stencilBits = intHints.at(16);
    glfw_hint<int>& auxBuffers = intHints.at(17);
    glfw_hint<int>& stereo = intHints.at(18);
    glfw_hint<int>& samples = intHints.at(19);
    glfw_hint<int>& srgbCapable = intHints.at(20);
    glfw_hint<int>& doubleBuffer = intHints.at(21);

    // Monitor hints.
    glfw_hint<int>& refreshRate = intHints.at(22);

    // Context hints.
    glfw_hint<int>& clientApi = intHints.at(23);
    glfw_hint<int>& contextCreationApi = intHints.at(24);
    glfw_hint<int>& contextVersionMajor = intHints.at(25);
    glfw_hint<int>& contextVersionMinor = intHints.at(26);
    glfw_hint<int>& openglForwardCompatible = intHints.at(27);
    glfw_hint<int>& openglDebugContext = intHints.at(28);
    glfw_hint<int>& openglProfile = intHints.at(29);
    glfw_hint<int>& contextRobustness = intHints.at(30);
    glfw_hint<int>& contextReleaseBehavior = intHints.at(31);
    glfw_hint<int>& contextNoError = intHints.at(32);

    // MacOS hints.
    glfw_hint<int>& cocoaRetinaFramebuffer = intHints.at(33);
    glfw_hint<int>& cocoaGraphicsSwitching = intHints.at(34);
    glfw_hint<const char*>& cocoaFrameName = stringHints.at(0);

    // Linux hints.
    glfw_hint<const char*>& x11ClassName = stringHints.at(1);
    glfw_hint<const char*>& x11InstanceName = stringHints.at(2);

    // NOLINTEND

    window_creation_hints(const info& Window_Creation_Hints = DEFAULT);
};

struct gvw::device_info
{
    device_selection::signature selectPhysicalDevicesAndQueues =
        device_selection::MinimumForPresentation;
    std::optional<std::vector<vk::SurfaceFormatKHR>> surfaceFormats =
        surface_formats::STANDARD;
    std::optional<std::vector<vk::PresentModeKHR>> presentModes =
        present_modes::FIFO;
    vk::PhysicalDeviceFeatures physicalDeviceFeatures =
        physical_device_features::NONE;
    const std::vector<const char*>& logicalDeviceExtensions =
        logical_device_extensions::SWAPCHAIN;
};

struct gvw::buffer_info
{
    vk::DeviceSize sizeInBytes = 0;
    vk::BufferUsageFlags usage = vk::BufferUsageFlagBits::eVertexBuffer;
    vk::MemoryPropertyFlags memoryProperties =
        vk::MemoryPropertyFlagBits::eHostVisible |
        vk::MemoryPropertyFlagBits::eHostCoherent;
};

struct gvw::buffer
{
    vk::DeviceSize size;
    vk::UniqueBuffer buffer;
    vk::UniqueDeviceMemory memory;
};

struct gvw::render_pass_info
{
    vk::Format format = vk::Format::eB8G8R8A8Srgb;
    vk::SampleCountFlagBits samples = vk::SampleCountFlagBits::e1;
    uint32_t graphicsAttachment = 0;
    vk::ImageLayout graphicsLayout = vk::ImageLayout::eColorAttachmentOptimal;
};

struct gvw::swapchain_info
{
    const area<int>& framebufferSize = window_size::W_640_H_360;
    uint32_t graphicsQueueIndex = 0;
    uint32_t presentQueueIndex = 0;
    vk::SurfaceKHR surface;
    vk::RenderPass renderPass;
    swapchain_ptr oldSwapchain = nullptr;
};

struct gvw::swapchain
{
    vk::Viewport viewport = { .x = 0.0F,
                              .y = 0.0F,
                              .width = 0.0F,
                              .height = 0.0F,
                              .minDepth = 0.0F,
                              .maxDepth = 1.0F };
    vk::Rect2D scissor = { .offset = { .x = 0, .y = 0 },
                           .extent = { .width = 0, .height = 0 } };
    vk::UniqueSwapchainKHR swapchain;
    std::vector<vk::Image> swapchainImages;
    std::vector<vk::UniqueImageView> swapchainImageViews;
    std::vector<vk::UniqueFramebuffer> swapchainFramebuffers;
};

struct gvw::pipeline_info
{
    const std::vector<shader>& shaders = NO_SHADERS;
    const std::vector<vk::DynamicState>& dynamicStates =
        dynamic_states::VIEWPORT_AND_SCISSOR;
    const std::vector<vk::VertexInputBindingDescription>&
        vertexInputBindingDescriptions = NO_VERTEX_BINDING_DESCRIPTIONS;
    const std::vector<vk::VertexInputAttributeDescription>&
        vertexInputAttributeDescriptions = NO_VERTEX_ATTRIBUTE_DESCRIPTIONS;
    vk::RenderPass renderPass;
};

struct gvw::pipeline
{
    vk::UniquePipelineLayout layout;
    vk::UniquePipeline pipeline;
};

struct gvw::window_info
{
    std::optional<const coordinate<int>> position = std::nullopt;
    const area<int>& size = window_size::W_640_H_360;
    const char* title = window_title::UNTITLED;
    GLFWmonitor* fullScreenMonitor = nullptr;
    const window_creation_hints& creationHints = DEFAULT_WINDOW_CREATION_HINTS;
    const window_event_callbacks& eventCallbacks = NO_WINDOW_EVENT_CALLBACKS;
    const device_info& deviceInfo = DEFAULT_DEVICE_INFO;
    vk::DeviceSize sizeOfVerticesInBytes = 0;
};

struct gvw::window_event::key
{
    int key;
    int scancode;
    int action;
    int mods;
};

struct gvw::window_event::mouse_button
{
    int button;
    int action;
    int mods;
};

struct gvw::window_event::file_drop
{
    int count;
    const char** paths;
};

struct gvw::glsl_compiler
{
    glsl_compiler();
    glsl_compiler(const glsl_compiler&) = default;
    glsl_compiler(glsl_compiler&&) noexcept = default;
    glsl_compiler& operator=(const glsl_compiler&) = default;
    glsl_compiler& operator=(glsl_compiler&&) noexcept = default;
    ~glsl_compiler();

    /// @brief Convert Vulkan shader stage to GLSL shader stage.
    static glslang_stage_t VulkanStageToGlslStage(
        vk::ShaderStageFlagBits Vulkan_Shader_Stage);

    /// @brief Compile GLSL source code to SPIR-V binary code.
    std::vector<uint32_t> Compile(const char* Source,
                                  glslang_stage_t Stage,
                                  const char* Output_File_Name);
};

struct gvw::device_selection::queue_family_info
{
    vk::DeviceQueueCreateInfo createInfo;
    vk::QueueFamilyProperties properties;
};

struct gvw::device_selection::parameters
{
    vk::PhysicalDevice physicalDevice;
    std::optional<std::vector<vk::SurfaceFormatKHR>> surfaceFormats;
    std::optional<std::vector<vk::PresentModeKHR>> presentModes;
};

struct gvw::device_selection::returns
{
    vk::PhysicalDevice physicalDevice;
    vk::SurfaceFormatKHR surfaceFormat;
    vk::PresentModeKHR presentMode;
    std::vector<queue_family_info> queueFamilyInfos;
};

// Trailing local includes
#include "gvw-types.tpp"