// Standard includes
#include <iostream>
#include <numeric>

// Local includes
#include "gvw.hpp"

namespace gvw {

instance::instance(const instance_info& Instance_Info)
    : glfwTerminator(std::make_unique<internal::terminator<>>(TerminateGlfw))
    , vulkanInstanceExtensions(Instance_Info.extensions)
    , vulkanInstanceLayers(Instance_Info.layers)
{
    // Set warning and error callbacks.
    SetWarningCallback(Instance_Info.warningCallback);
    SetErrorCallback(Instance_Info.errorCallback);
    SetGlfwCallback(Instance_Info.glfwErrorCallback);

    {
        // Lock the GLFW mutex while invoking GLFW functions.
        std::scoped_lock lock(internal::global::GLFW_MUTEX);

        // Apply GLFW initialization hints.
        Instance_Info.initHints.Apply();

        // Initialize the GLFW library.
        if (glfwInit() == GLFW_FALSE) {
            ErrorCallback("GLFW failed to initialize");
        }

        // Check for Vulkan support
        if (glfwVulkanSupported() == GLFW_FALSE) {
            ErrorCallback("Failed to find the Vulkan loader and/or a minimally "
                          "functional "
                          "ICD (Installable Client Driver)");
        }

        // Get required instance extensions
        uint32_t extensionCount = 0;
        const char** extensionsPointer =
            glfwGetRequiredInstanceExtensions(&extensionCount);
        for (uint32_t i = 0; i < extensionCount; ++i) {
            this->vulkanInstanceExtensions.emplace_back(
                extensionsPointer[i]); // NOLINT
        }
    }

    // Verify that the requested extensions are available
    std::vector<vk::ExtensionProperties> allAvailableExtensions =
        vk::enumerateInstanceExtensionProperties();
    std::vector<const char*> extensionsNotFound =
        internal::UserItemsMissingInVulkanArray<const char*,
                                                vk::ExtensionProperties>(
            this->vulkanInstanceExtensions,
            allAvailableExtensions,
            [](const char* User, vk::ExtensionProperties Vulkan) {
                return std::strcmp(User, Vulkan.extensionName) == 0;
            });
    if (!extensionsNotFound.empty()) {
        ErrorCallback(
            ("The following Vulkan instance extensions were requested but are "
             "not supported on this system: " +
             std::accumulate(
                 extensionsNotFound.begin(),
                 extensionsNotFound.end(),
                 std::string(),
                 [](const std::string& String, const char* Extension) {
                     return String + "\n\t" + Extension;
                 }))
                .c_str());
    }

    // Verify that the requested layers are available
    std::vector<vk::LayerProperties> allAvailableLayers =
        vk::enumerateInstanceLayerProperties();
    std::vector<const char*> layersNotFound =
        internal::UserItemsMissingInVulkanArray<const char*,
                                                vk::LayerProperties>(
            this->vulkanInstanceLayers,
            allAvailableLayers,
            [](const char* User, vk::LayerProperties Vulkan) {
                return std::strcmp(User, Vulkan.layerName) == 0;
            });
    if (!layersNotFound.empty()) {
        ErrorCallback(
            ("The following Vulkan instance layers were requested but are not "
             "supported on this system: " +
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
            &Instance_Info.debugUtilsMessengerInfo),
        .flags = Instance_Info.creationFlags,
        // NOLINTNEXTLINE
        .pApplicationInfo = reinterpret_cast<const vk::ApplicationInfo*>(
            &Instance_Info.applicationInfo),
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
            // NOLINTNEXTLINE
            *reinterpret_cast<const vk::DebugUtilsMessengerCreateInfoEXT*>(
                &Instance_Info.debugUtilsMessengerInfo),
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
    const std::optional<vk::SurfaceKHR>& Window_Surface)
{
    std::vector<vk::PhysicalDevice> physicalDevices =
        this->vulkanInstance->enumeratePhysicalDevices();
    if (physicalDevices.empty()) {
        ErrorCallback("No physical devices with Vulkan support are available.");
    }

    std::vector<device_selection_parameter> physicalDevicesInfoCompatible;
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

            availableSurfaceFormats = Device_Info.surfaceFormats;
            availablePresentModes = Device_Info.presentModes;

            // if (!Device_Info.surfaceFormats.has_value()) {
            //     availableSurfaceFormats = physicalDeviceSurfaceFormats;
            // } else {
            //     availableSurfaceFormats = Device_Info.surfaceFormats.value();
            // }
            // if (!Device_Info.presentModes.has_value()) {
            //     availablePresentModes = physicalDevicePresentModes;
            // } else {
            //     availablePresentModes = Device_Info.presentModes.value();
            // }

            availableSurfaceFormats =
                internal::UserItemsFoundInVulkanArray<vk::SurfaceFormatKHR,
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
                internal::UserItemsFoundInVulkanArray<vk::PresentModeKHR,
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
        ErrorCallback("No physical devices support a selected surface format "
                      "and/or present mode.");
    }

    std::vector<device_info> selectedPhysicalDevicesInfo =
        Device_Info.selectPhysicalDevicesAndQueues(
            physicalDevicesInfoCompatible, Window_Surface);
    if (selectedPhysicalDevicesInfo.empty()) {
        ErrorCallback("No physical devices were selected.");
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
cursor_ptr instance::CreateCursor(
    const standard_cursor_info& Standard_Cursor_Info)
{
    return std::make_shared<internal::cursor_public_constructor>(
        Standard_Cursor_Info);
}

// NOLINTNEXTLINE
cursor_ptr instance::CreateCursor(const custom_cursor_info& Custom_Cursor_Info)
{
    return std::make_shared<internal::cursor_public_constructor>(
        Custom_Cursor_Info);
}

} // namespace gvw