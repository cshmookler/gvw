#pragma once

/**
 * @file device.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief Logical device wrapper class.
 * @date 2023-07-26
 */

// Local includes
#include "gvw.ipp"

namespace gvw {

class device : internal::uncopyable_unmovable // NOLINT
{
    friend internal::device_public_constructor;

    ////////////////////////////////////////////////////////////////////////////
    ///                Constructors, Operators, and Destructor               ///
    ////////////////////////////////////////////////////////////////////////////

    device(const device_info& Device_Info);

  public:
    // The destructor is public to allow explicit destruction.
    ~device() = default;

  private:
    ////////////////////////////////////////////////////////////////////////////
    ///                           Private Variables                          ///
    ////////////////////////////////////////////////////////////////////////////

    instance_ptr gvwInstance;

    vk::UniqueDevice handle;
    vk::PhysicalDevice physicalDevice;
    vk::SurfaceFormatKHR surfaceFormat;
    vk::PresentModeKHR presentMode;
    std::vector<device_selection_queue_family_info> queueFamilyInfos;

  public:
    ////////////////////////////////////////////////////////////////////////////
    ///                        Public Member Functions                       ///
    ////////////////////////////////////////////////////////////////////////////

    [[nodiscard]] vk::Device GetHandle() const;

    [[nodiscard]] vk::PhysicalDevice GetPhysicalDevice() const;

    [[nodiscard]] vk::SurfaceFormatKHR GetSurfaceFormat() const;

    [[nodiscard]] vk::PresentModeKHR GetPresentMode() const;

    [[nodiscard]] std::vector<device_selection_queue_family_info>
    GetQueueFamilyInfos() const;

    [[nodiscard]] shader_ptr LoadShaderFromSpirVFile(
        const shader_info& Shader_Info);

    [[nodiscard]] vertex_shader_ptr LoadVertexShaderFromSpirVFile(
        const vertex_shader_info& Vertex_Shader_Info);

    [[nodiscard]] fragment_shader_ptr LoadFragmentShaderFromSpirVFile(
        const fragment_shader_info& Fragment_Shader_Info);

    [[nodiscard]] buffer_ptr CreateBuffer(
        const buffer_info& Buffer_Info = buffer_info_config::DEFAULT);

    [[nodiscard]] render_pass_ptr CreateRenderPass(
        const render_pass_info& Render_Pass_Info =
            render_pass_info_config::DEFAULT);

    [[nodiscard]] swapchain_ptr CreateSwapchain(
        const swapchain_info& Swapchain_Info = swapchain_info_config::DEFAULT);

    [[nodiscard]] pipeline_ptr CreatePipeline(
        const pipeline_info& Pipeline_Info = pipeline_info_config::DEFAULT);
};

} // namespace gvw