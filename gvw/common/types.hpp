#pragma once

/**
 * @file types.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief Structs used in multiple source files
 * @date 2023-02-02
 */

// Standard includes
#include <cstdint>
#include <vector>

// External includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace gvw {

/// @brief Version major, minor, and revision.
struct version
{
    int major;
    int minor;
    int revision;
};

template<typename type>
struct size;

/// @brief An x and y value pair. Structurally identical to `gvw::size`.
/// @tparam type
template<typename type>
struct coordinate
{
    type x;
    type y;
    coordinate<type>& operator=(const coordinate<type>& coordinate);
    coordinate<type>& operator=(const size<type>& area);
};

/// @brief A width and height. Structurally identical to `gvw::coordinate`.
/// @tparam type
template<typename type>
struct size
{
    type width;
    type height;
    size<type>& operator=(const size<type>& area);
    size<type>& operator=(const coordinate<type>& coordinate);
};

/// @brief The key, scancode, action, and mods of a key event.
struct key_event
{
    int key;
    int scancode;
    int action;
    int mods;
};

/// @brief A character event stored as an unsigned int. Can be interpreted as a
/// UTF-8, UTF-16, or UTF-32 character.
using character_event = unsigned int;

/// @brief A cursor position event stored as a `gvw::coordinate` of type double.
using cursor_position_event = coordinate<double>;

/// @brief A cursor enter/leave event stored as an integer. Can be interpreted
/// as a boolean.
using cursor_enter_event = int;

/// @brief The button, action, and mods of a mouse button event.
struct mouse_button_event
{
    int button;
    int action;
    int mods;
};

/// @brief A scroll event stored as a `gvw::coordinate` of type double.
using scroll_event = coordinate<double>;

/// @brief Joystick ID and event type of a joystick event.
struct joystick_event
{
    int jid;
    int event;
};

/// @brief File count and file paths from a file drop event.
struct file_drop_event
{
    int count;
    const char** paths;
};

/// @brief The width, height, components per pixel, and pixel data of an image.
struct image
{
    int width;
    int height;
    int componentsPerPixel;
    std::vector<uint8_t> pixelData;
    operator GLFWimage();
};

} // namespace gvw