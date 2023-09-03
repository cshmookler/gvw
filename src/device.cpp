// Standard includes
#include <iostream>
#include <fstream>

// Local includes
#include "device.hpp"

namespace gvw {

device::device(const device_info& Device_Info)
    : gvwInstance(internal::global::GVW_INSTANCE)
    , physicalDevice(Device_Info.physicalDevice)
    , surfaceFormat(Device_Info.surfaceFormat)
    , presentMode(Device_Info.presentMode)
    , queueFamilyInfos(Device_Info.queueFamilyInfos)
{
    internal::AssertInitialization();

    /// @todo Get queue create infos out of selection::queue_family_info
    /// without copying the data here.
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    queueCreateInfos.reserve(this->queueFamilyInfos.size());
    std::transform(
        queueFamilyInfos.begin(),
        queueFamilyInfos.end(),
        std::back_inserter(queueCreateInfos),
        [](const device_selection_queue_family_info& Queue_Family_Info) {
            return Queue_Family_Info.createInfo;
        });

    vk::DeviceCreateInfo logicalDeviceCreateInfo = {
        .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
        .pQueueCreateInfos = queueCreateInfos.data(),
        .enabledLayerCount = static_cast<uint32_t>(
            this->gvwInstance->vulkanInstanceLayers.size()),
        .ppEnabledLayerNames = this->gvwInstance->vulkanInstanceLayers.data(),
        .enabledExtensionCount =
            static_cast<uint32_t>(Device_Info.logicalDeviceExtensions.size()),
        .ppEnabledExtensionNames = Device_Info.logicalDeviceExtensions.data(),
        .pEnabledFeatures = &Device_Info.physicalDeviceFeatures
    };
    this->handle = physicalDevice.createDeviceUnique(logicalDeviceCreateInfo);
}

vk::Device device::GetHandle() const
{
    return this->handle.get();
}

vk::PhysicalDevice device::GetPhysicalDevice() const
{
    return this->physicalDevice;
}

vk::SurfaceFormatKHR device::GetSurfaceFormat() const
{
    return this->surfaceFormat;
}

vk::PresentModeKHR device::GetPresentMode() const
{
    return this->presentMode;
}

std::vector<device_selection_queue_family_info> device::GetQueueFamilyInfos()
    const
{
    return this->queueFamilyInfos;
}

shader_ptr device::LoadShaderFromSpirVFile(const shader_info& Shader_Info)
{
    auto charBuffer = ReadFile(Shader_Info.code);

    vk::ShaderModuleCreateInfo shaderModuleCreateInfo = {
        .codeSize = charBuffer.size(),
        .pCode = reinterpret_cast<const uint32_t*>(charBuffer.data()) // NOLINT
    };
    return std::make_shared<internal::shader_public_constructor>(
        this->handle->createShaderModuleUnique(shaderModuleCreateInfo),
        Shader_Info.stage,
        Shader_Info.entryPoint);
}

vertex_shader_ptr device::LoadVertexShaderFromSpirVFile(
    const vertex_shader_info& Vertex_Shader_Info)
{
    shader_ptr genericShader =
        LoadShaderFromSpirVFile(Vertex_Shader_Info.general);
    return std::make_shared<internal::vertex_shader_public_constructor>(
        std::move(genericShader->handle),
        genericShader->stage,
        genericShader->entryPoint,
        Vertex_Shader_Info.bindingDescriptions,
        Vertex_Shader_Info.attributeDescriptions);
}

fragment_shader_ptr device::LoadFragmentShaderFromSpirVFile(
    const fragment_shader_info& Fragment_Shader_Info)
{
    // NOLINTBEGIN
    shader_ptr genericShader =
        LoadShaderFromSpirVFile(Fragment_Shader_Info.general);
    // NOLINTEND
    return std::make_shared<internal::fragment_shader_public_constructor>(
        std::move(genericShader->handle),
        genericShader->stage,
        genericShader->entryPoint);
}

buffer_ptr device::CreateBuffer(const buffer_info& Buffer_Info)
{
    buffer_ptr buffer = std::make_shared<internal::buffer_public_constructor>(
        Buffer_Info.sizeInBytes,
        vk::UniqueBuffer(nullptr),
        vk::UniqueDeviceMemory(nullptr));

    vk::BufferCreateInfo bufferCreateInfo = {
        .size = buffer->size,
        .usage = Buffer_Info.usage,
        // Sharing is exclusive when only one queue has access to the buffer.
        .sharingMode = vk::SharingMode::eExclusive
    };
    buffer->handle = this->handle->createBufferUnique(bufferCreateInfo);

    vk::MemoryRequirements memoryRequirements =
        this->handle->getBufferMemoryRequirements(buffer->handle.get());

    vk::PhysicalDeviceMemoryProperties memoryProperties =
        this->physicalDevice.getMemoryProperties();

    std::optional<uint32_t> memoryTypeIndex;
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
        if (((memoryRequirements.memoryTypeBits & (1 << i)) != 0U) &&
            ((memoryProperties.memoryTypes.at(i).propertyFlags &
              Buffer_Info.memoryProperties) == Buffer_Info.memoryProperties)) {
            memoryTypeIndex = i;
        }
    }

    if (memoryTypeIndex.has_value() == false) {
        ErrorCallback(
            "Failed to find a viable memory type for a Vulkan buffer.");
    }

    vk::MemoryAllocateInfo memoryAllocateInfo = { .allocationSize =
                                                      memoryRequirements.size,
                                                  .memoryTypeIndex =
                                                      memoryTypeIndex.value() };
    buffer->memory = this->handle->allocateMemoryUnique(memoryAllocateInfo);

    this->handle->bindBufferMemory(
        buffer->handle.get(), buffer->memory.get(), 0);

    return buffer;
}

render_pass_ptr device::CreateRenderPass(
    const render_pass_info& Render_Pass_Info)
{
    // Describe how to use the attachment.
    vk::AttachmentDescription attachmentDescription = {
        .format = Render_Pass_Info.format,
        .samples = Render_Pass_Info.samples,
        .loadOp = vk::AttachmentLoadOp::eClear,
        .storeOp = vk::AttachmentStoreOp::eStore,
        .stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
        .stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
        .initialLayout = vk::ImageLayout::eUndefined,
        .finalLayout = vk::ImageLayout::ePresentSrcKHR
    };

    // Provide the layout index of 'outColor' in the fragment shader (0).
    vk::AttachmentReference colorAttachmentReference = {
        .attachment = Render_Pass_Info.graphicsAttachment,
        .layout = Render_Pass_Info.graphicsLayout
    };

    // Define the subpass.
    vk::SubpassDescription subpass = { .pipelineBindPoint =
                                           vk::PipelineBindPoint::eGraphics,
                                       .colorAttachmentCount = 1,
                                       .pColorAttachments =
                                           &colorAttachmentReference };

    // Create subpass dependency information.
    vk::SubpassDependency subpassDependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
        .dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
        .srcAccessMask = vk::AccessFlagBits::eNone,
        .dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite
    };

    // Create the render pass.
    vk::RenderPassCreateInfo renderPassCreateInfo = {
        .attachmentCount = 1,
        .pAttachments = &attachmentDescription,
        .subpassCount = 1,
        .pSubpasses = &subpass,
        .dependencyCount = 1,
        .pDependencies = &subpassDependency
    };

    return std::make_shared<internal::render_pass_public_constructor>(
        this->handle->createRenderPassUnique(renderPassCreateInfo));
}

swapchain_ptr device::CreateSwapchain(const swapchain_info& Swapchain_Info)
{
    vk::SurfaceCapabilitiesKHR surfaceCapabilities =
        this->physicalDevice.getSurfaceCapabilitiesKHR(Swapchain_Info.surface);
    vk::Extent2D framebufferExtent = {
        .width = std::clamp(
            static_cast<uint32_t>(Swapchain_Info.framebufferSize.width),
            surfaceCapabilities.minImageExtent.width,
            surfaceCapabilities.maxImageExtent.width),
        .height = std::clamp(
            static_cast<uint32_t>(Swapchain_Info.framebufferSize.height),
            surfaceCapabilities.minImageExtent.height,
            surfaceCapabilities.maxImageExtent.height)
    };

    swapchain_ptr swapchainInfo;
    if (Swapchain_Info.oldSwapchain == nullptr) {
        swapchainInfo =
            std::make_shared<internal::swapchain_public_constructor>();
    } else {
        swapchainInfo = Swapchain_Info.oldSwapchain;
    }

    // Define the viewport dimensions (almost always the same as the swap
    // chain extent).
    swapchainInfo->viewport =
        vk::Viewport{ .x = 0.0F,
                      .y = 0.0F,
                      .width = static_cast<float>(framebufferExtent.width),
                      .height = static_cast<float>(framebufferExtent.height),
                      .minDepth = 0.0F,
                      .maxDepth = 1.0F };

    // Define the scissor dimensions which trims the viewport (usually the
    // same size as the viewport)
    swapchainInfo->scissor =
        vk::Rect2D{ .offset = { .x = 0, .y = 0 }, .extent = framebufferExtent };

    vk::SurfaceTransformFlagBitsKHR imageTransform =
        surfaceCapabilities.currentTransform;

    vk::SharingMode sharingMode = {};
    std::vector<uint32_t> queueFamilyIndicies = {};
    if (Swapchain_Info.graphicsQueueIndex == Swapchain_Info.presentQueueIndex) {
        sharingMode = vk::SharingMode::eExclusive;
        queueFamilyIndicies = {};
    } else {
        sharingMode = vk::SharingMode::eConcurrent;
        queueFamilyIndicies = { Swapchain_Info.graphicsQueueIndex,
                                Swapchain_Info.presentQueueIndex };
    }
    vk::SwapchainCreateInfoKHR swapchainCreateInfo = {
        .surface = Swapchain_Info.surface,
        .minImageCount = surfaceCapabilities.minImageCount,
        .imageFormat = this->surfaceFormat.format,
        .imageColorSpace = this->surfaceFormat.colorSpace,
        .imageExtent = framebufferExtent,
        .imageArrayLayers = 1,
        .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
        .imageSharingMode = sharingMode,
        .queueFamilyIndexCount =
            static_cast<uint32_t>(queueFamilyIndicies.size()),
        .pQueueFamilyIndices = queueFamilyIndicies.data(),
        .preTransform = imageTransform,
        .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
        .presentMode = this->presentMode,
        .clipped = VK_TRUE,
        .oldSwapchain = nullptr
    };
    swapchainInfo->handle =
        this->handle->createSwapchainKHRUnique(swapchainCreateInfo);

    // Get handles to swapchain images.
    swapchainInfo->swapchainImages =
        this->handle->getSwapchainImagesKHR(swapchainInfo->handle.get());

    // Get handles to swapchain image views.
    swapchainInfo->swapchainImageViews
        .clear(); // Deallocate existing image views.
    for (const auto& swapchainImage : swapchainInfo->swapchainImages) {
        vk::ImageViewCreateInfo imageViewCreateInfo = {
            .image = swapchainImage,
            .viewType = vk::ImageViewType::e2D,
            .format = this->surfaceFormat.format,
            .components = { vk::ComponentSwizzle::eIdentity,
                            vk::ComponentSwizzle::eIdentity,
                            vk::ComponentSwizzle::eIdentity,
                            vk::ComponentSwizzle::eIdentity },
            .subresourceRange = { .aspectMask = vk::ImageAspectFlagBits::eColor,
                                  .baseMipLevel = 0,
                                  .levelCount = 1,
                                  .baseArrayLayer = 0,
                                  .layerCount = 1 }
        };
        swapchainInfo->swapchainImageViews.emplace_back(
            this->handle->createImageViewUnique(imageViewCreateInfo));
    }

    // Bind the framebuffers to the swapchain image views.
    swapchainInfo->swapchainFramebuffers
        .clear(); // Deallocate existing image views.
    swapchainInfo->swapchainFramebuffers.resize(
        swapchainInfo->swapchainImageViews.size());
    for (size_t i = 0; i < swapchainInfo->swapchainFramebuffers.size(); ++i) {
        std::array<vk::ImageView, 1> attachments{
            swapchainInfo->swapchainImageViews.at(i).get()
        };
        vk::FramebufferCreateInfo framebufferCreateInfo = {
            .renderPass = Swapchain_Info.renderPass,
            .attachmentCount = static_cast<uint32_t>(attachments.size()),
            .pAttachments = attachments.data(),
            .width = framebufferExtent.width,
            .height = framebufferExtent.height,
            .layers = 1
        };
        swapchainInfo->swapchainFramebuffers.at(i) =
            this->handle->createFramebufferUnique(framebufferCreateInfo);
    }

    return swapchainInfo;
}

pipeline_ptr device::CreatePipeline(const pipeline_info& Pipeline_Info)
{
    // Pipeline dynamic states (selects what is configurable after pipeline
    // creation).
    vk::PipelineDynamicStateCreateInfo dynamicState = {
        .dynamicStateCount =
            static_cast<uint32_t>(Pipeline_Info.dynamicStates.size()),
        .pDynamicStates = Pipeline_Info.dynamicStates.data()
    };

    // Binds vertex buffers to the vertex shader.
    vk::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {
        .vertexBindingDescriptionCount = static_cast<uint32_t>(
            Pipeline_Info.shaders.vertex->bindingDescriptions.size()),
        .pVertexBindingDescriptions =
            Pipeline_Info.shaders.vertex->bindingDescriptions.data(),
        .vertexAttributeDescriptionCount = static_cast<uint32_t>(
            Pipeline_Info.shaders.vertex->attributeDescriptions.size()),
        .pVertexAttributeDescriptions =
            Pipeline_Info.shaders.vertex->attributeDescriptions.data()
    };

    // Defines vertex assembly behavior (currently configured to construct
    // triangles without reusing vertices).
    vk::PipelineInputAssemblyStateCreateInfo vertexInputAssemblyCreateInfo = {
        .topology = vk::PrimitiveTopology::eTriangleList,
        .primitiveRestartEnable = VK_FALSE
    };

    // Define the viewport and scissor count. The viewport and scissor are
    // dynamic, so they will be passed to vulkan later, but if they were
    // static they'd be passed to vulkan here.
    vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo = {
        .viewportCount = 1, .scissorCount = 1
    };

    // Define the behavior of the rasterization pipeline stage.
    vk::PipelineRasterizationStateCreateInfo
        pipelineRasterizationStateCreateInfo = {
            .depthClampEnable = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode = vk::PolygonMode::eFill,
            .cullMode = vk::CullModeFlagBits::eBack,
            .frontFace = vk::FrontFace::eClockwise,
            .depthBiasEnable = VK_FALSE,
            .depthBiasConstantFactor = 0.0F,
            .depthBiasClamp = 0.0F,
            .depthBiasSlopeFactor = 0.0F,
            .lineWidth = 1.0F
        };

    // Define multisampling behavior (disabled here).
    vk::PipelineMultisampleStateCreateInfo
        pipelineMultisampleStateCreateInfo = { .rasterizationSamples =
                                                   vk::SampleCountFlagBits::e1,
                                               .sampleShadingEnable = VK_FALSE,
                                               .minSampleShading = 1.0F,
                                               .pSampleMask = nullptr,
                                               .alphaToCoverageEnable =
                                                   VK_FALSE,
                                               .alphaToOneEnable = VK_FALSE };

    // Color blending for the attached framebuffer. If blendEnable is
    // VK_FALSE, the rest of this struct (except for colorWriteMask) is
    // ignored and color blending is disabled.
    vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState = {
        .blendEnable = VK_FALSE,
        .srcColorBlendFactor = vk::BlendFactor::eOne,
        .dstColorBlendFactor = vk::BlendFactor::eZero,
        .colorBlendOp = vk::BlendOp::eAdd,
        .srcAlphaBlendFactor = vk::BlendFactor::eOne,
        .dstAlphaBlendFactor = vk::BlendFactor::eZero,
        .alphaBlendOp = vk::BlendOp::eAdd,
        .colorWriteMask =
            vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
            vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
    };

    // Global color blending.
    vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo = {
        .logicOpEnable = VK_FALSE,
        .logicOp = vk::LogicOp::eCopy,
        .attachmentCount = 1,
        .pAttachments = &pipelineColorBlendAttachmentState,
        .blendConstants =
            vk::ArrayWrapper1D<float, 4>({ 0.0F, 0.0F, 0.0F, 0.0F })
    };

    pipeline_ptr pipeline =
        std::make_shared<internal::pipeline_public_constructor>();

    // Pipeline layout creation.
    vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
        .setLayoutCount = 0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr
    };
    pipeline->layout =
        this->handle->createPipelineLayoutUnique(pipelineLayoutCreateInfo);

    std::vector<vk::PipelineShaderStageCreateInfo>
        pipelineShaderStageCreateInfos =
            Pipeline_Info.shaders.StageCreationInfos();

    // Create the graphics pipeline.
    vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {
        .stageCount =
            static_cast<uint32_t>(pipelineShaderStageCreateInfos.size()),
        .pStages = pipelineShaderStageCreateInfos.data(),
        .pVertexInputState = &vertexInputStateCreateInfo,
        .pInputAssemblyState = &vertexInputAssemblyCreateInfo,
        .pViewportState = &pipelineViewportStateCreateInfo,
        .pRasterizationState = &pipelineRasterizationStateCreateInfo,
        .pMultisampleState = &pipelineMultisampleStateCreateInfo,
        .pDepthStencilState = nullptr, // optional
        .pColorBlendState = &pipelineColorBlendStateCreateInfo,
        .pDynamicState = &dynamicState,
        .layout = pipeline->layout.get(),
        .renderPass = Pipeline_Info.renderPass,
        .subpass = 0,
        .basePipelineHandle = VK_NULL_HANDLE, // optional
        .basePipelineIndex = -1,              // optional
    };
    pipeline->handle = this->handle
                           ->createGraphicsPipelineUnique(
                               VK_NULL_HANDLE, graphicsPipelineCreateInfo)
                           .value;
    // The vertex shader and fragment shader modules may now destroyed.

    return pipeline;
}

} // namespace gvw