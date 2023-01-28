#include "image.tpp"

// Standard includes
#include <cstdint>
#include <string>

// External includes
#define STB_IMAGE_IMPLEMENTATION
#include "../../external/stb-master/stb_image.h"

// Local includes
#include "const.hpp"
#include "global.hpp"

namespace gvw {

image LoadImage(const char* imagePath, int colorChannels)
{
    image imageBuffer;
    uint8_t* pixelBuffer = stbi_load(imagePath,
                                     &imageBuffer.width,
                                     &imageBuffer.height,
                                     &imageBuffer.componentsPerPixel,
                                     colorChannels);
    if (pixelBuffer == nullptr) {
        ERROR_CALLBACK(ERROR_ICON_FAILED_TO_LOAD,
                       ERROR_MESSAGE_ICON_FAILED_TO_LOAD);
        return imageBuffer;
    }
    size_t pixelBufferSize = size_t(imageBuffer.width) *
                             size_t(imageBuffer.height) *
                             size_t(imageBuffer.componentsPerPixel);
    imageBuffer.pixelData.assign(pixelBuffer, pixelBuffer + pixelBufferSize);
    stbi_image_free(pixelBuffer);

    return imageBuffer;
}

image LoadRGBImage(const char* imagePath)
{
    return LoadImage(imagePath, 3);
}

image LoadRGBAImage(const char* imagePath)
{
    return LoadImage(imagePath, 4);
}

} // namespace gvw