#ifndef __RENDER_H__
#define __RENDER_H__

#include <stdlib.h>

#include "color.h"
#include "vector.h"
#include "texture.h"
#include "shader.h"

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

void render_draw_triangle_with_buffer(render_ctx *ctx, vector3 *pts, float *zbuf, color c);

void render_draw_triangle_with_buffer_and_texture(render_ctx *ctx, vector3 *pts, vector2 *uvs, float *zbuf, texture *tex);

void render_draw_triangle_with_buffer_and_texture_and_shader(render_ctx *ctx, vector3 *pts,
                                                             vector2 *uvs, float *zbuf,
                                                             texture *tex,
                                                             model *model_,
                                                             matrix modelview,
                                                             matrix projection,
                                                             matrix viewport,
                                                             vector3 light_dir,
                                                             shader_ctx *s_ctx,
                                                             shader *shader_);

#endif // __RENDER_H__