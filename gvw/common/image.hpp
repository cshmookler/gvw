#pragma once

/**
 * @file image.hpp
 * @author Caden Shmookler (cshmookler@gmail.com)
 * @brief Image loading and manipulation functions
 * @date 2023-02-02
 */

// Standard includes
#include <string>

// Local includes
#include "types.hpp"

namespace gvw {

/// @brief Loads an image from a path relative to the executable.
/// @remark Uses STB to load the image. The returned image object must be
/// assigned ownership to an L-value.
/// @param imagePath
/// @param colorChannels
/// @return
image LoadImage(const char* imagePath, int colorChannels);

/// @brief Loads an RGB image from a path relative to the executable.
/// @remark Transparency in the image is not loaded. Uses STB to load the image.
/// The returned image object must be assigned ownership to an L-value.
/// @param imagePath
/// @return
image LoadRGBImage(const char* imagePath);

/// @brief Loads an RGBA image from a path relative to the executable.
/// @remark Uses STB to load the image. The returned image object must be
/// assigned ownership to an L-value.
/// @param imagePath
/// @return
image LoadRGBAImage(const char* imagePath);

} // namespace gvw