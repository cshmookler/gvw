#include "types.tpp"

namespace gvw {

image::operator GLFWimage()
{
    GLFWimage glfwImage = { .width = this->width,
                            .height = this->height,
                            .pixels = this->pixelData.data() };
    return glfwImage;
}

} // namespace gvw