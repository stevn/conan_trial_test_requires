#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

char const * tw_version();

bool tw_write_gray_ramp_tiff(const char* filename, int width, int height);

#ifdef __cplusplus
}
#endif
