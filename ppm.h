#pragma once

#include "stdafx.h"
#include "common.h"

/**
 * Output an image in ppm format 
 */
int WriteRGBImg(const char* path, int nx, int ny, Color *pix);

