#include "monitor.hpp"

// Local includes
#include "window.hpp"

namespace glfw {

inline void monitor::Init(GLFWmonitor* monitorId)
{
    this->monitorId_ = monitorId;
}

inline void monitor::SetGammaRamp(const GLFWgammaramp& gammaRamp)
{
    glfwSetGammaRamp(this->monitorId_, &gammaRamp);
}

inline void monitor::ResetGammaRamp()
{
    glfwSetGamma(this->monitorId_, GAMMA_DEFAULT);
}

} // namespace glfw