#include "tiff_writer_api.h"

#include "tiff_writer.hpp"

#include <stdexcept>

extern "C" char const * tw_version()
{
    return "tiff_writer 0.1.0";
}

extern "C" bool tw_write_gray_ramp_tiff(const char* filename, int width, int height)
{
    try {
        if (!filename) return false;
        return tiff_core::write_gray_ramp_tiff(filename, width, height);
    } catch (std::exception const & e) {
        return false;
    }
}
