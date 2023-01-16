#pragma once

// Standard includes
#include <string>

// External includes
#include <CImg.h>

// Local includes
#include "types.hpp"

namespace gvw {

image LoadImage(const char* imagePath, int colorChannels);
image LoadRGBImage(const char* imagePath);
image LoadRGBAImage(const char* imagePath);

} // namespace gvw