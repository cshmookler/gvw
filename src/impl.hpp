#pragma once

// Local includes
#include "gvw.hpp"
#include "gvw.ipp"
#include "internal.hpp"
#include "internal.ipp"
#include "instance.hpp"

namespace gvw {

struct instance::impl
{
    /// @todo Determine if these Vulkan related variables need to be
    /// thread-safe.
    std::unique_ptr<internal::terminator<>> glfwTerminator;

    instance_extensions vulkanInstanceExtensions;
    instance_layers vulkanInstanceLayers;

    vk::UniqueInstance vulkanInstance;
    vk::DispatchLoaderDynamic vulkanDispatchLoaderDynamic;
    vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic>
        vulkanDebugUtilsMessenger;
};

} // namespace gvw