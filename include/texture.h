#ifndef __TEXTURE__H__
#define __TEXTURE__H__

#include "color.h"
#include "stdlib.h"

typedef struct {
    int width;
    int height;
    color data[];
} texture;

color texture_sample(texture *tex, float u, float v);

#endif // __TEXTURE__H__
