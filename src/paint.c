#include "paint.h"
#include <string.h>
#include <float.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define OFFSET_COORD(a) (a) * 200 + 250

paint_ctx* paint_new(model *model_, size_t model_num, light l, vector3 camera)
{
    paint_ctx *ctx = (paint_ctx *)malloc(sizeof(paint_ctx) + sizeof(model *) * model_num);
    if (ctx == NULL)
    {
        return NULL;
    }
    ctx->model_num = model_num;
    ctx->l = l;
    ctx->camera = camera;
    ctx->models[0] = model_;
    return ctx;
}

matrix _calculate_viewport(int x, int y, int w, int h, int depth)
{
    matrix m = matrix_identity(4);
    m.m[0][3] = x + w / 2.f;
    m.m[1][3] = y + h / 2.f;
    m.m[2][3] = depth / 2.f;

    m.m[0][0] = w / 2.f;
    m.m[1][1] = h / 2.f;
    m.m[2][2] = depth / 2.f;
    return m;
}

vector3 m2v(matrix m)
{
    return vector3_new(m.m[0][0] / m.m[3][0], m.m[1][0] / m.m[3][0], m.m[2][0] / m.m[3][0]);
}

matrix v2m(vector3 v)
{
    matrix m = matrix_new(4, 1);
    m.m[0][0] = v.x;
    m.m[1][0] = v.y;
    m.m[2][0] = v.z;
    m.m[3][0] = 1.f;
    return m;
}

int _calculate_camera(paint_ctx *p_ctx, matrix *projection, matrix *viewport)
{
    *projection = matrix_identity(4);
    *viewport = _calculate_viewport(-250, -250, SCREEN_WIDTH * 3 / 4, SCREEN_HEIGHT * 3 / 4, 255);
    projection->m[3][2] = 1; // 1.f / p_ctx->camera.z;
    return 1;
}

int paint(paint_ctx *p_ctx, render_ctx *r_ctx)
{
    matrix *projection = (matrix *)malloc(sizeof(matrix));
    if (projection == NULL)
    {
        return 0;
    }
    matrix *viewport = (matrix *)malloc(sizeof(matrix));
    if (viewport == NULL)
    {
        return 0;
    }
    _calculate_camera(p_ctx, projection, viewport);
    for (int j = 0; j < 1; j++)
    {
        model *model = p_ctx->models[j];
        size_t nfaces = model_nfaces(model);
        for (int i = 0; i < nfaces; i++)
        {
            vector3 screen_coords[3];
            vector3 world_coords[3];
            vector2 uv_coords[3];
            for (int k = 0; k < 3; k++)
            {
                vector3 v =
                    vector3_new(OFFSET_COORD(model->verts[i * model->nvertpf + k].pos.x),
                                OFFSET_COORD(model->verts[i * model->nvertpf + k].pos.y * -1),
                                OFFSET_COORD(model->verts[i * model->nvertpf + k].pos.z * -1));
                matrix _v = v2m(v);
                matrix _p = matrix_multiply(*projection, _v, 4, 4, 4, 1);
                matrix _m = matrix_multiply(*viewport, _p, 4, 4, 4, 1);
                vector3 r_m = m2v(_m);
                screen_coords[k] = vector3_new(r_m.x / 1, r_m.y / 1, r_m.z);
                world_coords[k] = v;
                uv_coords[k] = vector2_new(model->verts[i * model->nvertpf + k].uv.x,
                                           model->verts[i * model->nvertpf + k].uv.y);
            }
            vector3 n = vector3_cross((vector3_subtract(world_coords[2], world_coords[0])), (vector3_subtract(world_coords[1], world_coords[0])));
            n = vector3_normalize(n);
            float intensity = vector3_dot(n, p_ctx->l);
            if (intensity > 0)
            {
                float zbuffer[SCREEN_HEIGHT * SCREEN_WIDTH] = {-FLT_MAX};
                render_draw_triangle_with_buffer_and_texture(r_ctx, screen_coords, uv_coords,
                                                             zbuffer, model->tex);
                /*render_draw_triangle_with_buffer(
                    r_ctx, world_coords, zbuffer,
                    color_new(intensity * 1.0f, intensity * 1.0f, intensity * 1.0f, 1.0f));*/
            }
            
        }
    }
    return 1;
}