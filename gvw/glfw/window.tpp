#pragma once

#include "window.hpp"

namespace gvw {

template<typename type>
void window::ScreenCoordinateToPixel(type screenCoordinateX,
                                     type screenCoordinateY,
                                     type& pixelX,
                                     type& pixelY)
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }

    size<int> windowSizeInScreenCoordinates;
    glfwGetWindowSize(this->Id(),
                      &windowSizeInScreenCoordinates.width,
                      &windowSizeInScreenCoordinates.height);
    size<int> windowSizeInPixels;
    glfwGetFramebufferSize(
        this->Id(), &windowSizeInPixels.width, &windowSizeInPixels.height);

    coordinate<double> pixelOverScreenCoordinateRatio;
    pixelOverScreenCoordinateRatio.x =
        double(windowSizeInPixels.width) /
        double(windowSizeInScreenCoordinates.width);
    pixelOverScreenCoordinateRatio.y =
        double(windowSizeInPixels.height) /
        double(windowSizeInScreenCoordinates.height);

    pixelX = type(double(pixelX) * pixelOverScreenCoordinateRatio.x);
    pixelY = type(double(pixelY) * pixelOverScreenCoordinateRatio.y);
}

template<typename type>
coordinate<type> window::ScreenCoordinateToPixel(
    coordinate<type>& screenCoordinate)
{
    coordinate<type> pixel = { 0, 0 };
    this->ScreenCoordinateToPixel(
        screenCoordinate.x, screenCoordinate.y, pixel.x, pixel.y);
    return pixel;
}

template<typename type>
size<type> window::ScreenCoordinateToPixel(size<type>& screenCoordinate)
{
    size<type> pixel = { 0, 0 };
    this->ScreenCoordinateToPixel(screenCoordinate.width,
                                  screenCoordinate.height,
                                  pixel.width,
                                  pixel.height);
    return pixel;
}

template<typename type>
void window::PixelToScreenCoordinate(type pixelX,
                                     type pixelY,
                                     type& screenCoordinateX,
                                     type& screenCoordinateY)
{
    if (this->AssertCreation_() == internal::ASSERT_FAILURE) {
        return;
    }

    size<int> windowSizeInScreenCoordinates;
    glfwGetWindowSize(this->Id(),
                      &windowSizeInScreenCoordinates.width,
                      &windowSizeInScreenCoordinates.height);
    size<int> windowSizeInPixels;
    glfwGetFramebufferSize(
        this->Id(), &windowSizeInPixels.width, &windowSizeInPixels.height);

    coordinate<double> screenCoordinateOverPixelRatio;
    screenCoordinateOverPixelRatio.x =
        double(windowSizeInScreenCoordinates.width) /
        double(windowSizeInPixels.width);
    screenCoordinateOverPixelRatio.y =
        double(windowSizeInScreenCoordinates.height) /
        double(windowSizeInPixels.height);

    pixelX = type(double(pixelX) * screenCoordinateOverPixelRatio.x);
    pixelY = type(double(pixelY) * screenCoordinateOverPixelRatio.y);
}

template<typename type>
coordinate<type> window::PixelToScreenCoordinate(coordinate<type>& pixel)
{
    coordinate<type> screenCoordinate = { 0, 0 };
    this->PixelToScreenCoordinate(
        pixel.x, pixel.y, screenCoordinate.x, screenCoordinate.y);
    return screenCoordinate;
}

template<typename type>
size<type> window::PixelToScreenCoordinate(size<type>& pixel)
{
    size<type> screenCoordinate = { 0, 0 };
    this->PixelToScreenCoordinate(pixel.width,
                                  pixel.height,
                                  screenCoordinate.width,
                                  screenCoordinate.height);
    return screenCoordinate;
}

} // namespace gvw