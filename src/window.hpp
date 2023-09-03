#pragma once

/**
 * @file window.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief Window management.
 * @date 2023-05-22
 */

// Local includes
#include "gvw.hpp"

namespace gvw {

class window : public internal::uncopyable_unmovable // NOLINT
{
    friend internal::window_public_constructor;

    friend instance;

    ////////////////////////////////////////////////////////////////////////////
    ///                Constructors, Operators, and Destructor               ///
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Creates a window.
    /// @remark This constructor is made private to prevent if from being called
    /// from outside of GVW.
    window(const window_info& Window_Info = window_info_config::DEFAULT,
           window* Parent_Window = nullptr);

  public:
    // The destructor is public to allow explicit destruction.
    ~window();

  private:
    ////////////////////////////////////////////////////////////////////////////
    ///                        Private Static Functions                      ///
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Destroys the GLFW window.
    static void DestroyGlfwWindow(GLFWwindow* Window_Handle) noexcept;

    ////////////////////////////////////////////////////////////////////////////
    ///                           Private Variables                          ///
    ////////////////////////////////////////////////////////////////////////////

    instance_ptr gvwInstance;

    std::unique_ptr<internal::terminator<GLFWwindow*>> glfwWindowDestroyer;

    /// @brief The pointer to the underlying GLFW window object.
    GLFWwindow* windowHandle = nullptr;

    /// @brief The GLFW cursor.
    cursor_ptr cursorHandle = nullptr;

    /// @brief Window surface.
    vk::UniqueSurfaceKHR surface;

    /// @brief Logical device.
    device_ptr logicalDevice;

    /// @brief Graphics and presentation queue info.
    /// @remark The graphics queue is also used for transfer operations.
    uint32_t graphicsQueueIndex;
    vk::Queue graphicsQueue;
    uint32_t presentQueueIndex;
    vk::Queue presentQueue;

    render_pass_ptr renderPass;

    swapchain_ptr swapchain;

    /// @todo Shaders might not belong here.
    pipeline_shaders shaders;

    /// @brief Graphics pipeline.
    pipeline_ptr pipeline;

    /// @brief Command pool and command buffers.
    vk::UniqueCommandPool commandPool;
    vk::UniqueCommandBuffer stagingCommandBuffer;
    std::vector<vk::UniqueCommandBuffer> commandBuffers;

    /// @brief Vertex buffers.
    buffer_ptr staticVertexStagingBuffer;
    buffer_ptr staticVertexBuffer;

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

    /// @todo Figure out how to make these private.
  public:
    // NOLINTBEGIN
    /// @brief Key events.
    std::vector<window_key_event> keyEvents;
    std::mutex keyEventsMutex;

    /// @brief Character events.
    std::vector<window_character_event> characterEvents;
    std::mutex characterEventsMutex;

    /// @brief Cursor position events.
    std::vector<window_cursor_position_event> cursorPositionEvents;
    std::mutex cursorPositionEventsMutex;

    /// @brief Cursor enter events.
    std::vector<window_cursor_enter_event> cursorEnterEvents;
    std::mutex cursorEnterEventsMutex;

    /// @brief Mouse button events.
    std::vector<window_mouse_button_event> mouseButtonEvents;
    std::mutex mouseButtonEventsMutex;

    /// @brief Scroll events.
    std::vector<window_scroll_event> scrollEvents;
    std::mutex scrollEventsMutex;

    /// @brief File drop events.
    std::vector<window_file_drop_event> fileDropEvents;
    std::mutex fileDropEventsMutex;

    /// @brief The number of close events.
    size_t closeEvents = 0;
    std::mutex closeEventsMutex;

    /// @brief Size events.
    std::vector<window_size_event> sizeEvents;
    std::mutex sizeEventsMutex;

    /// @brief Framebuffer size events.
    std::vector<window_framebuffer_size_event> framebufferSizeEvents;
    std::mutex framebufferSizeEventsMutex;

    /// @brief Content scale events.
    std::vector<window_content_scale_event> contentScaleEvents;
    std::mutex contentScaleEventsMutex;

    /// @brief Position events.
    std::vector<window_position_event> positionEvents;
    std::mutex positionEventsMutex;

    /// @brief Iconify events.
    std::vector<window_iconify_event> iconifyEvents;
    std::mutex iconifyEventsMutex;

    /// @brief Maximize events.
    std::vector<window_maximize_event> maximizeEvents;
    std::mutex maximizeEventsMutex;

    /// @brief Focus events.
    std::vector<window_focus_event> focusEvents;
    std::mutex focusEventsMutex;

    /// @brief The number of refresh events.
    size_t refreshEvents = 0;
    std::mutex refreshEventsMutex;

    // NOLINTEND
  private:
    ////////////////////////////////////////////////////////////////////////////
    ///                        Private Member Functions                      ///
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Sets the GLFW window user pointer.
    void SetUserPointer(void* Pointer);

    /// @brief Creates the swapchain.
    void CreateSwapchain();

    /// @brief Creates the graphics pipeline.
    void CreatePipeline(const pipeline_dynamic_states& Dynamic_States);

  public:
    /// @brief Draws a frame.
    /// @todo This function does a lot of stuff that should be manually managed
    /// by the user. Reconsider it's existance here.
    /// @todo Make this function private or remove it entirely.
    void DrawFrame(const std::vector<vertex>& Vertices);

  private:
    /// @brief Returns an attribute of the window.
    [[nodiscard]] int GetWindowAttribute(int Attribute);

    /// @brief Sets an attribute of the window.
    void SetWindowAttribute(int Attribute, int Value);

    /// @brief Sets GLFW event callbacks. Passing no arguments sets all default
    /// event callbacks, which populate the event buffers.
    /// @warning This function is NOT thread safe.
    void SetEventCallbacksNoMutex(
        const window_event_callbacks& Event_Callbacks) const;

    /// @brief Returns the size of the content area of the window in screen
    /// coordinates.
    /// @warning This function is NOT thread safe.
    [[nodiscard]] area<int> GetSizeNoMutex() const;

    /// @brief Sets the size of the window.
    /// @warning This function is NOT thread safe.
    void SetSizeNoMutex(const area<int>& Size) const;

    /// @brief Returns the size of the content area of the window in pixels.
    /// @warning This function is NOT thread safe.
    [[nodiscard]] area<int> GetFramebufferSizeNoMutex() const;

    /// @brief Returns the position of the window in screen coordinates.
    /// @warning This function is NOT thread safe.
    [[nodiscard]] coordinate<int> GetPositionNoMutex() const;

    /// @brief Sets the position of the window in screen coordinates.
    /// @warning This function is NOT thread safe.
    void SetPositionNoMutex(const coordinate<int>& Position) const;

    /// @brief Hides the window.
    /// @warning This function is NOT thread safe.
    void HideNoMutex() const;

    /// @brief Shows the window.
    /// @warning This function is NOT thread safe.
    void ShowNoMutex() const;

  public:
    ////////////////////////////////////////////////////////////////////////////
    ///                        Public Member Functions                       ///
    ////////////////////////////////////////////////////////////////////////////

    /// @brief Sets GLFW event callbacks. Passing no arguments sets all default
    /// event callbacks, which populate the event buffers.
    void SetEventCallbacks(const window_event_callbacks& Event_Callbacks);

    /// @brief Creates a child window.
    [[nodiscard]] window_ptr CreateChildWindow(
        const window_info& Window_Info = window_info_config::DEFAULT);

    /// @brief Returns the handle to the underlying GLFW window object.
    [[nodiscard]] GLFWwindow* GetHandle() const noexcept;

    /// @brief Returns the key event buffer.
    [[nodiscard]] std::vector<window_key_event> GetKeyEvents() noexcept;

    /// @brief Returns the character event buffer.
    [[nodiscard]] std::vector<window_character_event>
    GetCharacterEvents() noexcept;

    /// @brief Returns the cursor position buffer.
    [[nodiscard]] std::vector<window_cursor_position_event>
    GetCursorPositionEvents() noexcept;

    /// @brief Returns the cursor enter event buffer.
    [[nodiscard]] std::vector<window_cursor_enter_event>
    GetCursorEnterEvents() noexcept;

    /// @brief Returns the mouse button event buffer.
    [[nodiscard]] std::vector<window_mouse_button_event>
    GetMouseButtonEvents() noexcept;

    /// @brief Returns the scroll event buffer.
    [[nodiscard]] std::vector<window_scroll_event> GetScrollEvents() noexcept;

    /// @brief Returns the file drop event buffer.
    [[nodiscard]] std::vector<window_file_drop_event>
    GetFileDropEvents() noexcept;

    /// @brief Returns the number of close events received by the window.
    [[nodiscard]] size_t GetCloseEvents() noexcept;

    /// @brief Returns the size event buffer.
    [[nodiscard]] std::vector<window_size_event> GetSizeEvents() noexcept;

    /// @brief Returns the framebuffer size event buffer.
    [[nodiscard]] std::vector<window_framebuffer_size_event>
    GetFramebufferSizeEvents() noexcept;

    /// @brief Returns the content scale event buffer.
    [[nodiscard]] std::vector<window_content_scale_event>
    GetContentScaleEvents() noexcept;

    /// @brief Returns the position event buffer.
    [[nodiscard]] std::vector<window_position_event>
    GetPositionEvents() noexcept;

    /// @brief Returns the iconify event buffer.
    [[nodiscard]] std::vector<window_iconify_event> GetIconifyEvents() noexcept;

    /// @brief Returns the maximize event buffer.
    [[nodiscard]] std::vector<window_maximize_event>
    GetMaximizeEvents() noexcept;

    /// @brief Returns the focus event buffer.
    [[nodiscard]] std::vector<window_focus_event> GetFocusEvents() noexcept;

    /// @brief Returns the number of refresh events received by the window.
    [[nodiscard]] size_t GetRefreshEvents() noexcept;

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
    [[nodiscard]] int GetKeyState(int Key) noexcept;

    /// @brief Returns the state of the 'close' flag.
    [[nodiscard]] bool ShouldClose() const;

    /// @brief Returns the inverse state of the 'close' flag.
    [[nodiscard]] bool ShouldNotClose() const;

    /// @brief Sets the state of the 'close' flag.
    void ShouldClose(bool State) const;

    /// @brief Returns the size of the content area of the window in screen
    /// coordinates.
    [[nodiscard]] area<int> GetSize();

    /// @brief Sets the size of the window.
    void SetSize(const area<int>& Size);

    /// @brief Returns the size of the content area of the window in pixels.
    [[nodiscard]] area<int> GetFramebufferSize();

    /// @brief Returns the position of the window in screen coordinates.
    [[nodiscard]] coordinate<int> GetPosition();

    /// @brief Sets the position of the window in screen coordinates.
    void SetPosition(const coordinate<int>& Position);

    /// @brief Returns the X and Y content scale of the window.
    /// @remark The content scale of the window is the ratio between the current
    /// DPI and the platform's default DPI.
    [[nodiscard]] coordinate<float> GetContentScale();

    /// @brief Sets the minimum and maximum size limits of the window.
    void SetSizeLimits(const area<int>& Minimum_Size,
                       const area<int>& Maximum_Size);

    /// @brief Sets the aspect ratio of the window.
    void SetAspectRatio(int Numerator, int Denominator);

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
    [[nodiscard]] float GetOpacity();

    /// @brief Sets the opacity of the entire window including decorations.
    void SetOpacity(float Opacity);

    /// @brief Returns the transparency of the framebuffer.
    [[nodiscard]] bool IsTransparent();

    /// @brief Enters full screen.
    void EnterFullScreen(const monitor_ptr& Full_Screen_Monitor,
                         const GLFWvidmode* Video_Mode = nullptr);

    /// @brief Exits full screen.
    void ExitFullScreen(
        const std::optional<coordinate<int>>& Position = std::nullopt,
        const std::optional<area<int>>& Size = std::nullopt);

    /// @brief Sets a custom cursor.
    void SetCursor(cursor_ptr Cursor);

    /// @brief Resets the cursor.
    void ResetCursor();

    /// @todo Declare and define the icon functions.
};

} // namespace gvw