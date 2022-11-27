#pragma once

namespace vgw {

class Window
{
  public:
    // Constructor
    Window();

    // Destructor
    ~Window();

    // Free resources (calls the destructor)
    inline void Destroy() { this->~Window(); }

    // Delete the copy constructor and copy assignment operators
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
};

} // namespace vgw