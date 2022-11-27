#pragma once

namespace vgw {

class Glfw
{
  public:
    // Constructor
    Glfw();

    // Destructor
    ~Glfw();

    // Initialize GLFW
    int Init(void (*errorCallback)(int, const char*) = GlfwErrorCallback_);

    // Free resources (calls the destructor)
    inline void Destroy() { this->~Glfw(); }

    // Delete the copy constructor and copy assignment operators
    Glfw(const Glfw&) = delete;
    Glfw& operator=(const Glfw&) = delete;

  private:
    // Handle GLFW errors
    static void GlfwErrorCallback_(int errorCode, const char* description);
};

} // namespace vgw