#pragma once

// Local includes
#include "gvw-types.hpp"

template<typename Type, typename Base>
gvw::strong_typedef<Type, Base>::strong_typedef() noexcept(
    (std::is_nothrow_default_constructible<Type>::value))
    : internal()
{
}
template<typename Type, typename Base>
gvw::strong_typedef<Type, Base>::strong_typedef(const Type& Value) noexcept(
    (std::is_nothrow_constructible<Type>::value))
    : internal(Value)
{
}
template<typename Type, typename Base>
gvw::strong_typedef<Type, Base>::strong_typedef(
    const strong_typedef&
        Derived) noexcept((std::is_nothrow_copy_constructible<Type>::value))
    : internal(Derived.internal)
{
}
template<typename Type, typename Base>
gvw::strong_typedef<Type, Base>::strong_typedef(Type&& Value) noexcept(
    (std::is_nothrow_move_constructible<Type>::value))
    : internal(std::move(Value))
{
}
template<typename Type, typename Base>
gvw::strong_typedef<Type, Base>::strong_typedef(
    strong_typedef&&
        Derived) noexcept((std::is_nothrow_move_constructible<Type>::value))
    : internal(std::move(Derived.internal))
{
}
template<typename Type, typename Base>
gvw::strong_typedef<Type, Base>& gvw::strong_typedef<Type, Base>::operator=(
    const Type& Value) noexcept((std::is_nothrow_assignable<Type, Type>::value))
{
    this->internal = Value;
    return *this;
}
template<typename Type, typename Base>
gvw::strong_typedef<Type, Base>& gvw::strong_typedef<Type, Base>::operator=(
    const strong_typedef&
        Derived) noexcept((std::is_nothrow_assignable<Type, Type>::value))
{
    this->internal = Derived.internal;
    return *this;
}
template<typename Type, typename Base>
gvw::strong_typedef<Type, Base>& gvw::strong_typedef<Type, Base>::operator=(
    Type&& Value) noexcept((std::is_nothrow_move_assignable<Type>::value))
{
    this->internal = std::move(Value);
    return *this;
}
template<typename Type, typename Base>
gvw::strong_typedef<Type, Base>&
gvw::strong_typedef<Type, Base>::operator=(strong_typedef&& Derived) noexcept(
    (std::is_nothrow_move_assignable<Type>::value))
{
    this->internal = std::move(Derived.internal);
    return *this;
}
template<typename Type, typename Base>
gvw::strong_typedef<Type, Base>::operator const Type&() const
{
    return this->internal;
}
template<typename Type, typename Base>
gvw::strong_typedef<Type, Base>::operator Type&()
{
    return this->internal;
}
template<typename Type, typename Base>
bool gvw::strong_typedef<Type, Base>::operator==(
    const strong_typedef& Derived) const
{
    return this->internal == Derived.internal;
}
template<typename Type, typename Base>
bool gvw::strong_typedef<Type, Base>::operator<(
    const strong_typedef& Derived) const
{
    return this->internal < Derived.internal;
}
template<typename Type, typename Base>
bool gvw::strong_typedef<Type, Base>::operator>(
    const strong_typedef& Derived) const
{
    return this->internal > Derived.internal;
}
template<typename Type, typename Base>
bool gvw::strong_typedef<Type, Base>::operator<=(
    const strong_typedef& Derived) const
{
    return !bool(this->internal > Derived.internal);
}
template<typename Type, typename Base>
bool gvw::strong_typedef<Type, Base>::operator>=(
    const strong_typedef& Derived) const
{
    return !bool(this->internal < Derived.internal);
}
template<typename Type, typename Base>
bool gvw::strong_typedef<Type, Base>::operator!=(
    const strong_typedef& Derived) const
{
    return !bool(this->internal == Derived.internal);
}
template<typename Type, typename Base>
bool gvw::strong_typedef<Type, Base>::operator<=(const Type& Value) const
{
    return !bool(this->internal > Value);
}
template<typename Type, typename Base>
bool gvw::strong_typedef<Type, Base>::operator>=(const Type& Value) const
{
    return !bool(this->internal < Value);
}
template<typename Type, typename Base>
bool gvw::strong_typedef<Type, Base>::operator!=(const Type& Value) const
{
    return !bool(this->internal == Value);
}
template<typename Type, typename Base>
Type& gvw::strong_typedef<Type, Base>::Get()
{
    return this->internal;
}
template<typename Type, typename Base>
const Type& gvw::strong_typedef<Type, Base>::Get() const
{
    return this->internal;
}
template<typename Type, typename Base>
Type* gvw::strong_typedef<Type, Base>::operator->()
{
    return &this->internal;
}
template<typename Type, typename Base>
const Type* gvw::strong_typedef<Type, Base>::operator->() const
{
    return &this->internal;
}
template<typename Type, typename Base>
constexpr bool operator>(const Type& Value,
                         const gvw::strong_typedef<Type, Base>& Derived)
{
    return Value > Derived.internal;
}
template<typename Type, typename Base>
constexpr bool operator<(const Type& Value,
                         const gvw::strong_typedef<Type, Base>& Derived)
{
    return Value < Derived.internal;
}
template<typename Type, typename Base>
constexpr bool operator<=(const Type& Value,
                          const gvw::strong_typedef<Type, Base>& Derived)
{
    return !bool(Value > Derived.internal);
}
template<typename Type, typename Base>
constexpr bool operator>=(const Type& Value,
                          const gvw::strong_typedef<Type, Base>& Derived)
{
    return !bool(Value < Derived.internal);
}
template<typename Type, typename Base>
constexpr bool operator==(const Type& Value,
                          const gvw::strong_typedef<Type, Base>& Derived)
{
    return Value == Derived.internal;
}
template<typename Type, typename Base>
constexpr bool operator!=(const Type& Value,
                          const gvw::strong_typedef<Type, Base>& Derived)
{
    return !bool(Value == Derived.internal);
}

template<typename... Args>
gvw::terminator<Args...>::terminator::terminator(deleter_signature Deleter,
                                                 Args... Arguments)
    : deleter(Deleter)
    , args(Arguments...)
{
}
template<typename... Args>
gvw::terminator<Args...>::terminator::~terminator()
{
    std::apply(this->deleter, this->args);
}

template<typename T>
gvw::glfw_hint<T>::glfw_hint(int Hint, T Value)
    : HINT(Hint)
    , value(Value)
{
}
template<typename T>
gvw::glfw_hint<T>::glfw_hint(const glfw_hint<T>& GLFW_Hint)
    : HINT(GLFW_Hint.HINT)
    , value(GLFW_Hint.value)
{
}
template<typename T>
gvw::glfw_hint<T>::glfw_hint(glfw_hint<T>&& GLFW_Hint) noexcept
    : HINT(std::move(GLFW_Hint.HINT))
    , value(std::move(GLFW_Hint.value))
{
}
template<typename T>
gvw::glfw_hint<T>& gvw::glfw_hint<T>::operator=(T Value)
{
    this->value = Value;
    return *this;
}
template<gvw::int_hint_function IntHintFunc,
         gvw::string_hint_function StringHintFunc,
         size_t IntHints,
         size_t StringHints>
gvw::glfw_hints<IntHintFunc, StringHintFunc, IntHints, StringHints>::glfw_hints(
    std::array<glfw_hint<int>, IntHints>&& Int_Hints,
    std::array<glfw_hint<const char*>, StringHints>&& String_Hints)
    : intHints(std::move(Int_Hints))
    , stringHints(std::move(String_Hints))
{
}
template<gvw::int_hint_function IntHintFunc,
         gvw::string_hint_function StringHintFunc,
         size_t IntHints,
         size_t StringHints>
gvw::glfw_hints<IntHintFunc, StringHintFunc, IntHints, StringHints>::glfw_hints(
    const glfw_hints& GLFW_Hints)
    : intHints(GLFW_Hints.intHints)
    , stringHints(GLFW_Hints.stringHints)
{
}
template<gvw::int_hint_function IntHintFunc,
         gvw::string_hint_function StringHintFunc,
         size_t IntHints,
         size_t StringHints>
gvw::glfw_hints<IntHintFunc, StringHintFunc, IntHints, StringHints>&
gvw::glfw_hints<IntHintFunc, StringHintFunc, IntHints, StringHints>::operator=(
    const glfw_hints& GLFW_Hints)
{
    this->intHints = GLFW_Hints.intHints;
    this->stringHints = GLFW_Hints.stringHints;
    return *this;
}
template<gvw::int_hint_function IntHintFunc,
         gvw::string_hint_function StringHintFunc,
         size_t IntHints,
         size_t StringHints>
gvw::glfw_hints<IntHintFunc, StringHintFunc, IntHints, StringHints>&
gvw::glfw_hints<IntHintFunc, StringHintFunc, IntHints, StringHints>::operator=(
    glfw_hints&& GLFW_Hints) noexcept
{
    this->intHints = std::move(GLFW_Hints.intHints);
    this->stringHints = std::move(GLFW_Hints.stringHints);
    return *this;
}
template<gvw::int_hint_function IntHintFunc,
         gvw::string_hint_function StringHintFunc,
         size_t IntHints,
         size_t StringHints>
void gvw::glfw_hints<IntHintFunc, StringHintFunc, IntHints, StringHints>::
    Apply() const
{
    for (const glfw_hint<int>& intHint : this->intHints) {
        IntHintFunc(intHint.HINT, intHint.value);
    }
    for (const glfw_hint<const char*>& stringHint : this->stringHints) {
        StringHintFunc(stringHint.HINT, stringHint.value);
    }
}

template<typename T>
gvw::coordinate<T>::coordinate(T X, T Y) // NOLINT
    : x(X)
    , y(Y)
{
}
template<typename T>
gvw::coordinate<T>::coordinate(const area<T>& Area)
    : x(Area.width)
    , y(Area.height)
{
}
template<typename T>
gvw::coordinate<T>::coordinate(area<T>&& Area) noexcept
    : x(std::move(Area.width))
    , y(std::move(Area.height))
{
}
template<typename T>
gvw::coordinate<T>& gvw::coordinate<T>::operator=(const area<T>& Area)
{
    this->x = Area.width;
    this->y = Area.height;
    return *this;
}
template<typename T>
gvw::coordinate<T>& gvw::coordinate<T>::operator=(area<T>&& Area) noexcept
{
    this->x = std::move(Area.width);
    this->y = std::move(Area.height);
    return *this;
}
template<typename T>
bool gvw::coordinate<T>::operator==(const coordinate<T>& Coordinate) const
{
    return (this->x == Coordinate.x) && (this->y == Coordinate.y);
}
template<typename T>
bool gvw::coordinate<T>::operator!=(const coordinate<T>& Coordinate) const
{
    return (this->x != Coordinate.x) || (this->y != Coordinate.y);
}

template<typename T>
gvw::area<T>::area(T Width, T Height)
    : width(Width)
    , height(Height)
{
}
template<typename T>
gvw::area<T>::area(const coordinate<T>& Coordinate)
    : width(Coordinate.X)
    , height(Coordinate.Y)
{
}
template<typename T>
gvw::area<T>::area(coordinate<T>&& Coordinate) noexcept
    : width(std::move(Coordinate.X))
    , height(std::move(Coordinate.Y))
{
}
template<typename T>
gvw::area<T>& gvw::area<T>::operator=(const coordinate<T>& Coordinate)
{
    this->width = Coordinate.x;
    this->height = Coordinate.y;
    return *this;
}
template<typename T>
gvw::area<T>& gvw::area<T>::operator=(coordinate<T>&& Coordinate) noexcept
{
    this->width = std::move(Coordinate.x);
    this->height = std::move(Coordinate.y);
    return *this;
}
template<typename T>
bool gvw::area<T>::operator==(const area<T>& Area) const
{
    return (this->width == Area.width) && (this->height == Area.height);
}
template<typename T>
bool gvw::area<T>::operator!=(const area<T>& Area) const
{
    return (this->width != Area.width) || (this->height != Area.height);
}