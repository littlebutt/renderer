#ifndef __RENDER_H__
#define __RENDER_H__

#include <stdlib.h>

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

#endif // __RENDER_H__