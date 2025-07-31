#pragma once

#include <string>

namespace tiff_core
{

bool write_gray_ramp_tiff(const std::string& filename, int width, int height);

}
