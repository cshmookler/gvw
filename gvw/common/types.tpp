#pragma once

#include "types.hpp"

namespace gvw {

template<typename type>
coordinate<type>& coordinate<type>::operator=(
    const coordinate<type>& coordinate)
{
    this->x = coordinate.x;
    this->y = coordinate.y;

    return *this;
}

template<typename type>
coordinate<type>& coordinate<type>::operator=(const size<type>& area)
{
    this->x = area.width;
    this->y = area.height;

    return *this;
}

template<typename type>
size<type>& size<type>::operator=(const size<type>& area)
{
    this->width = area.width;
    this->height = area.height;

    return *this;
}

template<typename type>
size<type>& size<type>::operator=(const coordinate<type>& coordinate)
{
    this->width = coordinate.x;
    this->height = coordinate.y;

    return *this;
}

} // namespace gvw