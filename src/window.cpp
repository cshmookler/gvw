// Local includes
#include "window.hpp"

// Standard includes
#include <algorithm>
#include <utility>

gvw::window_creation_hints::window_creation_hints(const info& Creation_Hints)
    : glfw_hints(
          { { { GLFW_RESIZABLE, Creation_Hints.resizable },
              { GLFW_VISIBLE, Creation_Hints.visible },
              { GLFW_DECORATED, Creation_Hints.decorated },
              { GLFW_FOCUSED, Creation_Hints.focused },
              { GLFW_AUTO_ICONIFY, Creation_Hints.autoIconify },
              { GLFW_FLOATING, Creation_Hints.floating },
              { GLFW_MAXIMIZED, Creation_Hints.maximized },
              { GLFW_CENTER_CURSOR, Creation_Hints.centerCursor },
              { GLFW_TRANSPARENT_FRAMEBUFFER,
                Creation_Hints.transparentFramebuffer },
              { GLFW_FOCUS_ON_SHOW, Creation_Hints.focusOnShow },
              { GLFW_SCALE_TO_MONITOR, Creation_Hints.scaleToMonitor },
              { GLFW_RED_BITS, Creation_Hints.redBits },
              { GLFW_GREEN_BITS, Creation_Hints.greenBits },
              { GLFW_BLUE_BITS, Creation_Hints.blueBits },
              { GLFW_ALPHA_BITS, Creation_Hints.alphaBits },
              { GLFW_DEPTH_BITS, Creation_Hints.depthBits },
              { GLFW_STENCIL_BITS, Creation_Hints.stencilBits },
              { GLFW_AUX_BUFFERS, Creation_Hints.auxBuffers },
              { GLFW_STEREO, Creation_Hints.stereo },
              { GLFW_SAMPLES, Creation_Hints.samples },
              { GLFW_SRGB_CAPABLE, Creation_Hints.srgbCapable },
              { GLFW_DOUBLEBUFFER, Creation_Hints.doubleBuffer },
              { GLFW_REFRESH_RATE, Creation_Hints.refreshRate },
              { GLFW_CLIENT_API, Creation_Hints.clientApi },
              { GLFW_CONTEXT_CREATION_API, Creation_Hints.contextCreationApi },
              { GLFW_CONTEXT_VERSION_MAJOR,
                Creation_Hints.contextVersionMajor },
              { GLFW_CONTEXT_VERSION_MINOR,
                Creation_Hints.contextVersionMinor },
              { GLFW_OPENGL_FORWARD_COMPAT,
                Creation_Hints.openglForwardCompatible },
              { GLFW_OPENGL_DEBUG_CONTEXT, Creation_Hints.openglDebugContext },
              { GLFW_OPENGL_PROFILE, Creation_Hints.openglProfile },
              { GLFW_CONTEXT_ROBUSTNESS, Creation_Hints.contextRobustness },
              { GLFW_CONTEXT_RELEASE_BEHAVIOR,
                Creation_Hints.contextReleaseBehavior },
              { GLFW_CONTEXT_NO_ERROR, Creation_Hints.contextNoError },
              { GLFW_COCOA_RETINA_FRAMEBUFFER,
                Creation_Hints.cocoaRetinaFramebuffer },
              { GLFW_COCOA_GRAPHICS_SWITCHING,
                Creation_Hints.cocoaGraphicsSwitching } } },
          { { { GLFW_COCOA_FRAME_NAME, Creation_Hints.cocoaFrameName },
              { GLFW_X11_CLASS_NAME, Creation_Hints.x11ClassName },
              { GLFW_X11_INSTANCE_NAME, Creation_Hints.x11InstanceName } } })
{
}

// NOLINTNEXTLINE
void gvw::window::DestroyGlfwWindow(ptr GVW, GLFWwindow* Window_Handle) noexcept
{
    std::scoped_lock<std::mutex> lock(GVW->glfwMutex);
    glfwDestroyWindow(Window_Handle);
}

gvw::window::window(ptr GVW,
                    const window_info& Window_Info,
                    window* Parent_Window)
    : gvw(std::move(GVW))
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);

    // Apply window creation hints.
    Window_Info.creationHints.Apply();

    // Briefly hide the window if an initial position was specified. If this is
    // not done the window may be in the wrong position for one frame (the
    // default position set by the operating system).
    if (Window_Info.position.has_value()) {
        glfwWindowHint(Window_Info.creationHints.visible.HINT, GLFW_FALSE);
    }

    // Create the window.
    this->windowHandle =
        glfwCreateWindow(Window_Info.size->width,
                         Window_Info.size->height,
                         Window_Info.title,
                         Window_Info.fullScreenMonitor,
                         nullptr); // "We use Vulkan in this household!"

    this->glfwWindowDestroyer = std::make_unique<terminator<ptr, GLFWwindow*>>(
        DestroyGlfwWindow, this->gvw, this->windowHandle);

    // Link this window object with the underlying GLFW window object.
    glfwSetWindowUserPointer(this->windowHandle, this);

    // Set an initial position if one was specified.
    if (Window_Info.position.has_value()) {
        this->PositionNoMutex(Window_Info.position.value());
        // Show the window now that it has been moved to the correct position.
        this->ShowNoMutex();
    }

    // Set the reset position and size of the window.
    this->resetMutex.lock();
    this->resetPosition = PositionNoMutex();
    this->resetSize = SizeNoMutex();
    this->resetMutex.unlock();

    // Set window event callbacks.
    this->EventCallbacksNoMutex(Window_Info.eventCallbacks);

    // Create window surface.
    VkSurfaceKHR tempSurface = nullptr;
    if (glfwCreateWindowSurface(*this->gvw->vulkanInstance,
                                this->windowHandle,
                                nullptr,
                                &tempSurface) != VK_SUCCESS) {
        gvwErrorCallback("Window surface creation failed");
    }
    this->surface =
        vk::UniqueSurfaceKHR(tempSurface, *this->gvw->vulkanInstance);

    // Use an already existing logical device or create a new one.
    if (Window_Info.device != nullptr) {
        this->logicalDeviceInfo = Window_Info.device;
    } else if (Parent_Window != nullptr) {
        this->logicalDeviceInfo = Parent_Window->logicalDeviceInfo;
    } else {
        this->logicalDeviceInfo =
            gvw->SelectPhysicalDevices(Window_Info.deviceInfo,
                                       this->surface.get())
                .at(0);
    }
    /// @todo Remove this. Rename `logicalDeviceInfo` to `logicalDevice`.
    this->logicalDevice = this->logicalDeviceInfo->logicalDevice->get();

    // Find indicies for the graphics and present queue families.
    std::optional<uint32_t> viableGraphicsQueueFamilyIndex;
    std::optional<uint32_t> viablePresentationQueueFamilyIndex;
    const auto& queueInfos = this->logicalDeviceInfo->queueInfos;
    for (const auto& queueInfo : queueInfos) {
        if (viableGraphicsQueueFamilyIndex.has_value() == false) {
            if (bool(queueInfo.properties.queueFlags &
                     vk::QueueFlagBits::eGraphics)) {
                viableGraphicsQueueFamilyIndex =
                    queueInfo.createInfo.queueFamilyIndex;
            }
        }
        if (viablePresentationQueueFamilyIndex.has_value() == false) {
            if (this->logicalDeviceInfo->physicalDevice.getSurfaceSupportKHR(
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
        gvwErrorCallback("The selected physical device does not offer a queue "
                         "family that supports graphics.");
    }
    if (viablePresentationQueueFamilyIndex.has_value() == false) {
        gvwErrorCallback("The selected physical device does not offer a queue "
                         "family that supports presentation.");
    }
    this->graphicsQueueIndex = viableGraphicsQueueFamilyIndex.value();
    this->presentQueueIndex = viablePresentationQueueFamilyIndex.value();

    /// @todo Allow selection of specific queue indicies.
    this->graphicsQueue =
        this->logicalDeviceInfo->logicalDevice->get().getQueue(
            this->graphicsQueueIndex, 0);
    this->presentQueue = this->logicalDeviceInfo->logicalDevice->get().getQueue(
        this->presentQueueIndex, 0);

    // Use an already existing render pass or create a new one.
    if (Window_Info.renderPass != nullptr) {
        if (Window_Info.renderPass->handle.getOwner() != this->logicalDevice) {
            gvwErrorCallback("Cannot use a render pass created with a "
                             "different logical device.");
        }
        this->renderPass = Window_Info.renderPass;
    } else {
        this->renderPass = this->logicalDeviceInfo->CreateRenderPass(
            { .format = this->logicalDeviceInfo->surfaceFormat.format });
    }

    // Create swapchain.
    this->CreateSwapchain();

    // Use an already existing pipeline or create a new one.
    if (Window_Info.pipeline != nullptr) {
        if (Window_Info.pipeline->pipeline.getOwner() != this->logicalDevice) {
            gvwErrorCallback("Cannot use a pipeline created with a different "
                             "logical device.");
        }
        this->shaders = Window_Info.shaders;
        this->pipeline = Window_Info.pipeline;
    } else {
        // Attempt to load the default shaders.
        std::vector<gvw::shader_info> shaderInfo = {
            { .code = "vert.spv", .stage = vk::ShaderStageFlagBits::eVertex },
            { .code = "frag.spv", .stage = vk::ShaderStageFlagBits::eFragment }
        };
        this->shaders =
            this->logicalDeviceInfo->LoadShadersFromSpirVFiles(shaderInfo);

        std::vector<vk::VertexInputBindingDescription> bindingDescriptions = {
            { .binding = 0,
              .stride = sizeof(vertex),
              .inputRate = vk::VertexInputRate::eVertex }
        };
        std::vector<vk::VertexInputAttributeDescription>
            attributeDescriptions = {
                { { .location = 0,
                    .binding = 0,
                    .format = vk::Format::eR32G32Sfloat,
                    .offset = offsetof(vertex, position) },
                  { .location = 1,
                    .binding = 0,
                    .format = vk::Format::eR32G32B32Sfloat,
                    .offset = offsetof(vertex, color) } }
            };
        this->CreatePipeline(dynamic_states::VIEWPORT_AND_SCISSOR,
                             bindingDescriptions,
                             attributeDescriptions);
    }

    // Create the command pool.
    vk::CommandPoolCreateInfo commandPoolCreateInfo = {
        .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
        .queueFamilyIndex = graphicsQueueIndex
    };
    this->commandPool =
        this->logicalDevice.createCommandPoolUnique(commandPoolCreateInfo);

    // Create vertex staging buffer.
    this->vertexStagingBuffer = this->logicalDeviceInfo->CreateBuffer(
        { .sizeInBytes = Window_Info.sizeOfDynamicVerticesInBytes,
          .usage = vk::BufferUsageFlagBits::eTransferSrc,
          .memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible |
                              vk::MemoryPropertyFlagBits::eHostCoherent });

    // Create destination vertex buffer.
    this->vertexBuffer = this->logicalDeviceInfo->CreateBuffer(
        { .sizeInBytes = vertexStagingBuffer.size,
          .usage = vk::BufferUsageFlagBits::eTransferDst |
                   vk::BufferUsageFlagBits::eVertexBuffer,
          .memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal });

    // Allocate a command buffer from the command pool for transfering vertex
    // data from the staging buffer to the destination buffer.
    vk::CommandBufferAllocateInfo stagingCommandBufferAllocateInfo = {
        .commandPool = commandPool.get(),
        .level = vk::CommandBufferLevel::ePrimary,
        .commandBufferCount = 1
    };
    this->stagingCommandBuffer = std::move(
        this->logicalDevice
            .allocateCommandBuffersUnique(stagingCommandBufferAllocateInfo)
            .at(0));

    vk::CommandBufferBeginInfo stagingCommandBufferBeginInfo = {
        // The 'OneTimeSubmit' flag tells Vulkan that this command buffer will
        // only be submitted once, so optimizations can be made that would
        // normally be blocked to allow subsequent rerecording of the command
        // buffer.
        // .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit
    };
    this->stagingCommandBuffer->begin(stagingCommandBufferBeginInfo);

    this->stagingCommandBuffer->copyBuffer(
        this->vertexStagingBuffer.handle.get(),
        this->vertexBuffer.handle.get(),
        { vk::BufferCopy{
            .srcOffset = 0, .dstOffset = 0, .size = vertexBuffer.size } });

    this->stagingCommandBuffer->end();

    // Allocate a command buffer from the command pool.
    vk::CommandBufferAllocateInfo commandBufferAllocateInfo = {
        .commandPool = commandPool.get(),
        .level = vk::CommandBufferLevel::ePrimary,
        .commandBufferCount = this->MAX_FRAMES_IN_FLIGHT
    };
    this->commandBuffers = this->logicalDevice.allocateCommandBuffersUnique(
        commandBufferAllocateInfo);

    // Create semaphores and fences to control the execution order in the
    // device and synchronize the host with the device.
    vk::SemaphoreCreateInfo semaphoreCreateInfo = {};
    vk::FenceCreateInfo fenceCreateInfo = {
        .flags = vk::FenceCreateFlagBits::eSignaled
    };

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        nextImageAvailableSemaphores.emplace_back(
            this->logicalDevice.createSemaphoreUnique(semaphoreCreateInfo));
        finishedRenderingSemaphores.emplace_back(
            this->logicalDevice.createSemaphoreUnique(semaphoreCreateInfo));
        inFlightFences.emplace_back(
            this->logicalDevice.createFenceUnique(fenceCreateInfo));
    }

    // Configure semaphore triggering.
    this->waitStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
}

gvw::window::~window()
{
    this->logicalDevice.waitIdle();
}

void gvw::window::CreateSwapchain()
{
    swapchain_ptr oldSwapchain;
    if (this->swapchainInfo != nullptr) {
        oldSwapchain = this->swapchainInfo;
    }

    this->swapchainInfo = this->logicalDeviceInfo->CreateSwapchain(
        { .framebufferSize = this->FramebufferSizeNoMutex(),
          .graphicsQueueIndex = this->graphicsQueueIndex,
          .presentQueueIndex = this->presentQueueIndex,
          .surface = this->surface.get(),
          .renderPass = this->renderPass->handle.get(),
          .oldSwapchain = oldSwapchain });
}

void gvw::window::CreatePipeline(
    const std::vector<vk::DynamicState>& Dynamic_States,
    const std::vector<vk::VertexInputBindingDescription>&
        Vertex_Input_Binding_Descriptions,
    const std::vector<vk::VertexInputAttributeDescription>&
        Vertex_Input_Attribute_Description)
{
    this->pipeline = this->logicalDeviceInfo->CreatePipeline(
        { .shaders = this->shaders,
          .dynamicStates = Dynamic_States,
          .vertexInputBindingDescriptions = Vertex_Input_Binding_Descriptions,
          .vertexInputAttributeDescriptions =
              Vertex_Input_Attribute_Description,
          .renderPass = this->renderPass->handle.get() });
}

void gvw::window::DrawFrame(const std::vector<vertex>& Vertices)
{
    // Wait until the previous frame is done rendering.
    if (logicalDevice.waitForFences(
            this->inFlightFences.at(this->currentFrameIndex).get(),
            VK_TRUE,
            UINT64_MAX) != vk::Result::eSuccess) {
        gvwErrorCallback("Failed to wait for the previous "
                         "frame to finish rendering.");
    }

    // Get an image from the swapchain to render to.
    vk::ResultValue<uint32_t> imageIndex = logicalDevice.acquireNextImageKHR(
        this->swapchainInfo->handle.get(),
        UINT64_MAX,
        nextImageAvailableSemaphores.at(currentFrameIndex).get());

    if (imageIndex.result == vk::Result::eErrorOutOfDateKHR) {
        logicalDevice.waitIdle();
        this->CreateSwapchain();
    } else if (imageIndex.result != vk::Result::eSuccess &&
               imageIndex.result != vk::Result::eSuboptimalKHR) {
        gvwErrorCallback("Failed to acquire next image from the swapchain.");
    } else {
        logicalDevice.resetFences(inFlightFences.at(currentFrameIndex).get());

        // Map vertices to the vertex buffer.
        void* vertexStagingBufferPointer = this->logicalDevice.mapMemory(
            this->vertexStagingBuffer.memory.get(),
            0,
            this->vertexStagingBuffer.size,
            {});
        memcpy(vertexStagingBufferPointer,
               Vertices.data(),
               static_cast<size_t>(vertexStagingBuffer.size));
        this->logicalDevice.unmapMemory(this->vertexStagingBuffer.memory.get());

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
                this->swapchainInfo->swapchainFramebuffers.at(imageIndex.value)
                    .get(),
            .renderArea = { .offset = { 0, 0 },
                            .extent = this->swapchainInfo->scissor.extent },
            .clearValueCount = 1,
            .pClearValues = &clearValue
        };

        // Record the render pass in the command buffer.
        commandBuffer.beginRenderPass(renderPassBeginInfo,
                                      vk::SubpassContents::eInline);
        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics,
                                   this->pipeline->pipeline.get());
        commandBuffer.setViewport(0, this->swapchainInfo->viewport);
        commandBuffer.setScissor(0, this->swapchainInfo->scissor);
        commandBuffer.bindVertexBuffers(
            0, { this->vertexBuffer.handle.get() }, { 0 });
        // NOLINTNEXTLINE
        commandBuffer.draw(static_cast<uint32_t>(Vertices.size()), 1, 0, 0);
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
            .pSwapchains = &this->swapchainInfo->handle.get(),
            .pImageIndices = &imageIndex.value,
            .pResults = nullptr // optional
        };

        // Presents the rendered image to the swapchain which is then
        // displayed on the window surface.
        vk::Result presentResult = presentQueue.presentKHR(&presentInfo);
        if (presentResult == vk::Result::eErrorOutOfDateKHR ||
            presentResult == vk::Result::eSuboptimalKHR) {
            logicalDevice.waitIdle();
            this->CreateSwapchain();
        } else if (presentResult != vk::Result::eSuccess) {
            gvwErrorCallback("Presentation failed.");
        }

        currentFrameIndex = (currentFrameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
    }
}

int gvw::window::WindowAttribute(int Attribute)
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    return glfwGetWindowAttrib(this->windowHandle, Attribute);
}

void gvw::window::WindowAttribute(int Attribute, int Value)
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    glfwSetWindowAttrib(this->windowHandle, Attribute, Value);
}

void gvw::window::EventCallbacksNoMutex(
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

gvw::area<int> gvw::window::SizeNoMutex() const
{
    area<int> size = { 0, 0 };
    glfwGetWindowSize(this->windowHandle, &size.width, &size.height);
    return size;
}

void gvw::window::SizeNoMutex(const area<int>& Size) const
{
    glfwSetWindowSize(this->windowHandle, Size.width, Size.height);
}

gvw::area<int> gvw::window::FramebufferSizeNoMutex() const
{
    area<int> size = { 0, 0 };
    glfwGetFramebufferSize(this->windowHandle, &size.width, &size.height);
    return size;
}

gvw::coordinate<int> gvw::window::PositionNoMutex() const
{
    coordinate<int> position = { 0, 0 };
    glfwGetWindowPos(this->windowHandle, &position.x, &position.y);
    return position;
}

void gvw::window::PositionNoMutex(const coordinate<int>& Position) const
{
    glfwSetWindowPos(this->windowHandle, Position.x, Position.y);
}

void gvw::window::HideNoMutex() const
{
    glfwHideWindow(this->windowHandle);
}

void gvw::window::ShowNoMutex() const
{
    glfwShowWindow(this->windowHandle);
}

void gvw::window::EventCallbacks(const window_event_callbacks& Event_Callbacks)
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    this->EventCallbacksNoMutex(Event_Callbacks);
}

gvw::window_ptr gvw::window::CreateChildWindow(const window_info& Window_Info)
{
    return std::make_shared<window_public_constructor>(
        this->gvw, Window_Info, this);
}

GLFWwindow* gvw::window::Handle() const noexcept
{
    return this->windowHandle;
}

std::vector<gvw::window_event::key> gvw::window::KeyEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->keyEventsMutex);
    return this->keyEvents;
}

std::vector<gvw::window_event::character>
gvw::window::CharacterEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->characterEventsMutex);
    return this->characterEvents;
}

std::vector<gvw::window_event::cursor_position>
gvw::window::CursorPositionEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->cursorPositionEventsMutex);
    return this->cursorPositionEvents;
}

std::vector<gvw::window_event::cursor_enter>
gvw::window::CursorEnterEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->cursorEnterEventsMutex);
    return this->cursorEnterEvents;
}

std::vector<gvw::window_event::mouse_button>
gvw::window::MouseButtonEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->mouseButtonEventsMutex);
    return this->mouseButtonEvents;
}

std::vector<gvw::window_event::scroll> gvw::window::ScrollEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->scrollEventsMutex);
    return this->scrollEvents;
}

std::vector<gvw::window_event::file_drop> gvw::window::FileDropEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->fileDropEventsMutex);
    return this->fileDropEvents;
}

[[nodiscard]] size_t gvw::window::CloseEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->closeEventsMutex);
    return this->closeEvents;
}

[[nodiscard]] std::vector<gvw::window_event::size>
gvw::window::SizeEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->sizeEventsMutex);
    return this->sizeEvents;
}

[[nodiscard]] std::vector<gvw::window_event::framebuffer_size>
gvw::window::FramebufferSizeEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->framebufferSizeEventsMutex);
    return this->framebufferSizeEvents;
}

[[nodiscard]] std::vector<gvw::window_event::content_scale>
gvw::window::ContentScaleEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->contentScaleEventsMutex);
    return this->contentScaleEvents;
}

[[nodiscard]] std::vector<gvw::window_event::position>
gvw::window::PositionEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->positionEventsMutex);
    return this->positionEvents;
}

[[nodiscard]] std::vector<gvw::window_event::iconify>
gvw::window::IconifyEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->iconifyEventsMutex);
    return this->iconifyEvents;
}

[[nodiscard]] std::vector<gvw::window_event::maximize>
gvw::window::MaximizeEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->maximizeEventsMutex);
    return this->maximizeEvents;
}

[[nodiscard]] std::vector<gvw::window_event::focus>
gvw::window::FocusEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->focusEventsMutex);
    return this->focusEvents;
}

[[nodiscard]] size_t gvw::window::RefreshEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->refreshEventsMutex);
    return this->refreshEvents;
}

void gvw::window::ClearKeyEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->keyEventsMutex);
    this->keyEvents.clear();
}

void gvw::window::ClearCharacterEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->characterEventsMutex);
    this->characterEvents.clear();
}

void gvw::window::ClearCursorPositionEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->cursorPositionEventsMutex);
    this->cursorPositionEvents.clear();
}

void gvw::window::ClearCursorEnterEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->cursorEnterEventsMutex);
    this->cursorEnterEvents.clear();
}

void gvw::window::ClearMouseButtonEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->mouseButtonEventsMutex);
    this->mouseButtonEvents.clear();
}

void gvw::window::ClearScrollEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->scrollEventsMutex);
    this->scrollEvents.clear();
}

void gvw::window::ClearFileDropEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->fileDropEventsMutex);
    this->fileDropEvents.clear();
}

void gvw::window::ClearCloseEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->closeEventsMutex);
    this->closeEvents = 0;
}

void gvw::window::ClearSizeEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->sizeEventsMutex);
    this->sizeEvents.clear();
}

void gvw::window::ClearFramebufferSizeEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->framebufferSizeEventsMutex);
    this->framebufferSizeEvents.clear();
}

void gvw::window::ClearContentScaleEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->contentScaleEventsMutex);
    this->contentScaleEvents.clear();
}

void gvw::window::ClearPositionEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->positionEventsMutex);
    this->positionEvents.clear();
}

void gvw::window::ClearIconifyEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->iconifyEventsMutex);
    this->iconifyEvents.clear();
}

void gvw::window::ClearMaximizeEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->maximizeEventsMutex);
    this->maximizeEvents.clear();
}

void gvw::window::ClearFocusEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->focusEventsMutex);
    this->focusEvents.clear();
}

void gvw::window::ClearRefreshEvents() noexcept
{
    std::scoped_lock<std::mutex> lock(this->refreshEventsMutex);
    this->refreshEvents = 0;
}

void gvw::window::ClearEvents() noexcept
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

int gvw::window::KeyState(int Key) noexcept
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    return glfwGetKey(this->windowHandle, Key);
}

bool gvw::window::ShouldClose() const
{
    return bool(glfwWindowShouldClose(this->windowHandle) != GLFW_FALSE);
}

void gvw::window::ShouldClose(bool State) const
{
    glfwSetWindowShouldClose(this->windowHandle, static_cast<int>(State));
}

gvw::area<int> gvw::window::Size()
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    return this->SizeNoMutex();
}

void gvw::window::Size(const area<int>& Size)
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    this->SizeNoMutex(Size);
}

gvw::area<int> gvw::window::FramebufferSize()
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    return this->FramebufferSizeNoMutex();
}

gvw::coordinate<int> gvw::window::Position()
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    return this->PositionNoMutex();
}

void gvw::window::Position(const coordinate<int>& Position)
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    this->PositionNoMutex(Position);
}

gvw::coordinate<float> gvw::window::ContentScale()
{
    coordinate<float> contentScale = { 0.0F, 0.0F };
    this->gvw->glfwMutex.lock();
    glfwGetWindowContentScale(
        this->windowHandle, &contentScale.x, &contentScale.y);
    this->gvw->glfwMutex.unlock();
    return contentScale;
}

void gvw::window::SizeLimits(const area<int>& Minimum_Size,
                             const area<int>& Maximum_Size)
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    glfwSetWindowSizeLimits(this->windowHandle,
                            Minimum_Size.width,
                            Minimum_Size.height,
                            Maximum_Size.width,
                            Maximum_Size.height);
}

void gvw::window::AspectRatio(int Numerator, int Denominator)
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    glfwSetWindowAspectRatio(this->windowHandle, Numerator, Denominator);
}

bool gvw::window::IsCursorHovering()
{
    return static_cast<bool>(this->WindowAttribute(GLFW_HOVERED));
}

bool gvw::window::IsResizable()
{
    return static_cast<bool>(this->WindowAttribute(GLFW_RESIZABLE));
}

void gvw::window::Decorate()
{
    this->WindowAttribute(GLFW_DECORATED, GLFW_TRUE);
}

void gvw::window::Undecorate()
{
    this->WindowAttribute(GLFW_DECORATED, GLFW_FALSE);
}

bool gvw::window::IsDecorated()
{
    return bool(this->WindowAttribute(GLFW_DECORATED));
}

void gvw::window::MinimizeOnFocusLoss()
{
    this->WindowAttribute(GLFW_AUTO_ICONIFY, GLFW_TRUE);
}

void gvw::window::DontMinimizeOnFocusLoss()
{
    this->WindowAttribute(GLFW_AUTO_ICONIFY, GLFW_FALSE);
}

bool gvw::window::IsMinimizedOnFocusLoss()
{
    return bool(this->WindowAttribute(GLFW_AUTO_ICONIFY));
}

void gvw::window::AlwaysOnTop()
{
    this->WindowAttribute(GLFW_FLOATING, GLFW_TRUE);
}

void gvw::window::NotAlwaysOnTop()
{
    this->WindowAttribute(GLFW_FLOATING, GLFW_FALSE);
}

bool gvw::window::IsAlwaysOnTop()
{
    return static_cast<bool>(this->WindowAttribute(GLFW_FLOATING));
}

void gvw::window::Restore()
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    glfwRestoreWindow(this->windowHandle);
}

void gvw::window::Minimize()
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    glfwIconifyWindow(this->windowHandle);
}

bool gvw::window::IsMinimized()
{
    return static_cast<bool>(this->WindowAttribute(GLFW_ICONIFIED));
}

void gvw::window::Maximize()
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    glfwMaximizeWindow(this->windowHandle);
}

bool gvw::window::IsMaximized()
{
    return static_cast<bool>(this->WindowAttribute(GLFW_MAXIMIZED));
}

void gvw::window::Hide()
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    this->HideNoMutex();
}

void gvw::window::Show()
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    this->ShowNoMutex();
}

bool gvw::window::IsVisible()
{
    return static_cast<bool>(this->WindowAttribute(GLFW_VISIBLE));
}

void gvw::window::RequestAttention()
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    glfwRequestWindowAttention(this->windowHandle);
}

void gvw::window::Focus()
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    glfwFocusWindow(this->windowHandle);
}

bool gvw::window::IsFocused()
{
    return static_cast<bool>(this->WindowAttribute(GLFW_FOCUSED));
}

void gvw::window::FocusOnShow()
{
    this->WindowAttribute(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
}

void gvw::window::DontFocusOnShow()
{
    this->WindowAttribute(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
}

bool gvw::window::IsFocusedOnShow()
{
    return static_cast<bool>(this->WindowAttribute(GLFW_FOCUS_ON_SHOW));
}

float gvw::window::Opacity()
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    return glfwGetWindowOpacity(this->windowHandle);
}

void gvw::window::Opacity(float Opacity)
{
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    glfwSetWindowOpacity(this->windowHandle, Opacity);
}

bool gvw::window::IsTransparent()
{
    return static_cast<bool>(
        this->WindowAttribute(GLFW_TRANSPARENT_FRAMEBUFFER));
}

void gvw::window::EnterFullScreen(monitor& Full_Screen_Monitor,
                                  const GLFWvidmode* Video_Mode)
{
    this->resetMutex.lock();
    this->resetPosition = this->Position();
    this->resetSize = this->Size();
    this->resetMutex.unlock();
    const GLFWvidmode* videoMode =
        Video_Mode == nullptr ? Full_Screen_Monitor.VideoMode() : Video_Mode;
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    glfwSetWindowMonitor(this->windowHandle,
                         Full_Screen_Monitor.Handle(),
                         GLFW_FALSE,
                         GLFW_FALSE,
                         videoMode->width,
                         videoMode->height,
                         videoMode->refreshRate);
}

void gvw::window::ExitFullScreen(const std::optional<coordinate<int>>& Position,
                                 const std::optional<area<int>>& Size)
{
    this->resetMutex.lock();
    const coordinate<int>& position = Position.value_or(this->resetPosition);
    const area<int>& size = Size.value_or(this->resetSize);
    this->resetMutex.unlock();
    std::scoped_lock<std::mutex> lock(this->gvw->glfwMutex);
    glfwSetWindowMonitor(this->windowHandle,
                         nullptr,
                         position.x,
                         position.y,
                         size.width,
                         size.height,
                         GLFW_FALSE);
}