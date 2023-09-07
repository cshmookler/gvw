// Standard includes
#include <iostream>
#include <numeric>

// Local includes
#include "gvw.hpp"

namespace gvw {

instance::instance(const instance_info& Instance_Info)
    : glfwTerminator(std::make_unique<internal::terminator<>>(TerminateGlfw))
{
    // Set warning and error callbacks.
    SetVerboseCallback(Instance_Info.verboseCallback);
    SetInfoCallback(Instance_Info.infoCallback);
    SetWarningCallback(Instance_Info.warningCallback);
    SetErrorCallback(Instance_Info.errorCallback);

    version compiletimeGlfw = GetGlfwCompiletimeVersion();
    version runtimeGlfw = GetGlfwRuntimeVersion();
    if (runtimeGlfw < compiletimeGlfw) {
        std::string message = "The GLFW runtime version (" +
                              static_cast<std::string>(runtimeGlfw) +
                              ") is lower than the GLFW compiletime version (" +
                              static_cast<std::string>(compiletimeGlfw) + ").";
        WarningCallback(message.c_str());
    }

    // Set the GLFW error callback.
    SetGlfwCallback(Instance_Info.glfwErrorCallback);

    uint32_t requiredInstanceExtensionCount = 0;
    const char** requiredInstanceExtensionsPointer = nullptr;
    {
        // Lock the GLFW mutex while invoking GLFW functions.
        std::scoped_lock lock(internal::global::GLFW_MUTEX);

        // Apply GLFW initialization hints.
        Instance_Info.initHints.Apply();

        // Initialize the GLFW library.
        if (glfwInit() == GLFW_FALSE) {
            ErrorCallback("GLFW failed to initialize");
            /// @todo The program should always terminate here.
            return;
        }

        // Check for Vulkan support
        if (glfwVulkanSupported() == GLFW_FALSE) {
            ErrorCallback("Failed to find the Vulkan loader and/or a minimally "
                          "functional "
                          "ICD (Installable Client Driver)");
            /// @todo The program should always terminate here.
            return;
        }

        // Get required instance extensions
        requiredInstanceExtensionsPointer =
            glfwGetRequiredInstanceExtensions(&requiredInstanceExtensionCount);
    }

    // Verify that the requested extensions are available
    std::vector<vk::ExtensionProperties> supportedInstanceExtensions =
        vk::enumerateInstanceExtensionProperties();

    std::vector<const char*> requiredInstanceExtensions;
    requiredInstanceExtensions.reserve(requiredInstanceExtensionCount);
    for (uint32_t i = 0; i < requiredInstanceExtensionCount; ++i) {
        requiredInstanceExtensions.emplace_back(
            requiredInstanceExtensionsPointer[i]); // NOLINT
    }
    std::vector<const char*> unavailableRequiredInstanceExtensions =
        internal::GetUncommonElementsInArr1(
            requiredInstanceExtensions,
            supportedInstanceExtensions,
            [](const char* Lhs, vk::ExtensionProperties Rhs) {
                return std::strcmp(Lhs, Rhs.extensionName) == 0;
            });
    if (!unavailableRequiredInstanceExtensions.empty()) {
        std::string message =
            "The following Vulkan instance extensions are required for window "
            "surface creation but are not supported on this system:" +
            std::accumulate(
                unavailableRequiredInstanceExtensions.begin(),
                unavailableRequiredInstanceExtensions.end(),
                std::string{},
                [](const std::string& String, const char* Extension_Name) {
                    return String + "\n\t" + Extension_Name;
                });
        ErrorCallback(message.c_str());
        /// @todo The program should always terminate here (or surface
        /// creation can be made optional).
        return;
    }

    std::vector<const char*> unsupportedSelectedExtensions =
        internal::GetUncommonElementsInArr1(
            Instance_Info.extensions,
            supportedInstanceExtensions,
            [](const char* Lhs, vk::ExtensionProperties Rhs) -> bool {
                return std::strcmp(Lhs, Rhs.extensionName) == 0;
            });
    if (!unsupportedSelectedExtensions.empty()) {
        std::string message =
            "The following Vulkan instance extensions were requested but are "
            "not supported on this system:" +
            std::accumulate(
                unsupportedSelectedExtensions.begin(),
                unsupportedSelectedExtensions.end(),
                std::string{},
                [](const std::string& String, const char* Extension_Name) {
                    return String + "\n\t" + Extension_Name;
                });
        ErrorCallback(message.c_str());
        /// @todo The program should always terminate here (or the programmer
        /// should have some way of checking which extensions were unsupported).
        return;
    }

    // Combine required instance extensions and selected instance extensions.
    this->vulkanInstanceExtensions = requiredInstanceExtensions;
    /// @todo Benchmark the line below (may not need .reserve()).
    this->vulkanInstanceExtensions.reserve(
        this->vulkanInstanceExtensions.size() +
        Instance_Info.extensions.size());
    this->vulkanInstanceExtensions.insert(this->vulkanInstanceExtensions.end(),
                                          Instance_Info.extensions.begin(),
                                          Instance_Info.extensions.end());

    // Verify that the requested layers are available
    std::vector<vk::LayerProperties> supportedInstanceLayers =
        vk::enumerateInstanceLayerProperties();
    std::vector<const char*> unsupportedInstanceLayers =
        internal::GetUncommonElementsInArr1(
            Instance_Info.layers,
            supportedInstanceLayers,
            [](const char* Lhs, vk::LayerProperties Rhs) {
                return std::strcmp(Lhs, Rhs.layerName) == 0;
            });
    if (!unsupportedInstanceLayers.empty()) {
        std::string message =
            "The following Vulkan instance layers were requested but are not "
            "supported on this system: " +
            std::accumulate(
                unsupportedInstanceLayers.begin(),
                unsupportedInstanceLayers.end(),
                std::string{},
                [](const std::string& String, const char* Layer_Name) {
                    return String + "\n\t" + Layer_Name;
                });
        ErrorCallback(message.c_str());
        /// @todo The program should always terminate here (or the programmer
        /// should have some way of checking which layers were unsupported).
        return;
    }

    /// @todo Create required instance layers (like required instance extensions
    /// but for instance layers).
    this->vulkanInstanceLayers = Instance_Info.layers;

    vk::ApplicationInfo applicationInfo = {
        .pNext = nullptr,
        .pApplicationName = Instance_Info.applicationInfo.pApplicationName,
        .applicationVersion = Instance_Info.applicationInfo.applicationVersion,
        .pEngineName = Instance_Info.applicationInfo.pEngineName,
        .engineVersion = Instance_Info.applicationInfo.engineVersion,
        .apiVersion = Instance_Info.applicationInfo.apiVersion
    };

    vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {
        .pNext = nullptr,
        .flags = {},
        .messageSeverity =
            Instance_Info.debugUtilsMessengerInfo.messageSeverity,
        .messageType = Instance_Info.debugUtilsMessengerInfo.messageType,
        .pfnUserCallback =
            Instance_Info.debugUtilsMessengerInfo.pfnUserCallback,
        .pUserData = Instance_Info.debugUtilsMessengerInfo.pUserData
    };

    // Create the instance creation information struct
    vk::InstanceCreateInfo instanceInfo = {
        .pNext = &debugUtilsMessengerCreateInfo,
        .flags = Instance_Info.creationFlags,
        .pApplicationInfo = &applicationInfo,
        .enabledLayerCount =
            static_cast<uint32_t>(this->vulkanInstanceLayers.size()),
        .ppEnabledLayerNames = this->vulkanInstanceLayers.data(),
        .enabledExtensionCount =
            static_cast<uint32_t>(this->vulkanInstanceExtensions.size()),
        .ppEnabledExtensionNames = this->vulkanInstanceExtensions.data()
    };

    // Create the vulkan instance
    this->vulkanInstance = vk::createInstanceUnique(instanceInfo);

#ifdef GVW_VULKAN_VALIDATION_LAYERS
    // Automatically look up instance extension function addresses
    this->vulkanDispatchLoaderDynamic = { vulkanInstance.get(),
                                          vkGetInstanceProcAddr };

    // Debug callback setup
    this->vulkanDebugUtilsMessenger =
        this->vulkanInstance->createDebugUtilsMessengerEXTUnique(
            debugUtilsMessengerCreateInfo,
            nullptr,
            this->vulkanDispatchLoaderDynamic);
#endif
}

void instance::TerminateGlfw()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwTerminate();
}

// NOLINTNEXTLINE
void instance::SetJoystickEventCallback(
    instance_joystick_event_callback Joystick_Event_Callback)
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetJoystickCallback(Joystick_Event_Callback);
}

// NOLINTNEXTLINE
const std::vector<instance_joystick_event>& instance::GetJoystickEvents()
{
    std::scoped_lock lock(internal::global::JOYSTICK_EVENTS_MUTEX);
    return internal::global::JOYSTICK_EVENTS;
}

// NOLINTNEXTLINE
void instance::ClearJoystickEvents()
{
    std::scoped_lock lock(internal::global::JOYSTICK_EVENTS_MUTEX);
    internal::global::JOYSTICK_EVENTS.clear();
}

// NOLINTNEXTLINE
void instance::PollEvents()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwPollEvents();
}

// NOLINTNEXTLINE
void instance::WaitThenPollEvents()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwWaitEvents();
}

// NOLINTNEXTLINE
void instance::WaitThenPollEvents(double Timeout)
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwWaitEventsTimeout(Timeout);
}

// NOLINTNEXTLINE
void instance::PostEmptyEvent()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwPostEmptyEvent();
}

// NOLINTNEXTLINE
window_ptr instance::CreateWindow(const window_info& Window_Info)
{
    return std::make_shared<internal::window_public_constructor>(Window_Info);
}

std::vector<device_ptr> instance::SelectPhysicalDevices(
    const device_selection_info& Device_Info,
    const vk::SurfaceKHR* Window_Surface)
{
    std::vector<vk::PhysicalDevice> physicalDevices =
        this->vulkanInstance->enumeratePhysicalDevices();
    if (physicalDevices.empty()) {
        ErrorCallback("No physical devices with Vulkan support are available.");
    }

    std::vector<vk::SurfaceFormatKHR> selectedSurfaceFormats =
        Device_Info.surfaceFormats;
    std::vector<vk::PresentModeKHR> selectedPresentModes =
        Device_Info.presentModes;

    std::vector<device_selection_parameter> compatiblePhysicalDevices;
    for (const auto& physicalDevice : physicalDevices) {

        std::vector<vk::SurfaceFormatKHR> availableSurfaceFormats =
            physicalDevice.getSurfaceFormatsKHR(*Window_Surface);
        if (availableSurfaceFormats.empty()) {
            /// @todo Log this.
            continue;
        }
        std::vector<vk::SurfaceFormatKHR> viableSurfaceFormats =
            internal::GetCommonElementsInArr1(
                selectedSurfaceFormats,
                availableSurfaceFormats,
                [](const vk::SurfaceFormatKHR& Lhs,
                   const vk::SurfaceFormatKHR& Rhs) { return Lhs == Rhs; });
        if (viableSurfaceFormats.empty()) {
            /// @todo Log this.
            continue;
        }

        std::vector<vk::PresentModeKHR> availablePresentModes =
            physicalDevice.getSurfacePresentModesKHR(*Window_Surface);
        if (availablePresentModes.empty()) {
            /// @todo Log this.
            continue;
        }
        std::vector<vk::PresentModeKHR> viablePresentModes =
            internal::GetCommonElementsInArr1(
                selectedPresentModes,
                availablePresentModes,
                [](const vk::PresentModeKHR& Lhs,
                   const vk::PresentModeKHR& Rhs) { return Lhs == Rhs; });
        if (viablePresentModes.empty()) {
            /// @todo Log this.
            continue;
        }

        compatiblePhysicalDevices.emplace_back(
            physicalDevice, viableSurfaceFormats, viablePresentModes);
    }
    if (compatiblePhysicalDevices.empty()) {
        ErrorCallback("No physical devices support a selected surface format "
                      "and/or present mode.");
        return {};
    }

    /// @todo Remove std::optional from this function.
    std::vector<device_info> selectedPhysicalDevicesInfo =
        Device_Info.selectPhysicalDevicesAndQueues(compatiblePhysicalDevices,
                                                   *Window_Surface);
    if (selectedPhysicalDevicesInfo.empty()) {
        ErrorCallback("No physical devices were selected.");
        return {};
    }

    std::vector<device_ptr> logicalDevices;
    logicalDevices.reserve(selectedPhysicalDevicesInfo.size());

    for (auto& physicalDeviceInfo : selectedPhysicalDevicesInfo) {
        physicalDeviceInfo.logicalDeviceExtensions =
            Device_Info.logicalDeviceExtensions;
        physicalDeviceInfo.physicalDeviceFeatures =
            Device_Info.physicalDeviceFeatures;

        logicalDevices.emplace_back(
            std::make_shared<internal::device_public_constructor>(
                physicalDeviceInfo));
    }

    return logicalDevices;
}

// NOLINTNEXTLINE
monitor_ptr instance::GetMonitor(const monitor_info& Monitor_Info)
{
    return std::make_shared<internal::monitor_public_constructor>(Monitor_Info);
}

// NOLINTNEXTLINE
monitor_ptr instance::GetPrimaryMonitor()
{
    GLFWmonitor* primaryMonitor = nullptr;
    {
        std::scoped_lock lock(internal::global::GLFW_MUTEX);
        primaryMonitor = glfwGetPrimaryMonitor();
    }
    return this->GetMonitor(primaryMonitor);
}

// NOLINTNEXTLINE
std::vector<gvw::monitor_ptr> instance::AllMonitors()
{
    int monitorCount = 0;
    GLFWmonitor** monitorPointerArray = nullptr;
    {
        std::scoped_lock lock(internal::global::GLFW_MUTEX);
        monitorPointerArray = glfwGetMonitors(&monitorCount);
    }
    std::vector<monitor_ptr> monitors;
    monitors.reserve(monitorCount);
    for (size_t i = 0; i < static_cast<size_t>(monitorCount); ++i) {
        monitors.emplace_back(GetMonitor(monitorPointerArray[i])); // NOLINT
    }
    return monitors;
}

// NOLINTNEXTLINE
cursor_ptr instance::CreateCursor(cursor_standard_shape Cursor_Standard_Shape)
{
    return std::make_shared<internal::cursor_public_constructor>(
        Cursor_Standard_Shape);
}

// NOLINTNEXTLINE
cursor_ptr instance::CreateCursor(
    const cursor_custom_shape_info& Cursor_Custom_Shape_Info)
{
    return std::make_shared<internal::cursor_public_constructor>(
        Cursor_Custom_Shape_Info);
}

const char* instance::GetClipboard() // NOLINT
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    return glfwGetClipboardString(nullptr);
}

void instance::SetClipboard(const char* Data) // NOLINT
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetClipboardString(nullptr, Data);
}

int instance::GetKeyScancode(window_key Key) // NOLINT
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    return glfwGetKeyScancode(static_cast<int>(Key));
}

const char* instance::GetKeyName(window_key Key, int Scancode) // NOLINT
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    return glfwGetKeyName(static_cast<int>(Key), Scancode);
}

} // namespace gvw