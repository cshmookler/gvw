// Standard includes
#include <iostream>
#include <numeric>

// Local includes
#include "gvw.hpp"
#include "gvw-constants.hpp"
#include "../utils/ansiec/ansiec.hpp"

class gvw::gvw_public_constructor : public gvw
{
  public:
    template<typename... Args>
    gvw_public_constructor(Args&&... Arguments)
        : gvw(std::forward<Args>(Arguments)...)
    {
    }
};

class gvw::window_public_constructor : public window
{
  public:
    template<typename... Args>
    window_public_constructor(Args&&... Arguments)
        : window(std::forward<Args>(Arguments)...)
    {
    }
};

class gvw::monitor_public_constructor : public monitor
{
  public:
    template<typename... Args>
    monitor_public_constructor(Args&&... Arguments)
        : monitor(std::forward<Args>(Arguments)...)
    {
    }
};

class gvw::device_public_constructor : public device
{
  public:
    template<typename... Args>
    device_public_constructor(Args&&... Arguments)
        : device(std::forward<Args>(Arguments)...)
    {
    }
};

// template<typename... Args>
// gvw::terminator<Args...>::terminator::terminator(deleter_signature Deleter,
//                                                  Args... Arguments)
//     : deleter(Deleter)
//     , args(Arguments...)
// {
// }

// template<typename... Args>
// gvw::terminator<Args...>::terminator::~terminator()
// {
//     std::apply(this->deleter, this->args);
// }

gvw::init_hints::init_hints(const info& Init_Hints)
    : glfw_hints(
          { { { GLFW_JOYSTICK_HAT_BUTTONS, Init_Hints.joystickHatButtons },
              { GLFW_COCOA_CHDIR_RESOURCES, Init_Hints.cocoaChdirResources },
              { GLFW_COCOA_MENUBAR, Init_Hints.cocoaMenubar } } },
          {})
{
}

// NOLINTBEGIN
std::mutex gvw::consoleMutex = {};
std::mutex gvw::glfwMutex = {};

gvw::gvw_error_callback gvw::gvwErrorCallback = gvw::ThrowOnGvwError;
std::mutex gvw::gvwErrorCallbackMutex = {};

std::vector<gvw::joystick_event> gvw::joystickEvents = {};
std::mutex gvw::joystickEventsMutex = {};
// NOLINTEND

void gvw::TerminateGlfw() noexcept
{
    std::scoped_lock<std::mutex> lock(glfwMutex);
    glfwTerminate();
}

void gvw::ThrowOnGvwError(const char* Description)
{
    std::stringstream error;
    error << ansiec::BOLD << ansiec::RED_FG << "GVW error: " << Description
          << ansiec::RESET;
    throw std::runtime_error(error.str());
}

void gvw::ThrowOnGlfwError(int Error_Code, const char* Description)
{
    std::stringstream error;
    error << ansiec::BOLD << ansiec::RED_FG << "GLFW error [" << Error_Code
          << "]: " << Description << ansiec::RESET;
    throw std::runtime_error(error.str());
}

gvw::version gvw::GetGlfwRuntimeVersion() noexcept
{
    version glfwVersion = { .major = 0, .minor = 0, .revision = 0 };
    glfwGetVersion(
        &glfwVersion.major, &glfwVersion.minor, &glfwVersion.revision);
    return glfwVersion;
}

gvw::version gvw::GetGlfwCompiletimeVersion() noexcept
{
    return version{ .major = GLFW_VERSION_MAJOR,
                    .minor = GLFW_VERSION_MINOR,
                    .revision = GLFW_VERSION_REVISION };
}

void gvw::SetGvwErrorCallback(gvw_error_callback GVW_Error_Callback) noexcept
{
    std::scoped_lock<std::mutex> lock(gvwErrorCallbackMutex);
    gvwErrorCallback = GVW_Error_Callback;
}

void gvw::SetGlfwErrorCallback(GLFWerrorfun GLFW_Error_Callback) noexcept
{
    std::scoped_lock<std::mutex> lock(glfwMutex);
    glfwSetErrorCallback(GLFW_Error_Callback);
}

void gvw::AppendToJoystickEventBuffer(int JID, int Event)
{
    std::scoped_lock<std::mutex> lock(joystickEventsMutex);
    joystickEvents.emplace_back(JID, Event);
}

gvw::window& gvw::WindowInstance(GLFWwindow* Window)
{
    return *static_cast<window*>(glfwGetWindowUserPointer(Window));
}

void gvw::AppendToKeyEventBuffer(GLFWwindow* Window,
                                 int Key,
                                 int Scancode,
                                 int Action,
                                 int Mods)
{
    gvw::window& window = WindowInstance(Window);
    std::scoped_lock<std::mutex> lock(window.keyEventsMutex);
    window.keyEvents.emplace_back(Key, Scancode, Action, Mods);
};

void gvw::AppendToCharacterEventBuffer(GLFWwindow* Window,
                                       unsigned int Code_Point)
{
    gvw::window& window = WindowInstance(Window);
    std::scoped_lock<std::mutex> lock(window.characterEventsMutex);
    window.characterEvents.emplace_back(Code_Point);
}

void gvw::AppendToCursorPositionEventBuffer(GLFWwindow* Window,
                                            double X_Position,
                                            double Y_Position)
{
    gvw::window& window = WindowInstance(Window);
    std::scoped_lock<std::mutex> lock(window.cursorPositionEventsMutex);
    window.cursorPositionEvents.emplace_back(X_Position, Y_Position);
}

void gvw::AppendToCursorEnterEventBuffer(GLFWwindow* Window, int Entered)
{
    gvw::window& window = WindowInstance(Window);
    std::scoped_lock<std::mutex> lock(window.cursorEnterEventsMutex);
    window.cursorEnterEvents.emplace_back(Entered);
}

void gvw::AppendToMouseButtonEventBuffer(GLFWwindow* Window,
                                         int Button,
                                         int Action,
                                         int Mods)
{
    gvw::window& window = WindowInstance(Window);
    std::scoped_lock<std::mutex> lock(window.mouseButtonEventsMutex);
    window.mouseButtonEvents.emplace_back(Button, Action, Mods);
}

void gvw::AppendToScrollEventBuffer(GLFWwindow* Window,
                                    double X_Offset,
                                    double Y_Offset)
{
    gvw::window& window = WindowInstance(Window);
    std::scoped_lock<std::mutex> lock(window.scrollEventsMutex);
    window.scrollEvents.emplace_back(X_Offset, Y_Offset);
}

void gvw::AppendToFileDropEventBuffer(GLFWwindow* Window,
                                      int Count,
                                      const char** Paths)
{
    gvw::window& window = WindowInstance(Window);
    std::scoped_lock<std::mutex> lock(window.fileDropEventsMutex);
    window.fileDropEvents.emplace_back(Count, Paths);
}

void gvw::AppendToCloseEventBuffer(GLFWwindow* Window)
{
    gvw::window& window = WindowInstance(Window);
    std::scoped_lock<std::mutex> lock(window.closeEventsMutex);
    window.closeEvents++;
}

void gvw::AppendToSizeEventBuffer(GLFWwindow* Window, int Width, int Height)
{
    gvw::window& window = WindowInstance(Window);
    std::scoped_lock<std::mutex> lock(window.sizeEventsMutex);
    window.sizeEvents.emplace_back(Width, Height);
}

void gvw::AppendToFramebufferSizeEventBuffer(GLFWwindow* Window,
                                             int Width,
                                             int Height)
{
    gvw::window& window = WindowInstance(Window);
    std::scoped_lock<std::mutex> lock(window.framebufferSizeEventsMutex);
    window.framebufferSizeEvents.emplace_back(Width, Height);
}

void gvw::AppendToContentScaleEventBuffer(GLFWwindow* Window,
                                          float XScale,
                                          float YScale)
{
    gvw::window& window = WindowInstance(Window);
    std::scoped_lock<std::mutex> lock(window.contentScaleEventsMutex);
    window.contentScaleEvents.emplace_back(XScale, YScale);
}

void gvw::AppendToPositionEventBuffer(GLFWwindow* Window,
                                      int XPosition,
                                      int YPosition)
{
    gvw::window& window = WindowInstance(Window);
    std::scoped_lock<std::mutex> lock(window.positionEventsMutex);
    window.positionEvents.emplace_back(XPosition, YPosition);
}

void gvw::AppendToIconifyEventBuffer(GLFWwindow* Window, int Iconified)
{
    gvw::window& window = WindowInstance(Window);
    std::scoped_lock<std::mutex> lock(window.iconifyEventsMutex);
    window.iconifyEvents.emplace_back(Iconified);
}

void gvw::AppendToMaximizeEventBuffer(GLFWwindow* Window, int Maximized)
{
    gvw::window& window = WindowInstance(Window);
    std::scoped_lock<std::mutex> lock(window.maximizeEventsMutex);
    window.maximizeEvents.emplace_back(Maximized);
}

void gvw::AppendToFocusEventBuffer(GLFWwindow* Window, int Focused)
{
    gvw::window& window = WindowInstance(Window);
    std::scoped_lock<std::mutex> lock(window.focusEventsMutex);
    window.focusEvents.emplace_back(Focused);
}

void gvw::AppendToRefreshEventBuffer(GLFWwindow* Window)
{
    gvw::window& window = WindowInstance(Window);
    std::scoped_lock<std::mutex> lock(window.refreshEventsMutex);
    window.refreshEvents++;
}

VKAPI_ATTR VkBool32 VKAPI_CALL gvw::PrintDebugMessagesToConsole(
    VkDebugUtilsMessageSeverityFlagBitsEXT Message_Severity,
    VkDebugUtilsMessageTypeFlagsEXT Message_Type,
    const VkDebugUtilsMessengerCallbackDataEXT* P_Callback_Data,
    void* P_User_Data)
{
    std::stringstream message;
    bool error = false;
    switch (Message_Severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            message << "[Verbose - ";
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            message << ansiec::BOLD << ansiec::CYAN_FG << "[Info - ";
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            message << ansiec::BOLD << ansiec::YELLOW_FG << "[Warning - ";
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            message << ansiec::BOLD << ansiec::RED_FG << "[Error - ";
            error = true;
            break;
        default:
            message << ansiec::BOLD << ansiec::MAGENTA_FG << "[Unknown - ";
            break;
    }

    switch (Message_Type) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
            message << "General]: ";
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
            message << "Validation]: ";
            break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
            message << "Performance]: ";
            break;
        default:
            message << "Unknown]: ";
            break;
    }

    message << P_Callback_Data->pMessage << ansiec::RESET;

    if (P_User_Data != nullptr) {
        message << " - Note: User data for this message was NOT null but was "
                   "ignored.";
    }

    message << "\n";

    consoleMutex.lock();
    std::cerr << message.str();
    consoleMutex.unlock();

    if (error) {
        gvwErrorCallback("Vulkan validation Error.");
    }

    return VK_FALSE;
}

gvw::gvw(const info& Init_Info)
    : glfwTerminator(std::make_unique<terminator<>>(TerminateGlfw))
{
    // Set error callbacks
    SetGvwErrorCallback(Init_Info.gvwErrorCallback);
    SetGlfwErrorCallback(Init_Info.glfwErrorCallback);

    std::scoped_lock<std::mutex> lock(glfwMutex);

    // Apply GLFW initialization hints
    Init_Info.initHints.Apply();

    // Initialize the GLFW library
    if (glfwInit() == GLFW_FALSE) {
        gvwErrorCallback("GLFW failed to initialize");
    }

    // Check for Vulkan support
    if (glfwVulkanSupported() == GLFW_FALSE) {
        gvwErrorCallback("Failed to find the Vulkan loader and/or a minimally "
                         "functional ICD (Installable Client Driver)");
    }

    // Get required instance extensions
    this->vulkanInstanceExtensions = Init_Info.instanceExtensions;
    uint32_t extensionCount = 0;
    const char** extensionsPointer =
        glfwGetRequiredInstanceExtensions(&extensionCount);
    for (uint32_t i = 0; i < extensionCount; ++i) {
        this->vulkanInstanceExtensions.emplace_back(
            extensionsPointer[i]); // NOLINT
    }

    // Verify that the requested extensions are available
    std::vector<vk::ExtensionProperties> allAvailableExtensions =
        vk::enumerateInstanceExtensionProperties();
    std::vector<const char*> extensionsNotFound =
        UserItemsMissingInVulkanArray<const char*, vk::ExtensionProperties>(
            this->vulkanInstanceExtensions,
            allAvailableExtensions,
            [](const char* User, vk::ExtensionProperties Vulkan) {
                return std::strcmp(User, Vulkan.extensionName) == 0;
            });
    if (!extensionsNotFound.empty()) {
        gvwErrorCallback(("The following Vulkan instance extensions were "
                          "requested but are not supported on this system: " +
                          std::accumulate(extensionsNotFound.begin(),
                                          extensionsNotFound.end(),
                                          std::string(),
                                          [](const std::string& String,
                                             const char* Extension) {
                                              return String + "\n\t" +
                                                     Extension;
                                          }))
                             .c_str());
    }

    // Get requested instance layers
    this->vulkanInstanceLayers = Init_Info.instanceLayers;

    // Verify that the requested layers are available
    std::vector<vk::LayerProperties> allAvailableLayers =
        vk::enumerateInstanceLayerProperties();
    std::vector<const char*> layersNotFound =
        UserItemsMissingInVulkanArray<const char*, vk::LayerProperties>(
            this->vulkanInstanceLayers,
            allAvailableLayers,
            [](const char* User, vk::LayerProperties Vulkan) {
                return std::strcmp(User, Vulkan.layerName) == 0;
            });
    if (!layersNotFound.empty()) {
        gvwErrorCallback(
            ("The following Vulkan instance layers were "
             "requested but are not supported on this system: " +
             std::accumulate(layersNotFound.begin(),
                             layersNotFound.end(),
                             std::string(),
                             [](const std::string& String, const char* Layer) {
                                 return String + "\n\t" + Layer;
                             }))
                .c_str());
    }

    // Create the instance creation information struct
    vk::InstanceCreateInfo instanceInfo = {
        // NOLINTNEXTLINE
        .pNext = reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT*>(
            &Init_Info.debugUtilsMessengerInfo),
        .flags = Init_Info.instanceFlags,
        // NOLINTNEXTLINE
        .pApplicationInfo = reinterpret_cast<const vk::ApplicationInfo*>(
            &Init_Info.applicationInfo),
        .enabledLayerCount =
            static_cast<uint32_t>(this->vulkanInstanceLayers.size()),
        .ppEnabledLayerNames = this->vulkanInstanceLayers.data(),
        .enabledExtensionCount =
            static_cast<uint32_t>(this->vulkanInstanceExtensions.size()),
        .ppEnabledExtensionNames = this->vulkanInstanceExtensions.data()
    };

    // Create the vulkan instance
    this->vulkanInstance = vk::createInstanceUnique(instanceInfo);

    // Automatically look up instance extension function addresses
    this->vulkanDispatchLoaderDynamic = { vulkanInstance.get(),
                                          vkGetInstanceProcAddr };

    // Debug callback setup
#ifdef GVW_VULKAN_VALIDATION_LAYERS
    this->vulkanDebugUtilsMessenger =
        this->vulkanInstance->createDebugUtilsMessengerEXTUnique(
            // NOLINTNEXTLINE
            *reinterpret_cast<const vk::DebugUtilsMessengerCreateInfoEXT*>(
                &Init_Info.debugUtilsMessengerInfo),
            nullptr,
            this->vulkanDispatchLoaderDynamic);
#endif
}

gvw::ptr gvw::Get(const info& Init_Info)
{
    static ptr gvw = std::make_unique<gvw_public_constructor>(Init_Info);
    return gvw;
}

// NOLINTNEXTLINE
void gvw::SetJoystickEventCallback(GLFWjoystickfun Joystick_Event_Callback)
{
    std::scoped_lock<std::mutex> lock(glfwMutex);
    glfwSetJoystickCallback(Joystick_Event_Callback);
}

const std::vector<gvw::joystick_event>& gvw::JoystickEvents() // NOLINT
{
    std::scoped_lock<std::mutex> lock(joystickEventsMutex);
    return joystickEvents;
}

void gvw::ClearJoystickEvents() // NOLINT
{
    std::scoped_lock<std::mutex> lock(joystickEventsMutex);
    joystickEvents.clear();
}

void gvw::PollEvents() // NOLINT
{
    std::scoped_lock<std::mutex> lock(glfwMutex);
    glfwPollEvents();
}

void gvw::WaitThenPollEvents() // NOLINT
{
    std::scoped_lock<std::mutex> lock(glfwMutex);
    glfwWaitEvents();
}

void gvw::WaitThenPollEvents(double Timeout) // NOLINT
{
    std::scoped_lock<std::mutex> lock(glfwMutex);
    glfwWaitEventsTimeout(Timeout);
}

void gvw::PostEmptyEvent() // NOLINT
{
    std::scoped_lock<std::mutex> lock(glfwMutex);
    glfwPostEmptyEvent();
}

// NOLINTNEXTLINE
void gvw::ApplyWindowCreationHints(
    const window_creation_hints& Window_Creation_Hints)
{
    std::scoped_lock<std::mutex> lock(glfwMutex);
    Window_Creation_Hints.Apply();
}

// NOLINTNEXTLINE
gvw::window_ptr gvw::CreateWindow(const window_info& Window_Info)
{
    return std::make_shared<window_public_constructor>(
        Get(), Window_Info, nullptr);
}

std::vector<gvw::device_ptr> gvw::SelectPhysicalDevices(
    const device_info& Device_Info,
    const std::optional<vk::SurfaceKHR>& Window_Surface)
{
    std::vector<vk::PhysicalDevice> physicalDevices =
        this->vulkanInstance->enumeratePhysicalDevices();
    if (physicalDevices.empty()) {
        gvwErrorCallback(
            "No physical devices with Vulkan support are available.");
    }

    std::vector<device_selection::parameters> physicalDevicesInfoCompatible;
    for (const auto& physicalDevice : physicalDevices) {
        std::optional<std::vector<vk::SurfaceFormatKHR>>
            availableSurfaceFormats;
        std::optional<std::vector<vk::PresentModeKHR>> availablePresentModes;

        if (!Window_Surface.has_value()) {
            availableSurfaceFormats = Device_Info.surfaceFormats;
            availablePresentModes = Device_Info.presentModes;
        } else {
            std::vector<vk::SurfaceFormatKHR> physicalDeviceSurfaceFormats =
                physicalDevice.getSurfaceFormatsKHR(Window_Surface.value());
            std::vector<vk::PresentModeKHR> physicalDevicePresentModes =
                physicalDevice.getSurfacePresentModesKHR(
                    Window_Surface.value());

            if (!Device_Info.surfaceFormats.has_value()) {
                availableSurfaceFormats = physicalDeviceSurfaceFormats;
            } else {
                availableSurfaceFormats = Device_Info.surfaceFormats.value();
            }
            if (!Device_Info.presentModes.has_value()) {
                availablePresentModes = physicalDevicePresentModes;
            } else {
                availablePresentModes = Device_Info.presentModes.value();
            }

            availableSurfaceFormats =
                UserItemsFoundInVulkanArray<vk::SurfaceFormatKHR,
                                            vk::SurfaceFormatKHR>(
                    availableSurfaceFormats.value(),
                    physicalDeviceSurfaceFormats,
                    [](vk::SurfaceFormatKHR Viable_Surface_Format,
                       vk::SurfaceFormatKHR Physical_Device_Surface_Format)
                        -> bool {
                        return (Viable_Surface_Format.format ==
                                Physical_Device_Surface_Format.format) &&
                               (Viable_Surface_Format.colorSpace ==
                                Physical_Device_Surface_Format.colorSpace);
                    });
            if (availableSurfaceFormats->empty()) {
                continue;
            }

            availablePresentModes =
                UserItemsFoundInVulkanArray<vk::PresentModeKHR,
                                            vk::PresentModeKHR>(
                    availablePresentModes.value(),
                    physicalDevicePresentModes,
                    [](vk::PresentModeKHR Viable_Present_Mode,
                       vk::PresentModeKHR Physical_Device_Present_Mode) {
                        return (Viable_Present_Mode ==
                                Physical_Device_Present_Mode);
                    });
            if (availablePresentModes->empty()) {
                continue;
            }
        }

        physicalDevicesInfoCompatible.emplace_back(
            physicalDevice, availableSurfaceFormats, availablePresentModes);
    }
    if (physicalDevicesInfoCompatible.empty()) {
        gvwErrorCallback("No physical devices with a compatible surface format "
                         "and present mode are available.");
    }

    std::vector<device_selection::returns> selectedPhysicalDevicesInfo =
        Device_Info.selectPhysicalDevicesAndQueues(
            physicalDevicesInfoCompatible, Window_Surface);
    if (selectedPhysicalDevicesInfo.empty()) {
        gvwErrorCallback("No physical devices were selected.");
    }

    std::vector<device_ptr> logicalDevices;
    logicalDevices.reserve(selectedPhysicalDevicesInfo.size());

    for (const auto& physicalDeviceInfo : selectedPhysicalDevicesInfo) {
        const auto& physicalDevice = physicalDeviceInfo.physicalDevice;
        const auto& surfaceFormat = physicalDeviceInfo.surfaceFormat;
        const auto& presentMode = physicalDeviceInfo.presentMode;
        const auto& queueFamilyInfos = physicalDeviceInfo.queueFamilyInfos;

        /// @todo Get queue create infos out of selection::queue_family_info
        /// without copying the data here.
        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        queueCreateInfos.reserve(queueFamilyInfos.size());
        std::transform(
            queueFamilyInfos.begin(),
            queueFamilyInfos.end(),
            std::back_inserter(queueCreateInfos),
            [](const device_selection::queue_family_info& Queue_Family_Info) {
                return Queue_Family_Info.createInfo;
            });

        vk::DeviceCreateInfo logicalDeviceCreateInfo = {
            .queueCreateInfoCount =
                static_cast<uint32_t>(queueCreateInfos.size()),
            .pQueueCreateInfos = queueCreateInfos.data(),
            .enabledLayerCount =
                static_cast<uint32_t>(this->vulkanInstanceLayers.size()),
            .ppEnabledLayerNames = this->vulkanInstanceLayers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(
                Device_Info.logicalDeviceExtensions->size()),
            .ppEnabledExtensionNames =
                Device_Info.logicalDeviceExtensions->data(),
            .pEnabledFeatures = &Device_Info.physicalDeviceFeatures.Get()
        };

        logicalDevices.emplace_back(std::make_shared<device_public_constructor>(
            std::make_shared<vk::UniqueDevice>(
                physicalDevice.createDeviceUnique(logicalDeviceCreateInfo)),
            physicalDevice,
            surfaceFormat,
            presentMode,
            queueFamilyInfos));
    }

    return logicalDevices;
}

gvw::monitor_ptr gvw::Monitor(GLFWmonitor* Monitor_Handle) // NOLINT
{
    return std::make_shared<monitor_public_constructor>(Get(), Monitor_Handle);
}

gvw::monitor_ptr gvw::PrimaryMonitor() // NOLINT
{
    std::scoped_lock<std::mutex> lock(glfwMutex);
    return Monitor(glfwGetPrimaryMonitor());
}

std::vector<gvw::monitor_ptr> gvw::AllMonitors()
{
    int monitorCount = 0;
    glfwMutex.lock();
    GLFWmonitor** monitorPointerArray = glfwGetMonitors(&monitorCount);
    glfwMutex.unlock();
    std::vector<monitor_ptr> monitors;
    monitors.reserve(monitorCount);
    for (size_t i = 0; i < static_cast<size_t>(monitorCount); ++i) {
        monitors.emplace_back(Monitor(monitorPointerArray[i])); // NOLINT
    }
    return monitors;
}