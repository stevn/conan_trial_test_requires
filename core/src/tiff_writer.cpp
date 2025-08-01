#include "tiff_writer.hpp"

#include <tiffio.h>

#include <vector>

namespace tiff_core
{

bool write_gray_ramp_tiff(const std::string& filename, int width, int height) {
    if (filename.empty() || width < 1 || height < 1) return false;

    TIFF* out = TIFFOpen(filename.c_str(), "w");
    if (!out) return false;

    TIFFSetField(out, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(out, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 3);
    TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    
    std::vector<unsigned char> row(width * 3);
    for (int y = 0; y < height; y++) {
        int gray = (y * 255) / (height - 1);
        for (int x = 0; x < width; x++) {
            row[x * 3 + 0] = gray;
            row[x * 3 + 1] = gray;
            row[x * 3 + 2] = gray;
        }
        TIFFWriteScanline(out, row.data(), y, 0);
    }

    TIFFClose(out);
    return true;
}

}
