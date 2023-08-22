#pragma once

/**
 * @file device.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief Logical device wrapper class.
 * @date 2023-07-26
 */

// Local includes
#include "gvw.hpp"

class gvw::device
{
    ////////////////////////////////////////////////////////////
    ///                   Private Variables                  ///
    ////////////////////////////////////////////////////////////

    /// @todo Make these private.
  public:
    std::shared_ptr<vk::UniqueDevice> logicalDevice;
    vk::PhysicalDevice physicalDevice;
    vk::SurfaceFormatKHR surfaceFormat;
    vk::PresentModeKHR presentMode;
    std::vector<device_selection::queue_family_info> queueInfos;

  private:
    ////////////////////////////////////////////////////////////
    ///       Constructors, Operators, and Destructors       ///
    ////////////////////////////////////////////////////////////

    device(std::shared_ptr<vk::UniqueDevice> Logical_Device,
           const vk::PhysicalDevice& Physical_Device,
           const vk::SurfaceFormatKHR& Surface_Format,
           const vk::PresentModeKHR& Present_Mode,
           const std::vector<device_selection::queue_family_info>& Queue_Infos);

    // Allow the private constructor to be called by the parent class.
    friend class gvw;

  public:
    ////////////////////////////////////////////////////////////
    ///                Public Member Functions               ///
    ////////////////////////////////////////////////////////////

    [[nodiscard]] std::vector<shader> LoadShadersFromSpirVFiles(
        const std::vector<shader_info>& Shader_Infos);

    [[nodiscard]] buffer CreateBuffer(
        const buffer_info& Buffer_Info = DEFAULT_BUFFER_INFO);

    [[nodiscard]] render_pass_ptr CreateRenderPass(
        const render_pass_info& Render_Pass_Info = DEFAULT_RENDER_PASS_INFO);

    [[nodiscard]] swapchain_ptr CreateSwapchain(
        const swapchain_info& Swapchain_Info = DEFAULT_SWAPCHAIN_INFO);

    [[nodiscard]] pipeline_ptr CreatePipeline(
        const pipeline_info& Pipeline_Info = DEFAULT_PIPELINE_INFO);
};

class gvw::device_public_constructor : public device
{
  public:
    template<typename... Args>
    device_public_constructor(Args&&... Arguments)
        : device(std::forward<Args>(Arguments)...)
    {
    }
};