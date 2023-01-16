#pragma once

// Standard includes
#include <cstdint>
#include <vector>

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace gvw {

struct version
{
    int major;
    int minor;
    int revision;
};

template<typename type>
struct size;

template<typename type>
struct coordinate
{
    type x;
    type y;
    coordinate<type>& operator=(const coordinate<type>& coordinate);
    coordinate<type>& operator=(const size<type>& area);
};

template<typename type>
struct size
{
    type width;
    type height;
    size<type>& operator=(const size<type>& area);
    size<type>& operator=(const coordinate<type>& coordinate);
};

struct key_event
{
    int key;
    int scancode;
    int action;
    int mods;
};
using character_event = unsigned int;
using cursor_position_event = coordinate<double>;
using cursor_enter_event = int;
struct mouse_button_event
{
    int button;
    int action;
    int mods;
};
using scroll_event = coordinate<double>;
struct joystick_event
{
    int jid;
    int event;
};
struct file_drop_event
{
    int count;
    const char** paths;
};

struct image
{
    int width;
    int height;
    int componentsPerPixel;
    std::vector<uint8_t> pixelData;
    operator GLFWimage();
};

} // namespace gvw