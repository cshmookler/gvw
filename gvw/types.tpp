#pragma once

// Standard includes

// External includes

// Local includes
#include "types.hpp"

namespace gvw {

template<typename T>
glfw_hint<T>::glfw_hint(int Hint, T Value)
    : HINT(Hint)
    , value(Value)
{
}

template<typename T>
glfw_hint<T>::glfw_hint(const glfw_hint<T>& GLFW_Hint)
    : HINT(GLFW_Hint.HINT)
    , value(GLFW_Hint.value)
{
}

template<typename T>
glfw_hint<T>::glfw_hint(glfw_hint<T>&& GLFW_Hint) noexcept
    : HINT(std::move(GLFW_Hint.HINT))
    , value(std::move(GLFW_Hint.value))
{
}

template<typename T>
glfw_hint<T>& glfw_hint<T>::operator=(T Value)
{
    this->value = Value;
    return *this;
}

template<size_t IntHints, size_t StringHints>
glfw_hints<IntHints, StringHints>::glfw_hints(
    std::function<void(int, int)>&& Int_Hint_Func,
    std::function<void(int, const char*)>&& String_Hint_Func,
    std::array<glfw_hint<int>, IntHints>&& Int_Hints,
    std::array<glfw_hint<const char*>, StringHints>&& String_Hints)
    : INT_HINT_FUNC(std::move(Int_Hint_Func))
    , STRING_HINT_FUNC(std::move(String_Hint_Func))
    , intHints(std::move(Int_Hints))
    , stringHints(std::move(String_Hints))
{
}

template<size_t IntHints, size_t StringHints>
glfw_hints<IntHints, StringHints>& glfw_hints<IntHints, StringHints>::operator=(
    const glfw_hints& GLFW_Hints)
{
    *this = GLFW_Hints;
    return *this;
}

template<size_t IntHints, size_t StringHints>
glfw_hints<IntHints, StringHints>& glfw_hints<IntHints, StringHints>::operator=(
    glfw_hints&& GLFW_Hints) noexcept
{
    *this = std::move(GLFW_Hints);
    return *this;
}

template<size_t IntHints, size_t StringHints>
void glfw_hints<IntHints, StringHints>::Apply() const
{
    for (const glfw_hint<int>& intHint : this->intHints) {
        INT_HINT_FUNC(intHint.HINT, intHint.value);
    }
    for (const glfw_hint<const char*>& stringHint : this->stringHints) {
        STRING_HINT_FUNC(stringHint.HINT, stringHint.value);
    }
}

template<typename T>
coordinate<T>::coordinate(T X, T Y) // NOLINT
    : x(X)
    , y(Y)
{
}

template<typename T>
coordinate<T>::coordinate(const coordinate<T>& Coordinate)
    : x(Coordinate.x)
    , y(Coordinate.y)
{
}

template<typename T>
coordinate<T>::coordinate(const area<T>& Area)
    : x(Area.width)
    , y(Area.height)
{
}

template<typename T>
coordinate<T>::coordinate(coordinate<T>&& Coordinate) noexcept
    : x(std::move(Coordinate.x))
    , y(std::move(Coordinate.y))
{
}

template<typename T>
coordinate<T>::coordinate(area<T>&& Area) noexcept
    : x(std::move(Area.x))
    , y(std::move(Area.y))
{
}

template<typename T>
coordinate<T>& coordinate<T>::operator=(const coordinate<T>& Coordinate)
{
    this->x = Coordinate.x;
    this->y = Coordinate.y;
    return *this;
}

template<typename T>
coordinate<T>& coordinate<T>::operator=(const area<T>& Area)
{
    this->x = Area.width;
    this->y = Area.height;
    return *this;
}

template<typename T>
coordinate<T>& coordinate<T>::operator=(coordinate<T>&& Coordinate) noexcept
{
    this->x = std::move(Coordinate.x);
    this->y = std::move(Coordinate.y);
    return *this;
}

template<typename T>
coordinate<T>& coordinate<T>::operator=(area<T>&& Area) noexcept
{
    this->x = std::move(Area.width);
    this->y = std::move(Area.height);
    return *this;
}

template<typename T>
bool coordinate<T>::operator==(const coordinate<T>& Coordinate) const
{
    return (this->x == Coordinate.x) && (this->y == Coordinate.y);
}

template<typename T>
bool coordinate<T>::operator!=(const coordinate<T>& Coordinate) const
{
    return (this->x != Coordinate.x) || (this->y != Coordinate.y);
}

template<typename T>
area<T>::area(T Width, T Height)
    : width(Width)
    , height(Height)
{
}

template<typename T>
area<T>::area(const area<T>& Area)
    : width(Area.width)
    , height(Area.height)
{
}

template<typename T>
area<T>::area(const coordinate<T>& Coordinate)
    : width(Coordinate.X)
    , height(Coordinate.Y)
{
}

template<typename T>
area<T>::area(area<T>&& Area) noexcept
    : width(std::move(Area.width))
    , height(std::move(Area.height))
{
}

template<typename T>
area<T>::area(coordinate<T>&& Coordinate) noexcept
    : width(std::move(Coordinate.X))
    , height(std::move(Coordinate.Y))
{
}

template<typename T>
area<T>& area<T>::operator=(const area<T>& Area)
{
    this->width = Area.width;
    this->height = Area.height;
    return *this;
}

template<typename T>
area<T>& area<T>::operator=(const coordinate<T>& Coordinate)
{
    this->width = Coordinate.x;
    this->height = Coordinate.y;
    return *this;
}

template<typename T>
area<T>& area<T>::operator=(area<T>&& Area) noexcept
{
    this->width = std::move(Area.width);
    this->height = std::move(Area.height);
    return *this;
}

template<typename T>
area<T>& area<T>::operator=(coordinate<T>&& Coordinate) noexcept
{
    this->width = std::move(Coordinate.x);
    this->height = std::move(Coordinate.y);
    return *this;
}

template<typename T>
bool area<T>::operator==(const area<T>& Area) const
{
    return (this->width == Area.width) && (this->height == Area.height);
}

template<typename T>
bool area<T>::operator!=(const area<T>& Area) const
{
    return (this->width != Area.width) || (this->height != Area.height);
}

} // namespace gvw