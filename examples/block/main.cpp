#include <iostream>

#include "../../gvw/gvw.hpp"

int main(int argc, char** argv)
{
    gvw::Init();
    gvw::window window;

    std::cout << "Throw on GLFW error: " << gvw::global::THROW_ON_GLFW_ERROR
              << std::endl;

    window.Create(600, 400, "block", "", nullptr, nullptr);

    while (!window.ShouldClose()) {
        gvw::PollEvents();
    }

    window.Destroy();
    gvw::Destroy();
}