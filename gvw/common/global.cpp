#include "global.hpp"

namespace gvw {

namespace internal {

GLFWerrorfun ERROR_CALLBACK = nullptr;

} // namespace internal

bool THROW_ON_GLFW_ERROR = GLFW_FALSE;

} // namespace gvw