// Standard includes
#include <iostream>

// Local includes
#include "window.hpp"

namespace gvw {

// NOLINTNEXTLINE
void window::DestroyGlfwWindow(GLFWwindow* Window_Handle) noexcept
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwDestroyWindow(Window_Handle);
}

window::window(const window_info& Window_Info, window* Parent_Window)
    : gvwInstance(internal::global::GVW_INSTANCE)
{
    internal::AssertInitialization();

    GLFWmonitor* fullScreenMonitor =
        (Window_Info.fullScreenMonitor != nullptr)
            ? Window_Info.fullScreenMonitor->GetHandle()
            : nullptr;

    {
        // Lock the GLFW mutex between the application of window hints and
        // window creation to prevent other threads from setting window hints.
        std::scoped_lock lock(internal::global::GLFW_MUTEX);

        // Apply window creation hints.
        Window_Info.creationHints.Apply();

        // Briefly hide the window if an initial position was specified. If this
        // is not done the window may be in the wrong position for one frame
        // (the default position set by the operating system).
        /// @todo Remove this when glfw 3.4.x is released.
        if (Window_Info.position.has_value()) {
            glfwWindowHint(Window_Info.creationHints.visible.HINT, GLFW_FALSE);
        }

        // Create the window.
        this->windowHandle =
            glfwCreateWindow(Window_Info.size.width,
                             Window_Info.size.height,
                             Window_Info.title,
                             fullScreenMonitor,
                             nullptr); // "We use Vulkan in this household!"
    }

    this->glfwWindowDestroyer =
        std::make_unique<internal::terminator<GLFWwindow*>>(DestroyGlfwWindow,
                                                            this->windowHandle);

    this->SetUserPointer(this);

    // Set an initial position if one was specified.
    if (Window_Info.position.has_value()) {
        this->SetPosition(Window_Info.position.value());
        // Show the window now that it has been moved to the correct position.
        this->Show();
    }

    {
        // Set the reset position and size of the window.
        std::scoped_lock lock(internal::global::GLFW_MUTEX, this->resetMutex);
        this->resetPosition = this->GetPositionNoMutex();
        this->resetSize = this->GetSizeNoMutex();
    }

    // Set window event callbacks.
    this->SetEventCallbacks(Window_Info.eventCallbacks);

    // Create window surface.
    /// @todo Place this into its own function.
    VkSurfaceKHR tempSurface = nullptr;
    {
        std::scoped_lock lock(internal::global::GLFW_MUTEX);
        if (glfwCreateWindowSurface(*this->gvwInstance->vulkanInstance,
                                    this->windowHandle,
                                    nullptr,
                                    &tempSurface) != VK_SUCCESS) {
            ErrorCallback("Window surface creation failed");
        }
    }
    this->surface =
        vk::UniqueSurfaceKHR(tempSurface, *this->gvwInstance->vulkanInstance);

    // Use an already existing logical device or create a new one.
    if (Window_Info.device != nullptr) {
        this->logicalDevice = Window_Info.device;
    } else if (Parent_Window != nullptr) {
        this->logicalDevice = Parent_Window->logicalDevice;
    } else {
        this->logicalDevice =
            gvwInstance
                ->SelectPhysicalDevices(Window_Info.deviceSelectionInfo,
                                        &this->surface.get())
                .at(0);
    }

    // Find indicies for the graphics and present queue families.
    /// @todo Place physical device minimum requirements verification into its
    /// own function.
    std::optional<uint32_t> viableGraphicsQueueFamilyIndex;
    std::optional<uint32_t> viablePresentationQueueFamilyIndex;
    const auto& queueInfos = this->logicalDevice->GetQueueFamilyInfos();
    for (const auto& queueInfo : queueInfos) {
        if (viableGraphicsQueueFamilyIndex.has_value() == false) {
            if (bool(queueInfo.properties.queueFlags &
                     vk::QueueFlagBits::eGraphics)) {
                viableGraphicsQueueFamilyIndex =
                    queueInfo.createInfo.queueFamilyIndex;
            }
        }
        if (viablePresentationQueueFamilyIndex.has_value() == false) {
            if (this->logicalDevice->GetPhysicalDevice().getSurfaceSupportKHR(
                    queueInfo.createInfo.queueFamilyIndex,
                    this->surface.get()) != VK_FALSE) {
                viablePresentationQueueFamilyIndex =
                    queueInfo.createInfo.queueFamilyIndex;
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
    if (viableGraphicsQueueFamilyIndex.has_value() == false) {
        ErrorCallback("The selected physical device does not offer a queue "
                      "family that supports graphics.");
    }
    if (viablePresentationQueueFamilyIndex.has_value() == false) {
        ErrorCallback("The selected physical device does not offer a queue "
                      "family that supports presentation.");
    }
    this->graphicsQueueIndex = viableGraphicsQueueFamilyIndex.value();
    this->presentQueueIndex = viablePresentationQueueFamilyIndex.value();

    /// @todo Allow selection of specific queue indicies.
    this->graphicsQueue =
        this->logicalDevice->GetHandle().getQueue(this->graphicsQueueIndex, 0);
    this->presentQueue =
        this->logicalDevice->GetHandle().getQueue(this->presentQueueIndex, 0);

    // Use an already existing render pass or create a new one.
    if (Window_Info.renderPass != nullptr) {
        if (Window_Info.renderPass->handle.getOwner() !=
            this->logicalDevice->GetHandle()) {
            ErrorCallback("Cannot use a render pass created with a different "
                          "logical device.");
        }
        this->renderPass = Window_Info.renderPass;
    } else {
        this->renderPass = this->logicalDevice->CreateRenderPass(
            { .format = this->logicalDevice->GetSurfaceFormat().format });
    }

    // Create swapchain.
    this->CreateSwapchain();

    /// @todo Place shader utilities into separate functions or within the
    /// shader class.
    if (Window_Info.shaders.vertex != nullptr) {
        if (this->shaders.vertex->handle.getOwner() !=
            this->logicalDevice->GetHandle()) {
            ErrorCallback("Cannot use a vertex shader created with a different "
                          "logical device.");
        }
        this->shaders.vertex = Window_Info.shaders.vertex;
    } else {
        // Attempt to load the default vertex shader.
        vertex_shader_info vertexShaderInfo = {
            .general = { .code = "vert.spv",
                         .stage = vk::ShaderStageFlagBits::eVertex },
            .bindingDescriptions = { { .binding = 0,
                                       .stride = sizeof(xy_rgb),
                                       .inputRate =
                                           vk::VertexInputRate::eVertex } },
            .attributeDescriptions = { { { .location = 0,
                                           .binding = 0,
                                           .format = vk::Format::eR32G32Sfloat,
                                           .offset = offsetof(xy_rgb, first) },
                                         { .location = 1,
                                           .binding = 0,
                                           .format =
                                               vk::Format::eR32G32B32Sfloat,
                                           .offset =
                                               offsetof(xy_rgb, second) } } }
        };
        this->shaders.vertex =
            this->logicalDevice->LoadVertexShaderFromSpirVFile(
                vertexShaderInfo);
    }

    if (Window_Info.shaders.fragment != nullptr) {
        if (this->shaders.fragment->handle.getOwner() !=
            this->logicalDevice->GetHandle()) {
            ErrorCallback("Cannot use a fragment shader created with a "
                          "different logical device.");
        }
        this->shaders.fragment = Window_Info.shaders.fragment;
    } else {
        // Attempt to load the default fragment shader.
        fragment_shader_info fragmentShaderInfo = {
            .general = { .code = "frag.spv",
                         .stage = vk::ShaderStageFlagBits::eFragment }
        };
        this->shaders.fragment =
            this->logicalDevice->LoadFragmentShaderFromSpirVFile(
                fragmentShaderInfo);
    }

    // Use an already existing pipeline or create a new one.
    if (Window_Info.pipeline != nullptr) {
        if (Window_Info.pipeline->handle.getOwner() !=
            this->logicalDevice->GetHandle()) {
            ErrorCallback("Cannot use a pipeline created with a different "
                          "logical device.");
        }
        this->pipeline = Window_Info.pipeline;
    } else {
        this->CreatePipeline(
            pipeline_dynamic_states_config::VIEWPORT_AND_SCISSOR);
    }

    // Create the command pool.
    vk::CommandPoolCreateInfo commandPoolCreateInfo = {
        .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
        // Graphics queues can also perform transfer operations.
        .queueFamilyIndex = graphicsQueueIndex
    };
    this->commandPool =
        this->logicalDevice->GetHandle().createCommandPoolUnique(
            commandPoolCreateInfo);

    // Allocate command buffers from the command pool.
    vk::CommandBufferAllocateInfo stagingCommandBufferAllocateInfo = {
        .commandPool = commandPool.get(),
        .level = vk::CommandBufferLevel::ePrimary,
        .commandBufferCount = 1
    };
    this->stagingCommandBuffer = std::move(
        this->logicalDevice->GetHandle()
            .allocateCommandBuffersUnique(stagingCommandBufferAllocateInfo)
            .at(0));

    // Create staging vertex buffer for static vertices.
    buffer_ptr tempVertexStagingBuffer = this->logicalDevice->CreateBuffer(
        { .sizeInBytes = (sizeof(xy_rgb) * Window_Info.staticVertices.size()),
          .usage = vk::BufferUsageFlagBits::eTransferSrc,
          .memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible |
                              vk::MemoryPropertyFlagBits::eHostCoherent });

    // Create device local buffer for static and dynamic data vertices.
    this->staticVertexBuffer = this->logicalDevice->CreateBuffer(
        { .sizeInBytes = tempVertexStagingBuffer->size +
                         Window_Info.sizeOfDynamicDataVerticesInBytes,
          .usage = vk::BufferUsageFlagBits::eTransferDst |
                   vk::BufferUsageFlagBits::eVertexBuffer,
          .memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal });

    // Map static vertices to the static vertex buffer.
    void* tempVertexStagingBufferPointer =
        this->logicalDevice->GetHandle().mapMemory(
            tempVertexStagingBuffer->memory.get(),
            0,
            tempVertexStagingBuffer->size,
            {});
    memcpy(tempVertexStagingBufferPointer,
           Window_Info.staticVertices.data(),
           static_cast<size_t>(tempVertexStagingBuffer->size));
    this->logicalDevice->GetHandle().unmapMemory(
        tempVertexStagingBuffer->memory.get());

    // Record command buffer for transferring static vertices.
    this->stagingCommandBuffer->begin(
        { .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit });
    this->stagingCommandBuffer->copyBuffer(
        tempVertexStagingBuffer->handle.get(),
        this->staticVertexBuffer->handle.get(),
        vk::BufferCopy{ .srcOffset = 0,
                        .dstOffset = 0,
                        .size = tempVertexStagingBuffer->size });
    this->stagingCommandBuffer->end();

    // Transfer static vertex buffer data from the staging buffer to the
    // device-local buffer.
    vk::SubmitInfo stagingSubmitInfo = {
        .commandBufferCount = 1,
        .pCommandBuffers = &this->stagingCommandBuffer.get()
    };
    this->graphicsQueue.submit({ stagingSubmitInfo });
    this->graphicsQueue.waitIdle();

    // Create vertex staging buffer.
    this->staticVertexStagingBuffer = this->logicalDevice->CreateBuffer(
        { .sizeInBytes = Window_Info.sizeOfDynamicDataVerticesInBytes,
          .usage = vk::BufferUsageFlagBits::eTransferSrc,
          .memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible |
                              vk::MemoryPropertyFlagBits::eHostCoherent });

    vk::CommandBufferBeginInfo stagingCommandBufferBeginInfo = {};
    this->stagingCommandBuffer->begin(stagingCommandBufferBeginInfo);
    this->stagingCommandBuffer->copyBuffer(
        this->staticVertexStagingBuffer->handle.get(),
        this->staticVertexBuffer->handle.get(),
        { vk::BufferCopy{ .srcOffset = 0,
                          .dstOffset = tempVertexStagingBuffer->size,
                          .size = this->staticVertexStagingBuffer->size } });
    this->stagingCommandBuffer->end();

    vk::CommandBufferAllocateInfo commandBufferAllocateInfo = {
        .commandPool = commandPool.get(),
        .level = vk::CommandBufferLevel::ePrimary,
        .commandBufferCount = this->MAX_FRAMES_IN_FLIGHT
    };
    this->commandBuffers =
        this->logicalDevice->GetHandle().allocateCommandBuffersUnique(
            commandBufferAllocateInfo);

    // Create semaphores and fences to control the execution order in the
    // device and synchronize the host with the device.
    vk::SemaphoreCreateInfo semaphoreCreateInfo = {};
    vk::FenceCreateInfo fenceCreateInfo = {
        .flags = vk::FenceCreateFlagBits::eSignaled
    };

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        nextImageAvailableSemaphores.emplace_back(
            this->logicalDevice->GetHandle().createSemaphoreUnique(
                semaphoreCreateInfo));
        finishedRenderingSemaphores.emplace_back(
            this->logicalDevice->GetHandle().createSemaphoreUnique(
                semaphoreCreateInfo));
        inFlightFences.emplace_back(
            this->logicalDevice->GetHandle().createFenceUnique(
                fenceCreateInfo));
    }

    // Configure semaphore triggering.
    this->waitStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
}

window::~window()
{
    this->logicalDevice->GetHandle().waitIdle();
}

void window::SetUserPointer(void* Pointer)
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetWindowUserPointer(this->windowHandle, Pointer);
}

void window::CreateSwapchain()
{
    this->swapchain = this->logicalDevice->CreateSwapchain(
        { .framebufferSize = this->GetFramebufferSizeNoMutex(),
          .graphicsQueueIndex = this->graphicsQueueIndex,
          .presentQueueIndex = this->presentQueueIndex,
          .surface = this->surface.get(),
          .renderPass = this->renderPass->handle.get(),
          .oldSwapchain = this->swapchain });
}

void window::CreatePipeline(const pipeline_dynamic_states& Dynamic_States)
{
    this->pipeline = this->logicalDevice->CreatePipeline(
        { .shaders = this->shaders,
          .dynamicStates = Dynamic_States,
          .renderPass = this->renderPass->handle.get() });
}

void window::DrawFrame(const std::vector<xy_rgb>& Vertices)
{
    // Wait until the previous frame is done rendering.
    if (logicalDevice->GetHandle().waitForFences(
            this->inFlightFences.at(this->currentFrameIndex).get(),
            VK_TRUE,
            UINT64_MAX) != vk::Result::eSuccess) {
        ErrorCallback("Failed to wait for the previous "
                      "frame to finish rendering.");
    }

    // Get an image from the swapchain to render to.
    vk::ResultValue<uint32_t> imageIndex =
        logicalDevice->GetHandle().acquireNextImageKHR(
            this->swapchain->handle.get(),
            UINT64_MAX,
            nextImageAvailableSemaphores.at(currentFrameIndex).get());

    if (imageIndex.result == vk::Result::eErrorOutOfDateKHR) {
        logicalDevice->GetHandle().waitIdle();
        this->CreateSwapchain();
    } else if (imageIndex.result != vk::Result::eSuccess &&
               imageIndex.result != vk::Result::eSuboptimalKHR) {
        ErrorCallback("Failed to acquire next image from the swapchain.");
    } else {
        logicalDevice->GetHandle().resetFences(
            inFlightFences.at(currentFrameIndex).get());

        // Map vertices to the vertex buffer.
        void* vertexStagingBufferPointer =
            this->logicalDevice->GetHandle().mapMemory(
                this->staticVertexStagingBuffer->memory.get(),
                0,
                this->staticVertexStagingBuffer->size,
                {});
        memcpy(vertexStagingBufferPointer,
               Vertices.data(),
               static_cast<size_t>(this->staticVertexStagingBuffer->size));
        this->logicalDevice->GetHandle().unmapMemory(
            this->staticVertexStagingBuffer->memory.get());

        // Transfer vertex buffer data from the staging buffer to the
        // destination buffer.
        vk::SubmitInfo stagingSubmitInfo = {
            .commandBufferCount = 1,
            .pCommandBuffers = &this->stagingCommandBuffer.get()
        };
        this->graphicsQueue.submit({ stagingSubmitInfo });
        this->graphicsQueue.waitIdle();

        // Use the command buffer to record drawing commands.
        vk::CommandBuffer commandBuffer =
            commandBuffers.at(currentFrameIndex).get();
        commandBuffer.reset();

        vk::CommandBufferBeginInfo commandBufferBeginInfo = {
            .pInheritanceInfo = nullptr // optional
        };
        commandBuffer.begin(commandBufferBeginInfo);

        vk::ClearColorValue clearColor = { 0.0F, 0.0F, 0.0F, 1.0F };
        vk::ClearValue clearValue(clearColor);

        vk::RenderPassBeginInfo renderPassBeginInfo = {
            .renderPass = this->renderPass->handle.get(),
            .framebuffer =
                this->swapchain->swapchainFramebuffers.at(imageIndex.value)
                    .get(),
            .renderArea = { .offset = { 0, 0 },
                            .extent = this->swapchain->scissor.extent },
            .clearValueCount = 1,
            .pClearValues = &clearValue
        };

        // Record the render pass in the command buffer.
        commandBuffer.beginRenderPass(renderPassBeginInfo,
                                      vk::SubpassContents::eInline);
        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics,
                                   this->pipeline->handle.get());
        commandBuffer.setViewport(0, this->swapchain->viewport);
        commandBuffer.setScissor(0, this->swapchain->scissor);
        commandBuffer.bindVertexBuffers(
            0, { this->staticVertexBuffer->handle.get() }, { 0 });
        commandBuffer.draw(
            static_cast<uint32_t>(this->staticVertexBuffer->size), 1, 0, 0);
        commandBuffer.endRenderPass();

        commandBuffer.end();

        vk::SubmitInfo submitInfo = {
            .waitSemaphoreCount = 1,
            .pWaitSemaphores =
                &nextImageAvailableSemaphores.at(currentFrameIndex).get(),
            .pWaitDstStageMask = waitStages.data(),
            .commandBufferCount = 1,
            .pCommandBuffers = &commandBuffer,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores =
                &finishedRenderingSemaphores.at(currentFrameIndex).get()
        };

        // Submit the command buffer to the graphics queue.
        graphicsQueue.submit(submitInfo,
                             inFlightFences.at(currentFrameIndex).get());

        // Configure presentation.
        vk::PresentInfoKHR presentInfo = {
            .waitSemaphoreCount = 1,
            .pWaitSemaphores =
                &finishedRenderingSemaphores.at(currentFrameIndex).get(),
            .swapchainCount = 1,
            .pSwapchains = &this->swapchain->handle.get(),
            .pImageIndices = &imageIndex.value,
            .pResults = nullptr // optional
        };

        // Presents the rendered image to the swapchain which is then
        // displayed on the window surface.
        vk::Result presentResult = presentQueue.presentKHR(&presentInfo);
        if (presentResult == vk::Result::eErrorOutOfDateKHR ||
            presentResult == vk::Result::eSuboptimalKHR) {
            logicalDevice->GetHandle().waitIdle();
            this->CreateSwapchain();
        } else if (presentResult != vk::Result::eSuccess) {
            ErrorCallback("Presentation failed.");
        }

        currentFrameIndex = (currentFrameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
    }
}

int window::GetWindowAttribute(int Attribute)
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    return glfwGetWindowAttrib(this->windowHandle, Attribute);
}

void window::SetWindowAttribute(int Attribute, int Value)
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetWindowAttrib(this->windowHandle, Attribute, Value);
}

void window::SetInputMode(int Mode, int Value)
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetInputMode(this->windowHandle, Mode, Value);
}

int window::GetInputMode(int Mode)
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    return glfwGetInputMode(this->windowHandle, Mode);
}

void window::SetEventCallbacksNoMutex(
    const window_event_callbacks& Window_Event_Callbacks) const
{
    glfwSetKeyCallback(this->windowHandle, Window_Event_Callbacks.keyCallback);
    glfwSetCharCallback(this->windowHandle,
                        Window_Event_Callbacks.characterCallback);
    glfwSetCursorPosCallback(this->windowHandle,
                             Window_Event_Callbacks.cursorPositionCallback);
    glfwSetCursorEnterCallback(this->windowHandle,
                               Window_Event_Callbacks.cursorEnterCallback);
    glfwSetMouseButtonCallback(this->windowHandle,
                               Window_Event_Callbacks.mouseButtonCallback);
    glfwSetScrollCallback(this->windowHandle,
                          Window_Event_Callbacks.scrollCallback);
    glfwSetDropCallback(this->windowHandle,
                        Window_Event_Callbacks.fileDropCallback);
    glfwSetWindowCloseCallback(this->windowHandle,
                               Window_Event_Callbacks.closeCallback);
    glfwSetWindowSizeCallback(this->windowHandle,
                              Window_Event_Callbacks.sizeCallback);
    glfwSetFramebufferSizeCallback(
        this->windowHandle, Window_Event_Callbacks.framebufferSizeCallback);
    glfwSetWindowContentScaleCallback(
        this->windowHandle, Window_Event_Callbacks.contentScaleCallback);
    glfwSetWindowPosCallback(this->windowHandle,
                             Window_Event_Callbacks.positionCallback);
    glfwSetWindowIconifyCallback(this->windowHandle,
                                 Window_Event_Callbacks.iconifyCallback);
    glfwSetWindowMaximizeCallback(this->windowHandle,
                                  Window_Event_Callbacks.maximizeCallback);
    glfwSetWindowFocusCallback(this->windowHandle,
                               Window_Event_Callbacks.focusCallback);
    glfwSetWindowRefreshCallback(this->windowHandle,
                                 Window_Event_Callbacks.refreshCallback);
}

area<int> window::GetSizeNoMutex() const
{
    area<int> size = { 0, 0 };
    glfwGetWindowSize(this->windowHandle, &size.width, &size.height);
    return size;
}

void window::SetSizeNoMutex(const area<int>& Size) const
{
    glfwSetWindowSize(this->windowHandle, Size.width, Size.height);
}

area<int> window::GetFramebufferSizeNoMutex() const
{
    area<int> size = { 0, 0 };
    glfwGetFramebufferSize(this->windowHandle, &size.width, &size.height);
    return size;
}

gvw::coordinate<int> window::GetPositionNoMutex() const
{
    coordinate<int> position = { 0, 0 };
    glfwGetWindowPos(this->windowHandle, &position.x, &position.y);
    return position;
}

void window::SetPositionNoMutex(const coordinate<int>& Position) const
{
    glfwSetWindowPos(this->windowHandle, Position.x, Position.y);
}

void window::HideNoMutex() const
{
    glfwHideWindow(this->windowHandle);
}

void window::ShowNoMutex() const
{
    glfwShowWindow(this->windowHandle);
}

void window::SetEventCallbacks(const window_event_callbacks& Event_Callbacks)
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    this->SetEventCallbacksNoMutex(Event_Callbacks);
}

window_ptr window::CreateChildWindow(const window_info& Window_Info)
{
    return std::make_shared<internal::window_public_constructor>(Window_Info,
                                                                 this);
}

GLFWwindow* window::GetHandle() const noexcept
{
    return this->windowHandle;
}

std::vector<window_key_event> window::GetKeyEvents() noexcept
{
    std::scoped_lock lock(this->keyEventsMutex);
    return this->keyEvents;
}

std::vector<window_character_event> window::GetCharacterEvents() noexcept
{
    std::scoped_lock lock(this->characterEventsMutex);
    return this->characterEvents;
}

std::vector<window_cursor_position_event>
window::GetCursorPositionEvents() noexcept
{
    std::scoped_lock lock(this->cursorPositionEventsMutex);
    return this->cursorPositionEvents;
}

std::vector<window_cursor_enter_event> window::GetCursorEnterEvents() noexcept
{
    std::scoped_lock lock(this->cursorEnterEventsMutex);
    return this->cursorEnterEvents;
}

std::vector<window_mouse_button_event> window::GetMouseButtonEvents() noexcept
{
    std::scoped_lock lock(this->mouseButtonEventsMutex);
    return this->mouseButtonEvents;
}

std::vector<window_scroll_event> window::GetScrollEvents() noexcept
{
    std::scoped_lock lock(this->scrollEventsMutex);
    return this->scrollEvents;
}

std::vector<window_file_drop_event> window::GetFileDropEvents() noexcept
{
    std::scoped_lock lock(this->fileDropEventsMutex);
    return this->fileDropEvents;
}

[[nodiscard]] size_t window::GetCloseEvents() noexcept
{
    std::scoped_lock lock(this->closeEventsMutex);
    return this->closeEvents;
}

[[nodiscard]] std::vector<window_size_event> window::GetSizeEvents() noexcept
{
    std::scoped_lock lock(this->sizeEventsMutex);
    return this->sizeEvents;
}

[[nodiscard]] std::vector<window_framebuffer_size_event>
window::GetFramebufferSizeEvents() noexcept
{
    std::scoped_lock lock(this->framebufferSizeEventsMutex);
    return this->framebufferSizeEvents;
}

[[nodiscard]] std::vector<window_content_scale_event>
window::GetContentScaleEvents() noexcept
{
    std::scoped_lock lock(this->contentScaleEventsMutex);
    return this->contentScaleEvents;
}

[[nodiscard]] std::vector<window_position_event>
window::GetPositionEvents() noexcept
{
    std::scoped_lock lock(this->positionEventsMutex);
    return this->positionEvents;
}

[[nodiscard]] std::vector<window_iconify_event>
window::GetIconifyEvents() noexcept
{
    std::scoped_lock lock(this->iconifyEventsMutex);
    return this->iconifyEvents;
}

[[nodiscard]] std::vector<window_maximize_event>
window::GetMaximizeEvents() noexcept
{
    std::scoped_lock lock(this->maximizeEventsMutex);
    return this->maximizeEvents;
}

[[nodiscard]] std::vector<window_focus_event> window::GetFocusEvents() noexcept
{
    std::scoped_lock lock(this->focusEventsMutex);
    return this->focusEvents;
}

[[nodiscard]] size_t window::GetRefreshEvents() noexcept
{
    std::scoped_lock lock(this->refreshEventsMutex);
    return this->refreshEvents;
}

void window::ClearKeyEvents() noexcept
{
    std::scoped_lock lock(this->keyEventsMutex);
    this->keyEvents.clear();
}

void window::ClearCharacterEvents() noexcept
{
    std::scoped_lock lock(this->characterEventsMutex);
    this->characterEvents.clear();
}

void window::ClearCursorPositionEvents() noexcept
{
    std::scoped_lock lock(this->cursorPositionEventsMutex);
    this->cursorPositionEvents.clear();
}

void window::ClearCursorEnterEvents() noexcept
{
    std::scoped_lock lock(this->cursorEnterEventsMutex);
    this->cursorEnterEvents.clear();
}

void window::ClearMouseButtonEvents() noexcept
{
    std::scoped_lock lock(this->mouseButtonEventsMutex);
    this->mouseButtonEvents.clear();
}

void window::ClearScrollEvents() noexcept
{
    std::scoped_lock lock(this->scrollEventsMutex);
    this->scrollEvents.clear();
}

void window::ClearFileDropEvents() noexcept
{
    std::scoped_lock lock(this->fileDropEventsMutex);
    this->fileDropEvents.clear();
}

void window::ClearCloseEvents() noexcept
{
    std::scoped_lock lock(this->closeEventsMutex);
    this->closeEvents = 0;
}

void window::ClearSizeEvents() noexcept
{
    std::scoped_lock lock(this->sizeEventsMutex);
    this->sizeEvents.clear();
}

void window::ClearFramebufferSizeEvents() noexcept
{
    std::scoped_lock lock(this->framebufferSizeEventsMutex);
    this->framebufferSizeEvents.clear();
}

void window::ClearContentScaleEvents() noexcept
{
    std::scoped_lock lock(this->contentScaleEventsMutex);
    this->contentScaleEvents.clear();
}

void window::ClearPositionEvents() noexcept
{
    std::scoped_lock lock(this->positionEventsMutex);
    this->positionEvents.clear();
}

void window::ClearIconifyEvents() noexcept
{
    std::scoped_lock lock(this->iconifyEventsMutex);
    this->iconifyEvents.clear();
}

void window::ClearMaximizeEvents() noexcept
{
    std::scoped_lock lock(this->maximizeEventsMutex);
    this->maximizeEvents.clear();
}

void window::ClearFocusEvents() noexcept
{
    std::scoped_lock lock(this->focusEventsMutex);
    this->focusEvents.clear();
}

void window::ClearRefreshEvents() noexcept
{
    std::scoped_lock lock(this->refreshEventsMutex);
    this->refreshEvents = 0;
}

void window::ClearEvents() noexcept
{
    this->ClearKeyEvents();
    this->ClearCharacterEvents();
    this->ClearCursorPositionEvents();
    this->ClearCursorEnterEvents();
    this->ClearMouseButtonEvents();
    this->ClearScrollEvents();
    this->ClearFileDropEvents();
    this->ClearCloseEvents();
    this->ClearSizeEvents();
    this->ClearFramebufferSizeEvents();
    this->ClearContentScaleEvents();
    this->ClearPositionEvents();
    this->ClearIconifyEvents();
    this->ClearMaximizeEvents();
    this->ClearFocusEvents();
    this->ClearRefreshEvents();
}

window_key_action window::GetKeyState(window_key Key)
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    return window_key_action(glfwGetKey(this->windowHandle, int(Key)));
}

bool window::IsKeyPressed(window_key Key)
{
    return (this->GetKeyState(Key) == window_key_action::ePress);
}

bool window::IsKeyReleased(window_key Key)
{
    return (this->GetKeyState(Key) == window_key_action::eRelease);
}

bool window::IsKeyRepeating(window_key Key)
{
    return (this->GetKeyState(Key) == window_key_action::eRepeat);
}

coordinate<double> window::GetCursorPosition()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    coordinate<double> cursorPosition = { 0, 0 };
    glfwGetCursorPos(this->windowHandle, &cursorPosition.x, &cursorPosition.y);
    return cursorPosition;
}

bool window::ShouldClose()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    return bool(glfwWindowShouldClose(this->windowHandle) != GLFW_FALSE);
}

bool window::ShouldNotClose()
{
    return !this->ShouldClose();
}

void window::ShouldClose(bool State)
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetWindowShouldClose(this->windowHandle, static_cast<int>(State));
}

gvw::area<int> window::GetSize()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    return this->GetSizeNoMutex();
}

void window::SetSize(const area<int>& Size)
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    this->SetSizeNoMutex(Size);
}

gvw::area<int> window::GetFramebufferSize()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    return this->GetFramebufferSizeNoMutex();
}

gvw::coordinate<int> window::GetPosition()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    return this->GetPositionNoMutex();
}

void window::SetPosition(const coordinate<int>& Position)
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    this->SetPositionNoMutex(Position);
}

gvw::coordinate<float> window::GetContentScale()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    coordinate<float> contentScale = { 0.0F, 0.0F };
    glfwGetWindowContentScale(
        this->windowHandle, &contentScale.x, &contentScale.y);
    return contentScale;
}

void window::SetSizeLimits(const area<int>& Minimum_Size,
                           const area<int>& Maximum_Size)
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetWindowSizeLimits(this->windowHandle,
                            Minimum_Size.width,
                            Minimum_Size.height,
                            Maximum_Size.width,
                            Maximum_Size.height);
}

void window::SetAspectRatio(int Numerator, int Denominator)
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetWindowAspectRatio(this->windowHandle, Numerator, Denominator);
}

bool window::IsCursorHovering()
{
    return static_cast<bool>(this->GetWindowAttribute(GLFW_HOVERED));
}

bool window::IsResizable()
{
    return static_cast<bool>(this->GetWindowAttribute(GLFW_RESIZABLE));
}

void window::Decorate()
{
    this->SetWindowAttribute(GLFW_DECORATED, GLFW_TRUE);
}

void window::Undecorate()
{
    this->SetWindowAttribute(GLFW_DECORATED, GLFW_FALSE);
}

bool window::IsDecorated()
{
    return bool(this->GetWindowAttribute(GLFW_DECORATED));
}

void window::MinimizeOnFocusLoss()
{
    this->SetWindowAttribute(GLFW_AUTO_ICONIFY, GLFW_TRUE);
}

void window::DontMinimizeOnFocusLoss()
{
    this->SetWindowAttribute(GLFW_AUTO_ICONIFY, GLFW_FALSE);
}

bool window::IsMinimizedOnFocusLoss()
{
    return bool(this->GetWindowAttribute(GLFW_AUTO_ICONIFY));
}

void window::AlwaysOnTop()
{
    this->SetWindowAttribute(GLFW_FLOATING, GLFW_TRUE);
}

void window::NotAlwaysOnTop()
{
    this->SetWindowAttribute(GLFW_FLOATING, GLFW_FALSE);
}

bool window::IsAlwaysOnTop()
{
    return static_cast<bool>(this->GetWindowAttribute(GLFW_FLOATING));
}

void window::Restore()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwRestoreWindow(this->windowHandle);
}

void window::Minimize()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwIconifyWindow(this->windowHandle);
}

bool window::IsMinimized()
{
    return static_cast<bool>(this->GetWindowAttribute(GLFW_ICONIFIED));
}

void window::Maximize()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwMaximizeWindow(this->windowHandle);
}

bool window::IsMaximized()
{
    return static_cast<bool>(this->GetWindowAttribute(GLFW_MAXIMIZED));
}

void window::Hide()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    this->HideNoMutex();
}

void window::Show()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    this->ShowNoMutex();
}

bool window::IsVisible()
{
    return static_cast<bool>(this->GetWindowAttribute(GLFW_VISIBLE));
}

void window::RequestAttention()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwRequestWindowAttention(this->windowHandle);
}

void window::Focus()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwFocusWindow(this->windowHandle);
}

bool window::IsFocused()
{
    return static_cast<bool>(this->GetWindowAttribute(GLFW_FOCUSED));
}

void window::FocusOnShow()
{
    this->SetWindowAttribute(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
}

void window::DontFocusOnShow()
{
    this->SetWindowAttribute(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
}

bool window::IsFocusedOnShow()
{
    return static_cast<bool>(this->GetWindowAttribute(GLFW_FOCUS_ON_SHOW));
}

float window::GetOpacity()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    return glfwGetWindowOpacity(this->windowHandle);
}

void window::SetOpacity(float Opacity)
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetWindowOpacity(this->windowHandle, Opacity);
}

bool window::IsTransparent()
{
    return static_cast<bool>(
        this->GetWindowAttribute(GLFW_TRANSPARENT_FRAMEBUFFER));
}

void window::EnterFullScreen(const monitor_ptr& Full_Screen_Monitor,
                             const GLFWvidmode* Video_Mode)
{
    /// @todo Avoid deadlock here.
    this->resetMutex.lock();
    this->resetPosition = this->GetPosition();
    this->resetSize = this->GetSize();
    this->resetMutex.unlock();
    const GLFWvidmode* videoMode = Video_Mode == nullptr
                                       ? Full_Screen_Monitor->GetVideoMode()
                                       : Video_Mode;
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetWindowMonitor(this->windowHandle,
                         Full_Screen_Monitor->GetHandle(),
                         GLFW_FALSE,
                         GLFW_FALSE,
                         videoMode->width,
                         videoMode->height,
                         videoMode->refreshRate);
}

void window::ExitFullScreen(const std::optional<coordinate<int>>& Position,
                            const std::optional<area<int>>& Size)
{
    /// @todo Avoid deadlock here.
    this->resetMutex.lock();
    const coordinate<int>& position = Position.value_or(this->resetPosition);
    const area<int>& size = Size.value_or(this->resetSize);
    this->resetMutex.unlock();
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetWindowMonitor(this->windowHandle,
                         nullptr,
                         position.x,
                         position.y,
                         size.width,
                         size.height,
                         GLFW_FALSE);
}

void window::SetCursorShape(const cursor_ptr& Cursor) // NOLINT
{
    if (Cursor == nullptr) {
        ErrorCallback("Failed to set cursor. Cursor pointer is NULL.");
        return;
    }
    this->cursor = Cursor;
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetCursor(this->windowHandle, this->cursor->handle);
}

void window::ResetCursorShape()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetCursor(this->windowHandle, nullptr);
}

void window::HideCursor()
{
    this->SetInputMode(
        static_cast<int>(internal::window_input_mode::eCursor),
        static_cast<int>(internal::window_input_mode_cursor::eHidden));
}

bool window::IsCursorHidden()
{
    return (static_cast<internal::window_input_mode_cursor>(this->GetInputMode(
                static_cast<int>(internal::window_input_mode::eCursor))) ==
            internal::window_input_mode_cursor::eHidden);
}

void window::DisableCursor()
{
    this->SetInputMode(
        static_cast<int>(internal::window_input_mode::eCursor),
        static_cast<int>(internal::window_input_mode_cursor::eDisabled));
}

bool window::IsCursorDisabled()
{
    return (static_cast<internal::window_input_mode_cursor>(this->GetInputMode(
                static_cast<int>(internal::window_input_mode::eCursor))) ==
            internal::window_input_mode_cursor::eDisabled);
}

void window::ResetCursorVisibility()
{
    this->SetInputMode(
        static_cast<int>(internal::window_input_mode::eCursor),
        static_cast<int>(internal::window_input_mode_cursor::eNormal));
}

bool window::IsCursorVisible()
{
    return (static_cast<internal::window_input_mode_cursor>(this->GetInputMode(
                static_cast<int>(internal::window_input_mode::eCursor))) ==
            internal::window_input_mode_cursor::eNormal);
}

void window::EnableStickyKeys()
{
    this->SetInputMode(
        static_cast<int>(internal::window_input_mode::eStickyKeys),
        static_cast<int>(
            internal::window_input_mode_sticky_mouse_buttons::eTrue));
}

void window::DisableStickyKeys()
{
    this->SetInputMode(
        static_cast<int>(internal::window_input_mode::eStickyKeys),
        static_cast<int>(
            internal::window_input_mode_sticky_mouse_buttons::eFalse));
}

bool window::IsStickyKeysEnabled()
{
    return (
        static_cast<internal::window_input_mode_sticky_keys>(this->GetInputMode(
            static_cast<int>(internal::window_input_mode::eStickyKeys))) ==
        internal::window_input_mode_sticky_keys::eTrue);
}

void window::EnableStickyMouseButtons()
{
    this->SetInputMode(
        static_cast<int>(internal::window_input_mode::eStickyMouseButtons),
        static_cast<int>(
            internal::window_input_mode_sticky_mouse_buttons::eTrue));
}

void window::DisableStickyMouseButtons()
{
    this->SetInputMode(
        static_cast<int>(internal::window_input_mode::eStickyMouseButtons),
        static_cast<int>(
            internal::window_input_mode_sticky_mouse_buttons::eFalse));
}

bool window::IsStickyMouseButtonsEnabled()
{
    return (static_cast<internal::window_input_mode_sticky_mouse_buttons>(
                this->GetInputMode(static_cast<int>(
                    internal::window_input_mode::eStickyMouseButtons))) ==
            internal::window_input_mode_sticky_mouse_buttons::eTrue);
}

void window::SetIcon(const image_ptr& Icon)
{
    if (Icon == nullptr) {
        ErrorCallback("Failed to set icon. Icon pointer is NULL.");
        return;
    }
    GLFWimage iconImage = { .width = Icon->size.width,
                            .height = Icon->size.height,
                            .pixels = Icon->data };
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetWindowIcon(this->windowHandle, 1, &iconImage);
}

void window::ResetIcon()
{
    std::scoped_lock lock(internal::global::GLFW_MUTEX);
    glfwSetWindowIcon(this->windowHandle, 0, nullptr);
}

} // namespace gvw