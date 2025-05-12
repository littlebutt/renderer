#ifndef __RENDER_H__
#define __RENDER_H__

#include <stdlib.h>

#include "color.h"
#include "vector.h"

typedef struct {
    int left;
    int top;
    int width;
    int height;
    void *device;
} render_ctx;

render_ctx *render_new(int left, int top, int width, int height);

void render_loop(render_ctx *ctx);

void render_free(render_ctx *ctx);

void render_set_pixel(render_ctx *ctx, int x, int y, color c);

void render_draw_line(render_ctx *ctx, int x0, int y0, int x1, int y1, color c);

void render_draw_triangle(render_ctx *ctx, vector2 *pts, color c);

#endif // __RENDER_H__