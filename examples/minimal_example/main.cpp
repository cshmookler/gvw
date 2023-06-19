#include "../../gvw/gvw.hpp"
int main()
{
    gvw::window_t window;
    while (!window.ShouldClose()) {
        gvw::WaitThenPollEvents();
    }
}