// Standard includes
#include <iostream>
#include <numeric>

// Local includes
#include "gvw.hpp"
#include "gvw.ipp"
#include "internal.hpp"
#include "internal.ipp"
#include "instance.hpp"
#include "monitor.hpp"
#include "window.hpp"
#include "device.hpp"
#include "impl.hpp"

namespace gvw {

bool instance::GlfwNotInitialized(const std::string& Function_Name) const
{
    return internal::NotInitializedTemplate(
        this->initializedGlfw, "GLFW is not initialized.", Function_Name);
}

bool instance::VulkanNotSupported(const std::string& Function_Name) const
{
    return internal::NotInitializedTemplate(
        this->vulkanSupported,
        "Vulkan is not supported on this system.",
        Function_Name);
}

bool instance::RequiredExtensionsNotSupported(
    const std::string& Function_Name) const
{
    return internal::NotInitializedTemplate(
        this->requiredExtensionsSupported,
        "At least one of the required Vulkan instance extensions is not "
        "supported on this system.",
        Function_Name);
}

bool instance::SelectedExtensionsNotSupported(
    const std::string& Function_Name) const
{
    return internal::NotInitializedTemplate(
        this->selectedExtensionsSupported,
        "At least one of the selected Vulkan instance extensions is not "
        "supported on this system.",
        Function_Name);
}

bool instance::SelectedLayersNotSupported(
    const std::string& Function_Name) const
{
    return internal::NotInitializedTemplate(
        this->selectedLayersSupported,
        "At least one of the selected Vulkan instance layers is not supported "
        "on this system.",
        Function_Name);
}

instance::instance(const instance_info& Instance_Info)
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
        if (glfwInit() == GLFW_TRUE) {
            this->initializedGlfw = true;
        } else {
            ErrorCallback("GLFW failed to initialize");
        }

        // Check for Vulkan support
        if (glfwVulkanSupported() == GLFW_TRUE) {
            this->vulkanSupported = true;
        } else {
            ErrorCallback("Failed to find the Vulkan loader and a minimally "
                          "functional ICD (Installable Client Driver)");
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
    if (unavailableRequiredInstanceExtensions.empty()) {
        this->requiredExtensionsSupported = true;
    } else {
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
    }

    std::vector<const char*> unsupportedSelectedExtensions =
        internal::GetUncommonElementsInArr1(
            Instance_Info.extensions,
            supportedInstanceExtensions,
            [](const char* Lhs, vk::ExtensionProperties Rhs) -> bool {
                return std::strcmp(Lhs, Rhs.extensionName) == 0;
            });
    if (unsupportedSelectedExtensions.empty()) {
        this->selectedExtensionsSupported = true;
    } else {
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
    }

    // Combine required instance extensions and selected instance extensions.
    instance_extensions instanceExtensions = requiredInstanceExtensions;
    /// @todo Benchmark the line below (may not need .reserve()).
    instanceExtensions.reserve(instanceExtensions.size() +
                               Instance_Info.extensions.size());
    instanceExtensions.insert(instanceExtensions.end(),
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
    if (unsupportedInstanceLayers.empty()) {
        this->selectedLayersSupported = true;
    } else {
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
    }

    /// @todo Create required instance layers (like required instance extensions
    /// but for instance layers).
    instance_layers instanceLayers = Instance_Info.layers;

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
        .enabledLayerCount = static_cast<uint32_t>(instanceLayers.size()),
        .ppEnabledLayerNames = instanceLayers.data(),
        .enabledExtensionCount =
            static_cast<uint32_t>(instanceExtensions.size()),
        .ppEnabledExtensionNames = instanceExtensions.data()
    };

    // Create the vulkan instance
    vk::UniqueInstance instance = vk::createInstanceUnique(instanceInfo);

    vk::DispatchLoaderDynamic dispatchLoaderDynamic;
    vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic>
        debugUtilsMessenger;
#ifdef GVW_VULKAN_VALIDATION_LAYERS
    // Automatically look up instance extension function addresses
    dispatchLoaderDynamic = { instance.get(), vkGetInstanceProcAddr };

    // Debug callback setup
    debugUtilsMessenger = instance->createDebugUtilsMessengerEXTUnique(
        debugUtilsMessengerCreateInfo, nullptr, dispatchLoaderDynamic);
#endif

    this->pImpl = std::make_unique<impl>(
        std::make_unique<internal::terminator<>>(TerminateGlfw),
        std::move(instanceExtensions),
        std::move(instanceLayers),
        std::move(instance),
        dispatchLoaderDynamic,
        std::move(debugUtilsMessenger));
}

void instance::TerminateGlfw()
{
    // Checking for initialization is unreliable during object destruction, so
    // don't check for initialization here.
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwTerminate();
}

void instance::SetJoystickEventCallback(
    instance_joystick_event_callback Joystick_Event_Callback)
{
    if (this->GlfwNotInitialized(static_cast<const char*>(__func__))) {
        return;
    }
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetJoystickCallback(Joystick_Event_Callback);
}

void instance::PollEvents()
{
    if (this->GlfwNotInitialized(static_cast<const char*>(__func__))) {
        return;
    }
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwPollEvents();
}

void instance::WaitThenPollEvents()
{
    if (this->GlfwNotInitialized(static_cast<const char*>(__func__))) {
        return;
    }
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwWaitEvents();
}

void instance::WaitThenPollEvents(double Timeout)
{
    if (this->GlfwNotInitialized(static_cast<const char*>(__func__))) {
        return;
    }
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwWaitEventsTimeout(Timeout);
}

void instance::PostEmptyEvent()
{
    if (this->GlfwNotInitialized(static_cast<const char*>(__func__))) {
        return;
    }
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwPostEmptyEvent();
}

window_ptr instance::CreateWindow(const window_info& Window_Info)
{
    if (this->GlfwNotInitialized(static_cast<const char*>(__func__)) ||
        this->VulkanNotSupported(static_cast<const char*>(__func__)) ||
        this->RequiredExtensionsNotSupported(
            static_cast<const char*>(__func__)) ||
        this->SelectedExtensionsNotSupported(
            static_cast<const char*>(__func__)) ||
        this->SelectedLayersNotSupported(static_cast<const char*>(__func__))) {
        /// @todo Return something that won't cause a segmentation fault.
        return nullptr;
    }
    return std::make_shared<internal::window_public_constructor>(Window_Info);
}

std::vector<device_ptr> instance::SelectPhysicalDevices(
    const device_selection_info& Device_Info,
    const vk::SurfaceKHR* Window_Surface)
{
    if (this->VulkanNotSupported(static_cast<const char*>(__func__)) ||
        this->RequiredExtensionsNotSupported(
            static_cast<const char*>(__func__)) ||
        this->SelectedExtensionsNotSupported(
            static_cast<const char*>(__func__)) ||
        this->SelectedLayersNotSupported(static_cast<const char*>(__func__))) {
        return {};
    }

    std::vector<vk::PhysicalDevice> physicalDevices =
        this->pImpl->vulkanInstance->enumeratePhysicalDevices();
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

monitor_ptr instance::GetMonitor(const monitor_info& Monitor_Info)
{
    if (this->GlfwNotInitialized(static_cast<const char*>(__func__))) {
        return nullptr;
    }
    return std::make_shared<internal::monitor_public_constructor>(Monitor_Info);
}

monitor_ptr instance::GetPrimaryMonitor()
{
    if (this->GlfwNotInitialized(static_cast<const char*>(__func__))) {
        return nullptr;
    }
    GLFWmonitor* primaryMonitor = nullptr;
    {
        std::scoped_lock lock(internal::global::GLFW_MUTEX);
        primaryMonitor = glfwGetPrimaryMonitor();
    }
    return this->GetMonitor(primaryMonitor);
}

std::vector<gvw::monitor_ptr> instance::GetAllMonitors()
{
    if (this->GlfwNotInitialized(static_cast<const char*>(__func__))) {
        return {};
    }
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

cursor_ptr instance::CreateCursor(cursor_standard_shape Cursor_Standard_Shape)
{
    if (this->GlfwNotInitialized(static_cast<const char*>(__func__))) {
        return nullptr;
    }
    return std::make_shared<internal::cursor_public_constructor>(
        Cursor_Standard_Shape);
}

cursor_ptr instance::CreateCursor(
    const cursor_custom_shape_info& Cursor_Custom_Shape_Info)
{
    if (this->GlfwNotInitialized(static_cast<const char*>(__func__))) {
        return nullptr;
    }
    return std::make_shared<internal::cursor_public_constructor>(
        Cursor_Custom_Shape_Info);
}

const char* instance::GetClipboard()
{
    if (this->GlfwNotInitialized(static_cast<const char*>(__func__))) {
        return "";
    }
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    return glfwGetClipboardString(nullptr);
}

void instance::SetClipboard(const char* Data)
{
    if (this->GlfwNotInitialized(static_cast<const char*>(__func__))) {
        return;
    }
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetClipboardString(nullptr, Data);
}

int instance::GetKeyScancode(window_key Key)
{
    if (this->GlfwNotInitialized(static_cast<const char*>(__func__))) {
        return 0;
    }
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    return glfwGetKeyScancode(static_cast<int>(Key));
}

const char* instance::GetKeyName(window_key Key, int Scancode)
{
    if (this->GlfwNotInitialized(static_cast<const char*>(__func__))) {
        return "";
    }
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    return glfwGetKeyName(static_cast<int>(Key), Scancode);
}

} // namespace gvw