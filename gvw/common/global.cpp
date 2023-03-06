#include "global.hpp"
#include <GLFW/glfw3.h>

namespace gvw {

namespace global {

GLFWerrorfun ERROR_CALLBACK = nullptr;
bool THROW_ON_GLFW_ERROR = GLFW_FALSE;

} // namespace global

} // namespace gvw