#pragma once

/**
 * @file init.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief GVW initialization functions and variables.
 * @date 2023-06-03
 */

// Standard includes

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Local includes
#include "types.tpp"

namespace gvw {

//////////////////////////////////////////////////
//                Const Variables               //
//////////////////////////////////////////////////

/// @brief GLFW initialization failed.
extern const error ERROR_GLFW_INIT_FAILED;

/// @brief Vulkan is not supported on the system.
extern const error ERROR_VULKAN_NOT_SUPPORTED;

/// @brief Default values for shared GLFW initialization hints.
extern const glfw_shared_init_hints DEFAULT_GLFW_SHARED_INIT_HINTS;

/// @brief Default values for MacOS GLFW initialization hints.
extern const glfw_macos_init_hints DEFAULT_GLFW_MACOS_INIT_HINTS;

//////////////////////////////////////////////////
//                   Variables                  //
//////////////////////////////////////////////////
// NOLINTBEGIN

/// @brief The number of objects depending on GVW being initialized.
extern size_t INSTANCE_COUNT;

/// @brief Pointer to the GVW error callback.
extern std::function<gvw_error_callback> GVW_ERROR_CALLBACK;

// NOLINTEND
//////////////////////////////////////////////////
//                   Functions                  //
//////////////////////////////////////////////////

/// @brief default gvw error callback. throws an exception for each error.
/// @param GVW_Error The error code and a human readable description of the
/// error.
/// @gvw_errors All errors.
/// @glfw_errors None.
/// @thread_safety Can be called from any thread.
void DefaultGvwErrorCallback(error GVW_Error);

/// @brief Intermediate error callback for handling GLFW errors.
/// @param Error_Code The error code.
/// @param Description A human readable description of the error.
/// @gvw_errors None.
/// @glfw_errors All errors.
/// @thread_safety Can be called from any thread.
void DefaultGlfwErrorCallback(int Error_Code, const char* Description);

/// @brief Returns the GLFW version used at runtime.
/// @return `version` object.
/// @gvw_errors None.
/// @glfw_errors None.
/// @thread_safety Can be called from any thread.
[[nodiscard]] version GetGlfwRuntimeVersion() noexcept;

/// @brief Returns the GLFW version used to compile GVW.
/// @return `version` object.
/// @gvw_errors None.
/// @glfw_errors None.
/// @thread_safety Can be called from any thread.
[[nodiscard]] version GetGlfwCompiletimeVersion() noexcept;

/// @brief Sets the GVW error callback. If no argument is provided,
/// `DefaultErrorCallback` is set as the GVW error callback.
/// @param GVW_Error_Callback The GVW error callback.
/// @gvw_errors None.
/// @glfw_errors None.
/// @thread_safety Must be called from the main thread.
void SetGvwErrorCallback(
    gvw_error_callback GVW_Error_Callback = DefaultGvwErrorCallback) noexcept;

/// @brief Sets the GLFW error callback. If no argument is provided,
/// `DefaultGlfwErrorCallback` is set as the GLFW error callback.
/// @param GLFW_Error_Callback The GLFW error callback.
/// @gvw_errors None.
/// @glfw_errors None.
/// @thread_safety Must be called from the main thread.
void SetGlfwErrorCallback(
    GLFWerrorfun GLFW_Error_Callback = DefaultGlfwErrorCallback) noexcept;

/// @brief Initializes the GVW library.
/// @param GVW_Error_Callback The GVW error callback. The default GVW error
/// callback throws all errors.
/// @param GLFW_Error_Callback The GLFW error callback. The default GLFW
/// error callback throws all errors.
/// @param Shared_Init_Hints Shared initialization hints for GLFW. Ignored
/// if GLFW is already initialized.
/// @param Macos_Init_Hints MacOS specific initialization hints for GLFW.
/// Ignored if GLFW is already initialized.
/// @gvw_errors None.
/// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
/// @thread_safety Must be called from the main thread.
void Init(gvw_error_callback GVW_Error_Callback = DefaultGvwErrorCallback,
          GLFWerrorfun GLFW_Error_Callback = DefaultGlfwErrorCallback,
          const glfw_shared_init_hints& Shared_Init_Hints =
              DEFAULT_GLFW_SHARED_INIT_HINTS,
          const glfw_macos_init_hints& MacOS_Init_Hints =
              DEFAULT_GLFW_MACOS_INIT_HINTS);

/// @brief Polls events for all windows.
/// @gvw_errors None.
/// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
/// @thread_safety Must be called from the main thread.
void PollEvents();

/// @brief Waits until a window event is received, then polls events for all
/// windows.
/// @gvw_errors None.
/// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
/// @thread_safety Must be called from the main thread.
void WaitThenPollEvents();

/// @brief Waits until either the timout expires or a window event is
/// received, then polls events for all windows.
/// @param Timeout The timeout in milliseconds.
/// @gvw_errors None.
/// @glfw_errors `GLFW_NOT_INITIALIZED`, `GLFW_INVALID_VALUE`, or
/// `GLFW_PLATFORM_ERROR`.
/// @thread_safety Must be called from the main thread.
void WaitThenPollEvents(double Timeout);

/// @brief Posts an empty event. Causes `WaitThenPollEvents` to poll events.
/// @gvw_errors None.
/// @glfw_errors `GLFW_NOT_INITIALIZED` or `GLFW_PLATFORM_ERROR`.
/// @thread_safety Must be called from the main thread.
void PostEmptyEvent();

} // namespace gvw