#pragma once

/**
 * @file window.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief The window class.
 * @date 2023-05-22
 */

// Standard includes
#include <optional>

// Local includes
#include "gvw.hpp"

class gvw::window
{
    ////////////////////////////////////////////////////////////
    ///                   Private Variables                  ///
    ////////////////////////////////////////////////////////////

    /// @brief The parent GVW instance.
    ptr gvw;

    /// @brief The pointer to the underlying GLFW window object.
    /// @remark This pointer never changes while the window exists, so it
    /// doesn't need to be atomic.
    GLFWwindow* windowHandle = nullptr;

    /// @brief Window surface.
    vk::UniqueSurfaceKHR surface;

    /// @brief Logical device.
    device_ptr logicalDeviceInfo;

    /// @todo Make this private.
  public:
    vk::Device logicalDevice; // NOLINT

  private:
    /// @brief Graphics and presentation queue info.
    /// @remark The graphics queue is also used for transfer operations.
    uint32_t graphicsQueueIndex;
    vk::Queue graphicsQueue;
    uint32_t presentQueueIndex;
    vk::Queue presentQueue;

    vk::UniqueRenderPass renderPass;

    swapchain_ptr swapchainInfo;

    /// @todo Shaders might not belong here.
    std::vector<shader> shaders;

    /// @brief Graphics pipeline.
    pipeline_ptr pipeline;

    /// @brief Command pool and command buffers.
    vk::UniqueCommandPool commandPool;
    vk::UniqueCommandBuffer stagingCommandBuffer;
    std::vector<vk::UniqueCommandBuffer> commandBuffers;

    /// @brief Vertex buffer.
    buffer vertexStagingBuffer;
    buffer vertexBuffer;

    /// @brief Semaphores and fences.
    std::vector<vk::UniqueSemaphore> nextImageAvailableSemaphores;
    std::vector<vk::UniqueSemaphore> finishedRenderingSemaphores;
    std::vector<vk::UniqueFence> inFlightFences;

    /// @brief Semaphore triggering configuration.
    std::vector<vk::PipelineStageFlags> waitStages;

    /// @brief Frames in flight.
    const uint32_t MAX_FRAMES_IN_FLIGHT = 1;
    uint32_t currentFrameIndex = 0;

    /// @brief The reset position of the window. This is the position of the
    /// window when it exits full screen, maximization, or iconification.
    coordinate<int> resetPosition = {};

    /// @brief The reset size of the window. This is the size of the window when
    /// it exits full screen, maximization, or iconification.
    area<int> resetSize = {};

    /// @brief Mutex for `resetPosition` and `resetSize`.
    std::mutex resetMutex;

    /// @brief Key events.
    std::vector<window_event::key> keyEvents;
    std::mutex keyEventsMutex;

    /// @brief Character events.
    std::vector<window_event::character> characterEvents;
    std::mutex characterEventsMutex;

    /// @brief Cursor position events.
    std::vector<window_event::cursor_position> cursorPositionEvents;
    std::mutex cursorPositionEventsMutex;

    /// @brief Cursor enter events.
    std::vector<window_event::cursor_enter> cursorEnterEvents;
    std::mutex cursorEnterEventsMutex;

    /// @brief Mouse button events.
    std::vector<window_event::mouse_button> mouseButtonEvents;
    std::mutex mouseButtonEventsMutex;

    /// @brief Scroll events.
    std::vector<window_event::scroll> scrollEvents;
    std::mutex scrollEventsMutex;

    /// @brief File drop events.
    std::vector<window_event::file_drop> fileDropEvents;
    std::mutex fileDropEventsMutex;

    /// @brief The number of close events.
    size_t closeEvents = 0;
    std::mutex closeEventsMutex;

    /// @brief Size events.
    std::vector<window_event::size> sizeEvents;
    std::mutex sizeEventsMutex;

    /// @brief Framebuffer size events.
    std::vector<window_event::framebuffer_size> framebufferSizeEvents;
    std::mutex framebufferSizeEventsMutex;

    /// @brief Content scale events.
    std::vector<window_event::content_scale> contentScaleEvents;
    std::mutex contentScaleEventsMutex;

    /// @brief Position events.
    std::vector<window_event::position> positionEvents;
    std::mutex positionEventsMutex;

    /// @brief Iconify events.
    std::vector<window_event::iconify> iconifyEvents;
    std::mutex iconifyEventsMutex;

    /// @brief Maximize events.
    std::vector<window_event::maximize> maximizeEvents;
    std::mutex maximizeEventsMutex;

    /// @brief Focus events.
    std::vector<window_event::focus> focusEvents;
    std::mutex focusEventsMutex;

    /// @brief The number of refresh events.
    size_t refreshEvents = 0;
    std::mutex refreshEventsMutex;

    ////////////////////////////////////////////////////////////
    ///       Constructors, Operators, and Destructors       ///
    ////////////////////////////////////////////////////////////

    /// @brief Creates a window.
    /// @remark This constructor is made private to prevent if from being called
    /// from outside of GVW.
    window(ptr GVW,
           const window_info& Window_Info = DEFAULT_WINDOW_INFO,
           GLFWwindow* Parent_Window = nullptr,
           const std::optional<device_ptr>& Logical_Device = std::nullopt);

    // Allow the private constructor to be called by the parent class.
    friend class gvw;

  public:
    // Delete the copy constructor, move constructor, copy assignment operator,
    // and move assignment operator. It should not be possible to copy or move
    // this object.
    window(const window&) = delete;
    window(window&&) noexcept = delete;
    window& operator=(const window&) = delete;
    window& operator=(window&&) noexcept = delete;

    // The destructor is public so as to allow explicit destruction using the
    // delete operator.
    ~window();

  private:
    ////////////////////////////////////////////////////////////
    ///                   Private Functions                  ///
    ////////////////////////////////////////////////////////////

    /// @brief Creates the swapchain.
    void CreateSwapchain();

    /// @brief Creates the graphics pipeline.
    void CreatePipeline(const std::vector<vk::DynamicState>& Dynamic_States =
                            dynamic_states::VIEWPORT_AND_SCISSOR,
                        const std::vector<vk::VertexInputBindingDescription>&
                            Vertex_Input_Binding_Descriptions =
                                NO_VERTEX_BINDING_DESCRIPTIONS,
                        const std::vector<vk::VertexInputAttributeDescription>&
                            Vertex_Input_Attribute_Description =
                                NO_VERTEX_ATTRIBUTE_DESCRIPTIONS);

  public:
    /// @brief Draws a frame.
    /// @todo This function does a lot of stuff that should be manually managed
    /// by the user. Reconsider it's existance here.
    /// @todo Make this function private, or remove it entirely?
    void DrawFrame(const std::vector<vertex>& Vertices);

  private:
    /// @brief Returns an attribute of the window.
    [[nodiscard]] int WindowAttribute(int Attribute);

    /// @brief Sets an attribute of the window.
    void WindowAttribute(int Attribute, int Value);

    /// @brief Sets GLFW event callbacks. Passing no arguments sets all default
    /// event callbacks, which populate the event buffers.
    /// @warning This function is NOT thread safe.
    void EventCallbacksNoMutex(const window_event_callbacks& Event_Callbacks =
                                   NO_WINDOW_EVENT_CALLBACKS) const;

    /// @brief Returns the size of the content area of the window in screen
    /// coordinates.
    /// @warning This function is NOT thread safe.
    [[nodiscard]] area<int> SizeNoMutex() const;

    /// @brief Sets the size of the window.
    /// @warning This function is NOT thread safe.
    void SizeNoMutex(const area<int>& Size) const;

    /// @brief Returns the size of the content area of the window in pixels.
    /// @warning This function is NOT thread safe.
    [[nodiscard]] area<int> FramebufferSizeNoMutex() const;

    /// @brief Returns the position of the window in screen coordinates.
    /// @warning This function is NOT thread safe.
    [[nodiscard]] coordinate<int> PositionNoMutex() const;

    /// @brief Sets the position of the window in screen coordinates.
    /// @warning This function is NOT thread safe.
    void PositionNoMutex(const coordinate<int>& Position) const;

    /// @brief Hides the window.
    /// @warning This function is NOT thread safe.
    void HideNoMutex() const;

    /// @brief Shows the window.
    /// @warning This function is NOT thread safe.
    void ShowNoMutex() const;

  public:
    ////////////////////////////////////////////////////////////
    ///                   Public Functions                   ///
    ////////////////////////////////////////////////////////////

    /// @brief Sets GLFW event callbacks. Passing no arguments sets all default
    /// event callbacks, which populate the event buffers.
    void EventCallbacks(const window_event_callbacks& Event_Callbacks =
                            NO_WINDOW_EVENT_CALLBACKS);

    /// @brief Creates a child window.
    [[nodiscard]] window CreateChildWindow(
        const window_info& Window_Info = DEFAULT_WINDOW_INFO);

    /// @brief Returns the handle to the underlying GLFW window object.
    [[nodiscard]] GLFWwindow* Handle() const noexcept;

    /// @brief Returns the key event buffer.
    [[nodiscard]] std::vector<window_event::key> KeyEvents() noexcept;

    /// @brief Returns the character event buffer.
    [[nodiscard]] std::vector<window_event::character>
    CharacterEvents() noexcept;

    /// @brief Returns the cursor position buffer.
    [[nodiscard]] std::vector<window_event::cursor_position>
    CursorPositionEvents() noexcept;

    /// @brief Returns the cursor enter event buffer.
    [[nodiscard]] std::vector<window_event::cursor_enter>
    CursorEnterEvents() noexcept;

    /// @brief Returns the mouse button event buffer.
    [[nodiscard]] std::vector<window_event::mouse_button>
    MouseButtonEvents() noexcept;

    /// @brief Returns the scroll event buffer.
    [[nodiscard]] std::vector<window_event::scroll> ScrollEvents() noexcept;

    /// @brief Returns the file drop event buffer.
    [[nodiscard]] std::vector<window_event::file_drop>
    FileDropEvents() noexcept;

    /// @brief Returns the number of close events received by the window.
    [[nodiscard]] size_t CloseEvents() noexcept;

    /// @brief Returns the size event buffer.
    [[nodiscard]] std::vector<window_event::size> SizeEvents() noexcept;

    /// @brief Returns the framebuffer size event buffer.
    [[nodiscard]] std::vector<window_event::framebuffer_size>
    FramebufferSizeEvents() noexcept;

    /// @brief Returns the content scale event buffer.
    [[nodiscard]] std::vector<window_event::content_scale>
    ContentScaleEvents() noexcept;

    /// @brief Returns the position event buffer.
    [[nodiscard]] std::vector<window_event::position> PositionEvents() noexcept;

    /// @brief Returns the iconify event buffer.
    [[nodiscard]] std::vector<window_event::iconify> IconifyEvents() noexcept;

    /// @brief Returns the maximize event buffer.
    [[nodiscard]] std::vector<window_event::maximize> MaximizeEvents() noexcept;

    /// @brief Returns the focus event buffer.
    [[nodiscard]] std::vector<window_event::focus> FocusEvents() noexcept;

    /// @brief Returns the number of refresh events received by the window.
    [[nodiscard]] size_t RefreshEvents() noexcept;

    /// @brief Clears the key event buffer.
    void ClearKeyEvents() noexcept;

    /// @brief Clears the character event buffer.
    void ClearCharacterEvents() noexcept;

    /// @brief Clears the cursor position event buffer.
    void ClearCursorPositionEvents() noexcept;

    /// @brief Clears the cursor enter event buffer.
    void ClearCursorEnterEvents() noexcept;

    /// @brief Clears the mouse button event buffer.
    void ClearMouseButtonEvents() noexcept;

    /// @brief Clears the scroll event buffer.
    void ClearScrollEvents() noexcept;

    /// @brief Clears the file drop event buffer.
    void ClearFileDropEvents() noexcept;

    /// @brief Resets the number of close events received by the window to zero.
    void ClearCloseEvents() noexcept;

    /// @brief Clears the size event buffer.
    void ClearSizeEvents() noexcept;

    /// @brief Clears the framebuffer size event buffer.
    void ClearFramebufferSizeEvents() noexcept;

    /// @brief Clears the content scale event buffer.
    void ClearContentScaleEvents() noexcept;

    /// @brief Clears the position event buffer.
    void ClearPositionEvents() noexcept;

    /// @brief Clears the iconify event buffer.
    void ClearIconifyEvents() noexcept;

    /// @brief Clears the maximize event buffer.
    void ClearMaximizeEvents() noexcept;

    /// @brief Clears the focus event buffer.
    void ClearFocusEvents() noexcept;

    /// @brief Resets the number of refresh events received by the window to
    /// zero.
    void ClearRefreshEvents() noexcept;

    /// @brief Clears all event buffers.
    void ClearEvents() noexcept;

    /// @brief Returns the state of a key.
    [[nodiscard]] int KeyState(int Key) noexcept;

    /// @brief Returns the state of the 'close' flag.
    [[nodiscard]] bool ShouldClose() const;

    /// @brief Sets the state of the 'close' flag.
    void ShouldClose(bool State) const;

    /// @brief Returns the size of the content area of the window in screen
    /// coordinates.
    [[nodiscard]] area<int> Size();

    /// @brief Sets the size of the window.
    void Size(const area<int>& Size);

    /// @brief Returns the size of the content area of the window in pixels.
    [[nodiscard]] area<int> FramebufferSize();

    /// @brief Returns the position of the window in screen coordinates.
    [[nodiscard]] coordinate<int> Position();

    /// @brief Sets the position of the window in screen coordinates.
    void Position(const coordinate<int>& Position);

    /// @brief Returns the X and Y content scale of the window.
    /// @remark The content scale of the window is the ratio between the current
    /// DPI and the platform's default DPI.
    [[nodiscard]] coordinate<float> ContentScale();

    /// @brief Sets the minimum and maximum size limits of the window.
    void SizeLimits(
        const area<int>& Minimum_Size = window_size_limit::NO_MINIMUM,
        const area<int>& Maximum_Size = window_size_limit::NO_MAXIMUM);

    /// @brief Sets the aspect ratio of the window.
    void AspectRatio(int Numerator = GLFW_DONT_CARE,
                     int Denominator = GLFW_DONT_CARE);

    /// @brief Returns the cursor hovering state over the window.
    [[nodiscard]] bool IsCursorHovering();

    /// @brief Returns the resizability of the window.
    [[nodiscard]] bool IsResizable();

    /// @brief Decorates the window.
    void Decorate();

    /// @brief Undecorates the window.
    void Undecorate();

    /// @brief Returns the decoration state of the window.
    [[nodiscard]] bool IsDecorated();

    /// @brief Minimizes the window when focus is lost.
    void MinimizeOnFocusLoss();

    /// @brief Does NOT minimize the window when focus is lost.
    void DontMinimizeOnFocusLoss();

    /// @brief Returns the state of the window's minimization on focus loss.
    [[nodiscard]] bool IsMinimizedOnFocusLoss();

    /// @brief Sets the window on top of all other windows even when focus is
    /// lost.
    void AlwaysOnTop();

    /// @brief Allows other windows to cover the window if focus is lost.
    void NotAlwaysOnTop();

    /// @brief Returns the state of the window's precidence over other windows
    /// if focus is lost.
    [[nodiscard]] bool IsAlwaysOnTop();

    /// @brief Unmaximizes or unminimizes the window.
    void Restore();

    /// @brief Minimizes the window.
    void Minimize();

    /// @brief Returns the minimization state of the window.
    [[nodiscard]] bool IsMinimized();

    /// @brief Maximizes the window.
    void Maximize();

    /// @brief Returns the Maximization state of the window.
    [[nodiscard]] bool IsMaximized();

    /// @brief Hides the window.
    void Hide();

    /// @brief Shows the window.
    void Show();

    /// @brief Returns the visibility of the window.
    [[nodiscard]] bool IsVisible();

    /// @brief Requests attention for the window.
    void RequestAttention();

    /// @brief Brings focus to the window.
    void Focus();

    /// @brief Returns the state of the window's focus.
    [[nodiscard]] bool IsFocused();

    /// @brief Brings focus to the window when it is shown.
    void FocusOnShow();

    /// @brief Does NOT bring focus to the window when it is shown.
    void DontFocusOnShow();

    /// @brief Returns the state of focus brought to the window when it is
    /// shown.
    [[nodiscard]] bool IsFocusedOnShow();

    /// @brief Returns the opacity of the window.
    [[nodiscard]] float Opacity();

    /// @brief Sets the opacity of the entire window including decorations.
    void Opacity(float Opacity);

    /// @brief Returns the transparency of the framebuffer.
    [[nodiscard]] bool IsTransparent();

    /// @brief Enters full screen.
    void EnterFullScreen(monitor& Full_Screen_Monitor,
                         const GLFWvidmode* Video_Mode = nullptr);

    /// @brief Exits full screen.
    void ExitFullScreen(
        const std::optional<coordinate<int>>& Position = std::nullopt,
        const std::optional<area<int>>& Size = std::nullopt);

    /// @todo Declare and define the icon functions.
    /// @todo Declare and define the cursor customization functions.
};