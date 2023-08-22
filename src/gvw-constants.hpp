#pragma once

// Local includes
#include "gvw.hpp"

const gvw::instance_layers gvw::instance_layers_constants::VALIDATION = {
#ifdef GVW_VULKAN_VALIDATION_LAYERS
    { "VK_LAYER_KHRONOS_validation" }
#endif
};

const gvw::instance_extensions
    gvw::instance_extensions_constants::PORTABILITY_AND_DEBUG_UTILS = {
        { VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
          VK_EXT_DEBUG_UTILS_EXTENSION_NAME }
    };

const float gvw::DEFAULT_MONITOR_GAMMA = 1.0F;

const gvw::window_event_callbacks gvw::NO_WINDOW_EVENT_CALLBACKS = {
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
};

const gvw::window_title gvw::window_title_constants::BLANK = " ";
const gvw::window_title gvw::window_title_constants::UNTITLED = "";

const gvw::window_size gvw::window_size_constants::W_500_H_500 = { { 500,
                                                                     500 } };
const gvw::window_size gvw::window_size_constants::W_640_H_360 = { { 640,
                                                                     360 } };

const gvw::window_size_limit gvw::window_size_limit_constants::NO_MINIMUM = {
    { GLFW_DONT_CARE, GLFW_DONT_CARE }
};
const gvw::window_size_limit gvw::window_size_limit_constants::NO_MAXIMUM = {
    { GLFW_DONT_CARE, GLFW_DONT_CARE }
};

const gvw::surface_formats gvw::surface_formats_constants::STANDARD = {
    { { .format = vk::Format::eB8G8R8A8Srgb,
        .colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear } }
};

const gvw::present_modes gvw::present_modes_constants::FIFO = {
    { vk::PresentModeKHR::eFifo }
};
const gvw::present_modes gvw::present_modes_constants::MAILBOX = {
    { vk::PresentModeKHR::eMailbox }
};
const gvw::present_modes gvw::present_modes_constants::MAILBOX_OR_FIFO = {
    { vk::PresentModeKHR::eMailbox, vk::PresentModeKHR::eFifo }
};

const gvw::physical_device_features
    gvw::physical_device_features_constants::NONE = {};

const gvw::logical_device_extensions
    gvw::logical_device_extensions_constants::SWAPCHAIN = {
        { VK_KHR_SWAPCHAIN_EXTENSION_NAME }
    };

const gvw::queue_priority gvw::queue_priority_constants::HIGH = 1.0F;

const gvw::dynamic_states gvw::dynamic_states_constants::VIEWPORT = {
    { vk::DynamicState::eViewport }
};
const gvw::dynamic_states
    gvw::dynamic_states_constants::VIEWPORT_AND_SCISSOR = {
        { vk::DynamicState::eViewport, vk::DynamicState::eScissor }
    };

const gvw::application_info gvw::DEFAULT_APPLICATION_INFO;

const gvw::debug_utils_messenger_info gvw::DEFAULT_UTILS_MESSENGER_INFO;

const gvw::init_hints::info gvw::init_hints::DEFAULT;
const gvw::init_hints gvw::DEFAULT_INIT_HINTS;

const gvw::info gvw::DEFAULT_INIT_INFO;

const gvw::window_creation_hints::info gvw::window_creation_hints::DEFAULT;
const gvw::window_creation_hints gvw::DEFAULT_WINDOW_CREATION_HINTS;

const gvw::window_info gvw::DEFAULT_WINDOW_INFO;

const gvw::buffer_info gvw::DEFAULT_BUFFER_INFO;

const gvw::render_pass_info gvw::DEFAULT_RENDER_PASS_INFO;

const gvw::swapchain_info gvw::DEFAULT_SWAPCHAIN_INFO;

const gvw::pipeline_info gvw::DEFAULT_PIPELINE_INFO;

const gvw::device_info gvw::DEFAULT_DEVICE_INFO;

const std::vector<gvw::shader> gvw::NO_SHADERS;

const std::vector<vk::VertexInputBindingDescription>
    gvw::NO_VERTEX_BINDING_DESCRIPTIONS;

const std::vector<vk::VertexInputAttributeDescription>
    gvw::NO_VERTEX_ATTRIBUTE_DESCRIPTIONS;