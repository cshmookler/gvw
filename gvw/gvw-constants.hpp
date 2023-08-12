#pragma once

// Local includes
#include "gvw.hpp"

const std::vector<const char*> gvw::instance_layers::VALIDATION = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*>
    gvw::instance_extensions::PORTABILITY_AND_DEBUG_UTILS = {
        VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };

const float gvw::DEFAULT_MONITOR_GAMMA = 1.0F;

const gvw::window_event_callbacks gvw::NO_WINDOW_EVENT_CALLBACKS = {
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
};

const char* const gvw::window_title::BLANK = " ";
const char* const gvw::window_title::UNTITLED = "";

const gvw::area<int> gvw::window_size::W_500_H_500 = { 500, 500 };
const gvw::area<int> gvw::window_size::W_640_H_360 = { 640, 360 };

const gvw::area<int> gvw::window_size_limit::NO_MINIMUM = { GLFW_DONT_CARE,
                                                            GLFW_DONT_CARE };
const gvw::area<int> gvw::window_size_limit::NO_MAXIMUM = { GLFW_DONT_CARE,
                                                            GLFW_DONT_CARE };

const std::vector<vk::SurfaceFormatKHR> gvw::surface_formats::STANDARD = {
    { .format = vk::Format::eB8G8R8A8Srgb,
      .colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear }
};

const std::vector<vk::PresentModeKHR> gvw::present_modes::FIFO = {
    vk::PresentModeKHR::eFifo
};
const std::vector<vk::PresentModeKHR> gvw::present_modes::MAILBOX = {
    vk::PresentModeKHR::eMailbox
};
const std::vector<vk::PresentModeKHR> gvw::present_modes::MAILBOX_OR_FIFO = {
    vk::PresentModeKHR::eMailbox,
    vk::PresentModeKHR::eFifo
};

const vk::PhysicalDeviceFeatures gvw::physical_device_features::NONE = {};

const std::vector<const char*> gvw::logical_device_extensions::SWAPCHAIN = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const float gvw::queue_priority::HIGH = 1.0F;

const std::vector<vk::DynamicState> gvw::dynamic_states::VIEWPORT = {
    vk::DynamicState::eViewport
};
const std::vector<vk::DynamicState>
    gvw::dynamic_states::VIEWPORT_AND_SCISSOR = { vk::DynamicState::eViewport,
                                                  vk::DynamicState::eScissor };

const char* const gvw::shader::VERTEX_BASIC =
    "#version 450\n"
    "layout(location = 0) in vec2 inPosition;\n"
    "layout(location = 1) in vec3 inColor;\n"
    ""
    "layout(location = 0) out vec3 fragColor;\n"
    ""
    "void main() {\n"
    "    gl_Position = vec4(inPosition, 0.0, 1.0);\n"
    "    fragColor = inColor;\n"
    "}\n";

const char* const gvw::shader::FRAGMENT_BASIC =
    "#version 450\n"
    ""
    "layout(location = 0) in vec3 fragColor;\n"
    ""
    "layout(location = 0) out vec4 outColor;\n"
    ""
    "void main() {\n"
    "    outColor = vec4(fragColor, 1.0);"
    "}\n";

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