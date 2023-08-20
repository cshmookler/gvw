// Standard includes
#include <fstream>

// External includes
#include <boost/dll/runtime_symbol_info.hpp>

// Local includes
#include "device.hpp"

std::vector<gvw::device_selection::returns>
gvw::device_selection::MinimumForPresentation( // NOLINT
    const std::vector<parameters>& Physical_Device_Infos,
    const std::optional<vk::SurfaceKHR>& Window_Surface)
{
    vk::PhysicalDevice selectedPhysicalDevice;
    vk::SurfaceFormatKHR selectedSurfaceFormat;
    vk::PresentModeKHR selectedPresentMode = {};
    std::vector<queue_family_info> selectedQueueFamilyInfos(1);

    int selectedPhysicalDeviceScore = -1;
    int currentPhysicalDeviceScore = 0;
    for (const auto& physicalDeviceInfo : Physical_Device_Infos) {
        const auto& physicalDevice = physicalDeviceInfo.physicalDevice;
        const auto& surfaceFormats = physicalDeviceInfo.surfaceFormats;
        const auto& presentModes = physicalDeviceInfo.presentModes;

        // Score different types of devices based on assumed performance.
        vk::PhysicalDeviceProperties physicalDeviceProperties =
            physicalDevice.getProperties();
        switch (physicalDeviceProperties.deviceType) {
            case vk::PhysicalDeviceType::eDiscreteGpu:
                currentPhysicalDeviceScore = 4;
                break;
            case vk::PhysicalDeviceType::eIntegratedGpu:
                currentPhysicalDeviceScore = 3;
                break;
            case vk::PhysicalDeviceType::eVirtualGpu:
                currentPhysicalDeviceScore = 2;
                break;
            case vk::PhysicalDeviceType::eOther:
                currentPhysicalDeviceScore = 1;
                break;
            case vk::PhysicalDeviceType::eCpu:
                currentPhysicalDeviceScore = 0;
                break;
        }

        // Select the first available surface format.
        vk::SurfaceFormatKHR selectedPhysicalDeviceSurfaceFormat = {};
        if (surfaceFormats.has_value()) {
            selectedPhysicalDeviceSurfaceFormat = surfaceFormats.value()->at(0);
        }

        vk::PresentModeKHR selectedPhysicalDevicePresentMode = {};
        if (presentModes.has_value()) {
            int selectedPresentModeScore = -1;
            vk::PresentModeKHR currentPhysicalDevicePresentMode = {};
            int currentPresentModeScore = 0;
            for (const auto& presentMode : presentModes.value().Get()) {
                switch (presentMode) {
                    case vk::PresentModeKHR::eMailbox:
                        currentPresentModeScore = 5; // NOLINT
                        currentPhysicalDevicePresentMode =
                            vk::PresentModeKHR::eMailbox;
                        break;
                    case vk::PresentModeKHR::eFifo:
                        currentPresentModeScore = 4;
                        currentPhysicalDevicePresentMode =
                            vk::PresentModeKHR::eFifo;
                        break;
                    case vk::PresentModeKHR::eFifoRelaxed:
                        currentPresentModeScore = 3;
                        currentPhysicalDevicePresentMode =
                            vk::PresentModeKHR::eFifoRelaxed;
                        break;
                    case vk::PresentModeKHR::eImmediate:
                        currentPresentModeScore = 2;
                        currentPhysicalDevicePresentMode =
                            vk::PresentModeKHR::eImmediate;
                        break;
                    case vk::PresentModeKHR::eSharedContinuousRefresh:
                        currentPresentModeScore = 1;
                        currentPhysicalDevicePresentMode =
                            vk::PresentModeKHR::eSharedContinuousRefresh;
                        break;
                    case vk::PresentModeKHR::eSharedDemandRefresh:
                        currentPresentModeScore = 0;
                        currentPhysicalDevicePresentMode =
                            vk::PresentModeKHR::eSharedDemandRefresh;
                        break;
                }

                if (currentPresentModeScore > selectedPresentModeScore) {
                    selectedPresentModeScore = currentPresentModeScore;
                    selectedPhysicalDevicePresentMode =
                        currentPhysicalDevicePresentMode;
                }
            }
        }

        std::vector<vk::QueueFamilyProperties> queueFamilyProperties =
            physicalDevice.getQueueFamilyProperties();
        if (queueFamilyProperties.empty()) {
            continue;
        }

        // Find queue families that supports both graphics and presentation.
        std::optional<uint32_t> viableGraphicsQueueFamilyIndex;
        std::optional<uint32_t> viablePresentationQueueFamilyIndex;
        for (size_t i = 0; i < queueFamilyProperties.size(); ++i) {
            vk::QueueFlags queueFlags = queueFamilyProperties.at(i).queueFlags;
            if (viableGraphicsQueueFamilyIndex.has_value() == false) {
                if (bool(queueFlags & vk::QueueFlagBits::eGraphics)) {
                    viableGraphicsQueueFamilyIndex = i;
                }
            }
            if (Window_Surface.has_value() &&
                (viablePresentationQueueFamilyIndex.has_value() == false)) {
                if (physicalDevice.getSurfaceSupportKHR(
                        i, Window_Surface.value()) != VK_FALSE) {
                    viablePresentationQueueFamilyIndex = i;
                }
            }

            if (viableGraphicsQueueFamilyIndex.has_value() &&
                viablePresentationQueueFamilyIndex.has_value()) {
                if (viableGraphicsQueueFamilyIndex.value() ==
                    viablePresentationQueueFamilyIndex.value()) {
                    break;
                }
            }
        }
        if ((viableGraphicsQueueFamilyIndex.has_value() == false) ||
            (viablePresentationQueueFamilyIndex.has_value() == false)) {
            continue;
        }

        if (currentPhysicalDeviceScore > selectedPhysicalDeviceScore) {
            selectedPhysicalDeviceScore = currentPhysicalDeviceScore;
            selectedPhysicalDevice = physicalDevice;
            selectedSurfaceFormat = selectedPhysicalDeviceSurfaceFormat;
            selectedPresentMode = selectedPhysicalDevicePresentMode;

            if (viableGraphicsQueueFamilyIndex.value() ==
                viablePresentationQueueFamilyIndex.value()) {
                selectedQueueFamilyInfos.resize(1);
            }

            selectedQueueFamilyInfos.at(0) = {
                .createInfo = { .queueFamilyIndex =
                                    viableGraphicsQueueFamilyIndex.value(),
                                .queueCount = 1,
                                .pQueuePriorities =
                                    &queue_priority::HIGH.Get() },
                .properties = queueFamilyProperties.at(
                    viableGraphicsQueueFamilyIndex.value())
            };

            if (viableGraphicsQueueFamilyIndex !=
                viablePresentationQueueFamilyIndex) {
                selectedQueueFamilyInfos.resize(2);
                selectedQueueFamilyInfos.at(1) = {
                    .createInfo = { .queueFamilyIndex =
                                        viablePresentationQueueFamilyIndex
                                            .value(),
                                    .queueCount = 1,
                                    .pQueuePriorities =
                                        &queue_priority::HIGH.Get() },
                    .properties = queueFamilyProperties.at(
                        viablePresentationQueueFamilyIndex.value())
                };
            }
        }
    }

    return { { selectedPhysicalDevice,
               selectedSurfaceFormat,
               selectedPresentMode,
               selectedQueueFamilyInfos } };
}

gvw::device::device(
    std::shared_ptr<vk::UniqueDevice> Logical_Device,
    const vk::PhysicalDevice& Physical_Device,
    const vk::SurfaceFormatKHR& Surface_Format,
    const vk::PresentModeKHR& Present_Mode,
    const std::vector<device_selection::queue_family_info>& Queue_Infos)
    : logicalDevice(std::move(Logical_Device))
    , physicalDevice(Physical_Device)
    , surfaceFormat(Surface_Format)
    , presentMode(Present_Mode)
    , queueInfos(Queue_Infos)
{
}

std::vector<gvw::shader> gvw::device::LoadShadersFromSpirVFiles(
    const std::vector<shader_info>& Shader_Infos)
{
    boost::filesystem::path executableDirectory =
        boost::dll::program_location().parent_path();

    std::vector<shader> shaders;

    for (const auto& shaderInfo : Shader_Infos) {
        boost::filesystem::path absolutePath =
            boost::filesystem::path(shaderInfo.code).is_absolute()
                ? shaderInfo.code
                : (executableDirectory / shaderInfo.code);

        auto charBuffer = ReadFile<std::vector<char>>(absolutePath.c_str());

        vk::ShaderModuleCreateInfo shaderModuleCreateInfo = {
            .codeSize = charBuffer.size(),
            // NOLINTNEXTLINE
            .pCode = reinterpret_cast<const uint32_t*>(charBuffer.data())
        };

        std::shared_ptr<vk::UniqueShaderModule> shaderModule =
            std::make_shared<vk::UniqueShaderModule>(
                this->logicalDevice->get().createShaderModuleUnique(
                    shaderModuleCreateInfo));

        vk::PipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {
            .stage = shaderInfo.stage,
            .module = shaderModule->get(),
            .pName = shaderInfo.entryPoint,
        };

        shaders.emplace_back(shaderModule, pipelineShaderStageCreateInfo);
    }

    return shaders;
}

gvw::buffer gvw::device::CreateBuffer(const buffer_info& Buffer_Info)
{
    buffer buffer;
    buffer.size = Buffer_Info.sizeInBytes;

    vk::BufferCreateInfo bufferCreateInfo = {
        .size = buffer.size,
        .usage = Buffer_Info.usage,
        // Sharing is exclusive when only one queue has access to the buffer.
        .sharingMode = vk::SharingMode::eExclusive
    };
    buffer.buffer =
        this->logicalDevice->get().createBufferUnique(bufferCreateInfo);

    vk::MemoryRequirements memoryRequirements =
        this->logicalDevice->get().getBufferMemoryRequirements(
            buffer.buffer.get());

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
        gvwErrorCallback(
            "Failed to find a viable memory type for a Vulkan buffer.");
    }

    vk::MemoryAllocateInfo memoryAllocateInfo = { .allocationSize =
                                                      memoryRequirements.size,
                                                  .memoryTypeIndex =
                                                      memoryTypeIndex.value() };
    buffer.memory =
        this->logicalDevice->get().allocateMemoryUnique(memoryAllocateInfo);

    this->logicalDevice->get().bindBufferMemory(
        buffer.buffer.get(), buffer.memory.get(), 0);

    return buffer;
}

vk::UniqueRenderPass gvw::device::CreateRenderPass(
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

    return this->logicalDevice->get().createRenderPassUnique(
        renderPassCreateInfo);
}

gvw::swapchain_ptr gvw::device::CreateSwapchain(
    const swapchain_info& Swapchain_Info)
{
    vk::SurfaceCapabilitiesKHR surfaceCapabilities =
        this->physicalDevice.getSurfaceCapabilitiesKHR(Swapchain_Info.surface);
    vk::Extent2D framebufferExtent = {
        .width = std::clamp(
            static_cast<uint32_t>(Swapchain_Info.framebufferSize->width),
            surfaceCapabilities.minImageExtent.width,
            surfaceCapabilities.maxImageExtent.width),
        .height = std::clamp(
            static_cast<uint32_t>(Swapchain_Info.framebufferSize->height),
            surfaceCapabilities.minImageExtent.height,
            surfaceCapabilities.maxImageExtent.height)
    };

    swapchain_ptr swapchainInfo;
    if (Swapchain_Info.oldSwapchain == nullptr) {
        swapchainInfo = std::make_shared<swapchain>();
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
    swapchainInfo->swapchain =
        this->logicalDevice->get().createSwapchainKHRUnique(
            swapchainCreateInfo);

    // Get handles to swapchain images.
    swapchainInfo->swapchainImages =
        this->logicalDevice->get().getSwapchainImagesKHR(
            swapchainInfo->swapchain.get());

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
            this->logicalDevice->get().createImageViewUnique(
                imageViewCreateInfo));
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
            this->logicalDevice->get().createFramebufferUnique(
                framebufferCreateInfo);
    }

    return swapchainInfo;
}

gvw::pipeline_ptr gvw::device::CreatePipeline(
    const pipeline_info& Pipeline_Info)
{
    // Pipeline dynamic states (selects what is configurable after pipeline
    // creation).
    vk::PipelineDynamicStateCreateInfo dynamicState = {
        .dynamicStateCount =
            static_cast<uint32_t>(Pipeline_Info.dynamicStates->size()),
        .pDynamicStates = Pipeline_Info.dynamicStates->data()
    };

    // Binds vertex buffers to the vertex shader.
    vk::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {
        .vertexBindingDescriptionCount = static_cast<uint32_t>(
            Pipeline_Info.vertexInputBindingDescriptions.size()),
        .pVertexBindingDescriptions =
            Pipeline_Info.vertexInputBindingDescriptions.data(),
        .vertexAttributeDescriptionCount = static_cast<uint32_t>(
            Pipeline_Info.vertexInputAttributeDescriptions.size()),
        .pVertexAttributeDescriptions =
            Pipeline_Info.vertexInputAttributeDescriptions.data()
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

    pipeline_ptr pipeline = std::make_shared<gvw::pipeline>();

    // Pipeline layout creation.
    vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
        .setLayoutCount = 0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr
    };
    pipeline->layout = this->logicalDevice->get().createPipelineLayoutUnique(
        pipelineLayoutCreateInfo);

    std::vector<vk::PipelineShaderStageCreateInfo>
        pipelineShaderStageCreateInfos;
    pipelineShaderStageCreateInfos.reserve(Pipeline_Info.shaders.size());
    std::transform(Pipeline_Info.shaders.begin(),
                   Pipeline_Info.shaders.end(),
                   std::back_inserter(pipelineShaderStageCreateInfos),
                   [](const shader& Shader) {
                       return Shader.pipelineShaderStageCreateInfo;
                   });

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
    pipeline->pipeline = this->logicalDevice->get()
                             .createGraphicsPipelineUnique(
                                 VK_NULL_HANDLE, graphicsPipelineCreateInfo)
                             .value;
    // The vertex shader and fragment shader modules may now destroyed.

    return pipeline;
}